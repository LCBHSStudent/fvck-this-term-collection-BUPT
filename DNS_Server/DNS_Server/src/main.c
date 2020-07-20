#include <config.h>

#include "hash_table/hash_table.h"
#include "ini_handler/ini_handler.h"
#include "utils_helper/utils_helper.h"

#pragma warning(disable:4996)

// -------NATIVE VARIABLE & FUNCTION SPACE--------- //

static IDTable		idTable  = { 0 };		// Id转换表
static DNSTable		dnsTable = { 0 };		// DNS域名转换表
static HashTable	dnsHashTable;	// DNS域名hash

static size_t		idRowCount  = 0;	// Id转换表行数
static size_t		dnsRowCount = 0;	// DNS转换表行数

static SYSTEMTIME	sysTime;		// 系统时间
static TIME			sysTimeLocal;	// 保存系统时间的独立变量

static char	url[ARR_LENGTH];

static char EXTERN_SERVER_HOST[16];
static char LOCAL_SERVER_HOST[16];
static int	DNS_SERVER_PORT = 0;

byte2 GetNewID(
	byte2				oldID,
	struct sockaddr_in* addr,
	BOOL				isDone,
	char* domain
);
void loadDNSTableData();
void initConfig();
void cleanup();


// --------UV VARIABLE & FUNCTION SPACE-------- //

static uv_loop_t*	loop = NULL;
static uv_udp_t		localSocket;
static uv_udp_t		serverSocket;
static struct sockaddr_in localEP;
static struct sockaddr_in serverEP;

void onReadRequest (
	uv_udp_t*				req,
	ssize_t					nread,
	const uv_buf_t*			buffer,
	const struct sockaddr*	addr,
	unsigned				flags
);

void onReadResponse (
	uv_udp_t* req,
	ssize_t					nread,
	const uv_buf_t*			buffer,
	const struct sockaddr*	addr,
	unsigned				flags
);

void allocBuffer(
	uv_handle_t*	handle,
	size_t			suggested_size,
	uv_buf_t*		buffer
);

// ---------Main----------- //

int main(int argc, char* argv[]) {
	// 加载配置文件
	initConfig();
	// 读取DNS本地解析文件与生成hash表
	loadDNSTableData();

	loop = uv_default_loop();

	// 初始化本地DNS通信socket
	{
		uv_udp_init(loop, &localSocket);
		uv_ip4_addr(LOCAL_SERVER_HOST, DNS_SERVER_PORT, &localEP);
		uv_udp_bind(
			&localSocket,
			(const struct sockaddr*)&localEP,
			UV_UDP_REUSEADDR
		);
		if (uv_udp_recv_start(&localSocket, allocBuffer, onReadRequest)) {
			PRINTERR(failed to listen local endpoint);
			exit(-3);
		}
	}

	// 初始化远程DNS通信socket
	{
		uv_udp_init(loop, &serverSocket);
		uv_ip4_addr(EXTERN_SERVER_HOST, DNS_SERVER_PORT, &serverEP);
		uv_udp_bind(
			&serverSocket,
			(const struct sockaddr*)&serverEP,
			UV_UDP_REUSEADDR
		);
		if (uv_udp_recv_start(&serverSocket, allocBuffer, onReadResponse)) {
			PRINTERR(failed to listen external server endpoint);
			exit(-3);
		}
	}

	// 同步系统时间
	{
		GetLocalTime(&sysTime);
		sysTimeLocal.Day	= (byte)sysTime.wDay;
		sysTimeLocal.Hour	= (byte)sysTime.wHour;
		sysTimeLocal.Minute = (byte)sysTime.wMinute;
		sysTimeLocal.Second = (byte)sysTime.wSecond;
		sysTimeLocal.Milliseconds = (byte)sysTime.wMilliseconds;
		printf("sync system time: [%d月%d日] %d:%d:%d:%d\n",
			sysTime.wMonth,
			sysTimeLocal.Day,
			sysTimeLocal.Hour,
			sysTimeLocal.Minute,
			sysTimeLocal.Second,
			sysTimeLocal.Milliseconds
		);
	}


	int ret = uv_run(loop, UV_RUN_DEFAULT);
	{
		cleanup();
	}

	return ret;
}

