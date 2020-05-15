#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uv.h"

static uv_loop_t* loop = NULL;
static uv_tcp_t l_server;

static void
uv_alloc_buf(uv_handle_t* handle,
	size_t suggested_size,
	uv_buf_t* buf) {
	if (handle->data != NULL) {
		free(handle->data);
		handle->data = NULL;
	}

	buf->base = malloc(suggested_size + 1);
	buf->len = suggested_size;
	handle->data = buf->base;
}

static void
on_close(uv_handle_t* handle) {
	printf("client close!\n");
	if (handle->data) {
		free(handle->data);
		handle->data = NULL;
	}
}

static void
on_shutdown(uv_shutdown_t* req, int status) {
	uv_close((uv_handle_t*)req->handle, on_close);
	free(req);
}

static void
after_write(uv_write_t* req, int status) {
	if (status == 0) {
		printf("write success!\n");
	}

	uv_buf_t* w_buf = req->data;
	if (w_buf) {
		free(w_buf);
	}
	free(req);
}

static void
after_read(uv_stream_t* stream,
	ssize_t nread,
	const uv_buf_t* buf) {
	if (nread < 0) {
		uv_shutdown_t* reg = malloc(sizeof(uv_shutdown_t));
		memset(reg, 0, sizeof(uv_shutdown_t));
		uv_shutdown(reg, stream, on_shutdown);
		return;
	}

	buf->base[nread] = 0;
	printf("recv %d\n", nread);
	printf("%s\n", buf->base);

	// test send
	uv_write_t* w_req = malloc(sizeof(uv_write_t));
	uv_buf_t* w_buf = malloc(sizeof(uv_buf_t));
	w_buf->base = buf->base;
	w_buf->len = nread;
	w_req->data = w_buf;
	uv_write(w_req, stream, w_buf, 1, after_write);
}

static void
uv_connection(uv_stream_t* server, int status) {
	printf("new client come in\n");
	uv_tcp_t* client = malloc(sizeof(uv_tcp_t));
	memset(client, 0, sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	uv_accept(server, (uv_stream_t*)client);

	//让event loop管理读
	uv_read_start((uv_stream_t*)client, uv_alloc_buf, after_read);
}

int main(int argc, char** argv) {
	int ret;

	loop = uv_default_loop();

	uv_tcp_init(loop, &l_server);

	// 配置我想要eventloop帮我管理的类型
	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", 6080, &addr);
	ret = uv_tcp_bind(&l_server, (const struct sockaddr*) & addr, 0);
	if (ret != 0) {
		goto failed;
	}
	uv_listen((uv_stream_t*)&l_server, SOMAXCONN, uv_connection);

	uv_run(loop, UV_RUN_DEFAULT);

failed:
	printf("end\n");
	system("pause");
	return 0;
}