void onReadRequest(
	uv_udp_t*				req,
	ssize_t					nread,
	const uv_buf_t*			buffer,
	const struct sockaddr*	addr,
	unsigned				flags
) {
	if (nread < 0) {
		fprintf(stderr, "Read error %s\n", uv_err_name((int)nread));
		uv_close((uv_handle_t*)req, NULL);
		free(buffer->base);
		return;
	}

	char sender[16] = {0};
	uv_ip4_name((const struct sockaddr_in*)addr, sender, 15);
	
	DNSHeader* pHeader = (DNSHeader*)buffer->base;
	void* pData = buffer->base + sizeof(DNSHeader);

	printf("%s\n", ParseUrlFromData(pData, (int)(nread - 16)));





	// 回收资源
	free(buffer->base);
}

void onReadResponse(
	uv_udp_t* req,
	ssize_t					nread,
	const uv_buf_t* buffer,
	const struct sockaddr* addr,
	unsigned				flags
) {
	if (nread < 0) {
		fprintf(stderr, "Read error %s\n", uv_err_name((int)nread));
		uv_close((uv_handle_t*)req, NULL);
		free(buffer->base);
		return;
	}

	char sender[16] = { 0 };
	uv_ip4_name((const struct sockaddr_in*)addr, sender, 15);
	
	free(buffer->base);
}

// 或者采用static内存+offset锁分配的方式？
void allocBuffer(
	uv_handle_t*	handle,
	size_t			suggested_size,
	uv_buf_t*		buffer
) {
	*buffer = 
		uv_buf_init((char*)malloc(suggested_size), (byte4)suggested_size);
}


void initConfig() {
	// 不需要free data，原函数中使用static数组做缓存
	char* data = GetIniKeyString(
		"DNS_CONFIG",
		"EXTERNAL_ENDPOINT_IPV4",
		INI_PATH
	);
	if (!data) {
		PRINTERR(failed to load config value : EXTERNAL_ENDPOINT_IPV4);
		exit(-1);
	}
	// 仅校验长度
	if (strlen(data) > 15) {
		PRINTERR(external_endpoint out of range);
		exit(-1);
	}
	strcpy(EXTERN_SERVER_HOST, data);
	printf("EXTERN_SERVER_HOST: %s\n", EXTERN_SERVER_HOST);

	data = GetIniKeyString(
		"DNS_CONFIG",
		"LOCAL_ENDPOINT_IPV4",
		INI_PATH
	);
	if (!data) {
		PRINTERR(failed to load config value : LOCAL_ENDPOINT_IPV4);
		exit(-1);
	}
	if (strlen(data) > 15) {
		PRINTERR(local_endpoint out of range);
		exit(-1);
	}
	strcpy(LOCAL_SERVER_HOST, data);
	printf("LOCAL_SERVER_HOST: %s\n", LOCAL_SERVER_HOST);

	DNS_SERVER_PORT = GetIniKeyInt(
		"DNS_CONFIG",
		"DNS_SERVER_PORT",
		INI_PATH
	);

}

byte2 GetNewID(
	byte2				oldID,
	struct sockaddr_in* addr,
	BOOL				isDone,
	char*				domain
) {

	return 0;
}

void loadDNSTableData() {
	FILE* pFile = NULL;
	dnsHashTable = NewHashTable(MAX_AMOUNT);

	if (NULL == (pFile = fopen("dns_table.txt", "r"))) {
		PRINTERR(failed to open dns_table.txt);
		exit(-2);
	}

	char data[256];
	dnsRowCount = 0;
	while (NULL != fgets(data, sizeof(data), pFile)) {
		char* pSpace = strchr(data, ' ');
		*pSpace = '\0';

		strcpy(dnsTable[dnsRowCount].IP, data);
		strcpy(dnsTable[dnsRowCount].Domain, pSpace + 1);

		size_t messIndex = strlen(pSpace + 1) - 1;
		dnsTable[dnsRowCount].Domain[messIndex] = '\0';

		InsertHashItem(
			dnsHashTable,
			dnsTable[dnsRowCount].Domain,
			dnsTable[dnsRowCount].IP
		);

		dnsRowCount++;
	}
	fclose(pFile);
}

void cleanup() {
	uv_udp_recv_stop(&localSocket);
	uv_udp_recv_stop(&serverSocket);

	DeleteHashTable(dnsHashTable);
}