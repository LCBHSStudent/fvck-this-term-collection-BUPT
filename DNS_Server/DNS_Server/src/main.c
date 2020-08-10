#include <config.h>

#include "hash_table/hash_table.h"
#include "ini_handler/ini_handler.h"
#include "utils_helper/utils_helper.h"

#define SUGGESTED_SZIE 1024

#pragma warning(disable:4996)

// TODO: 完成ID表中超时项目的删除？
//		（FUNC: start another thread or just handle it in ev loop）
// TODO: 完成从external server response中拿到查询IP的功能
//		（FUNC: onReadResponse）

// -------NATIVE VARIABLE & FUNCTION SPACE--------- //

static IDTable		idTable  = { 0 };		// Id转换表
static DNSTable		dnsTable = { 0 };		// DNS域名转换表
static HashTable	dnsHashTable;	// DNS域名hash

static size_t		idRowCount  = 0;	// Id转换表行数
static size_t		dnsRowCount = 0;	// DNS转换表行数

static SYSTEMTIME	sysTime;		// 系统时间
static TIME			sysTimeLocal;	// 保存系统时间的独立变量

static char EXTERN_SERVER_HOST[16];
static char LOCAL_SERVER_HOST[16];
static int	DNS_SERVER_PORT = 0;

byte2 GetNewID(
	byte2				oldID,
	struct sockaddr_in* addr,
	BOOL				isDone,
	int					offset,
	int					joinTime,
	char*				domain
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
static struct sockaddr_in clientEP;

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

void onSend2Client(
	uv_udp_send_t*	req,
	int				status
);

void onSend2Server(
	uv_udp_send_t* req,
	int				status
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
		uv_ip4_addr(ANY_HOST, DNS_SERVER_PORT, &localEP);
		uv_udp_bind(
			&localSocket,
			(const struct sockaddr*)&localEP,
			UV_UDP_REUSEADDR
		);
		if (uv_udp_recv_start(&localSocket, allocBuffer, onReadRequest)) {
			PRINTERR("failed to listen local endpoint");
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
			PRINTERR("failed to listen external server endpoint");
			exit(-3);
		}
	}

	// 同步系统时间
	{
		SyncTime(&sysTime, &sysTimeLocal);
		DisplayTime(&sysTime);
	}

	// 运行事件循环
	int ret = uv_run(loop, UV_RUN_DEFAULT);
	// 退出后做清理工作
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
	} if (nread > 1000) {
		return;
	}

	char sender[16] = {0};
	uv_ip4_name((const struct sockaddr_in*)addr, sender, 15);
	
	DNSHeader* pHeader = (DNSHeader*)buffer->base;
	void* pData = buffer->base + sizeof(DNSHeader);
	char* pUrl	= ParseUrlFromData(pData, (int)(nread - 16));

	if (pUrl == NULL) {
		free(buffer->base);
		return;
	}
	DisplayTime(&sysTime);
	printf("\t[client's request url] %s\n", pUrl);

	char* result = FindItemByKey(dnsHashTable, pUrl);
	
	if (result) {
		// 本地缓存中找到要查找的dns地址，构建报文返回客户端
		
		byte2 newID =
			GetNewID(
				nhswap_s(pHeader->Id),
				(struct sockaddr_in*)addr,
				TRUE, (int)nread, 0, pUrl
			);
		DisplayIDTransInfo(&idTable[newID]);


		byte2 temp = nhswap_s(0x8180);
		pHeader->Flags = temp;

		// 不良网站拦截啊嗯
		if (strcmp(result, "0.0.0.0") == 0) {
			printf("\t[notification] domain was found in the local cache, but it is banned\n");
			// 回答数为0，即屏蔽
			temp = nhswap_s(0x0000);
		}
		else {
			printf("\t[result found] destnation ipv4 address is: %s\n", result);
			// 服务器响应，回答数为1
			temp = nhswap_s(0x0001);
		}
		pHeader->AnswerNum = temp;

		// 构造DNS报文响应部分
		byte answer[16] = { 0 };
		byte2* pNum = (byte2*)(&answer[0]);
		{
			byte2 Name = nhswap_s(0xc00c);
			memcpy(pNum, &Name, sizeof(byte2));
			pNum += 1;

			byte2 TypeA = nhswap_s(0x0001);
			memcpy(pNum, &TypeA, sizeof(byte2));
			pNum += 1;

			byte2 ClassA = nhswap_s(0x0001);
			memcpy(pNum, &ClassA, sizeof(byte2));
			pNum += 1;

			byte4 timeLive = nhswap_l(0x7b);
			memcpy(pNum, &timeLive, sizeof(byte4));
			pNum += 2;

			byte2 IPLen = nhswap_s(0x0004);
			memcpy(pNum, &IPLen, sizeof(byte2));
			pNum += 1;

			byte4 IP = inet_addr_t(result);
			memcpy(pNum, &IP, sizeof(byte4));

			memcpy(buffer->base + nread, answer, 16);
		}
		
		// 回送request报文
		uv_udp_send_t* sendResponse =
			malloc(sizeof(uv_udp_send_t));
		uv_buf_t		responseBuf = 
			uv_buf_init((char*)malloc(1024), (byte4)nread + 16);
		memcpy(responseBuf.base, buffer->base, nread+16);
		
		uv_ip4_addr(
			sender,
			nhswap_s(idTable[newID].client.sin_port), &clientEP
		);

		uv_udp_send(
			sendResponse,
			&localSocket,
			&responseBuf, 1,
			(const struct sockaddr*)&clientEP,
			onSend2Client
		);
	}
	else {
		// 本地缓存中缺失，构建请求报文送往外部dns服务器
		PRINTERR("\t[notification] local cache missed, sending request to external server");
		pHeader->Id = nhswap_s(
			GetNewID(
				nhswap_s(pHeader->Id),
				(struct sockaddr_in*)addr,
				FALSE, (int)nread, 0, pUrl
			)
		);

		DisplayIDTransInfo(&idTable[nhswap_s(pHeader->Id)]);

		// 转发request报文
		uv_udp_send_t*	sendRequest =
			malloc(sizeof(uv_udp_send_t));
		uv_buf_t		requestBuf =
			uv_buf_init((char*)malloc(1024), (byte4)nread);

		memcpy(requestBuf.base, buffer->base, nread);
		uv_udp_send(
			sendRequest,
			&serverSocket,
			&requestBuf, 1,
			(const struct sockaddr*)&serverEP,
			onSend2Server
		);
	}

	// 回收资源
	free(buffer->base);
}


// 收到远程DNS服务器送达的响应报文
void onReadResponse(
	uv_udp_t* req,
	ssize_t					nread,
	const uv_buf_t*			buffer,
	const struct sockaddr*	addr,
	unsigned				flags
) {
	if (nread < 0 || nread > 1024) {
		fprintf(stderr, "Read error %s\n", uv_err_name((int)nread));
		uv_close((uv_handle_t*)req, NULL);
		free(buffer->base);
		return;
	}

	// 获取发送方IP raw字符串
	char sender[16] = { 0 };
	uv_ip4_name((const struct sockaddr_in*)addr, sender, 15);

	DNSHeader* pHeader		= (DNSHeader*)buffer->base;
	byte2 temp				= nhswap_s(pHeader->Id);
	idTable[temp].finished	= true;
	byte2 prevID			= nhswap_s(idTable[temp].prevID);

	// 如果未加入hashTable，则将key(url) & value(ipaddr)加入hashTable
	if (!FindItemByKey(dnsHashTable, idTable[temp].url)) {
		InsertHashItem(dnsHashTable, idTable[temp].url, NULL);
	}

	byte* pData = buffer->base + sizeof(DNSHeader);
	void* pIP	= pData + idTable[temp].offset;
	byte4 integerIP = nhswap_l(*(byte4*)pIP);

	// 获取第一条查询结果
	char result[17] = { 0 };
	uv_ip4_name((const struct sockaddr_in*)&integerIP, result, 16);
	
	{
		strcpy(result, dnsTable[dnsRowCount].IP);
		strcpy(idTable[temp].url, dnsTable[dnsRowCount].Domain);
		InsertHashItem(dnsHashTable, idTable[temp].url, result);

		dnsRowCount++;
		if (dnsRowCount >= MAX_AMOUNT) {
			PRINTERR("[[WARNING]] RECORDS IN DNS_TABLE HAVE REACHED THE LIMIT");
			exit(-114514);
		}
	}

	// 打印操作执行状态信息
	DisplayTime(&sysTime);
	printf("\t[get_external_server_response] the first result is: %s\n", result);
	DisplayIDTransInfo(&idTable[temp]);

	pHeader->Id = prevID;

	// 转发request报文
	uv_udp_send_t*	forwardResponse =
		malloc(sizeof(uv_udp_send_t));
	uv_buf_t		forwardBuf =
		uv_buf_init((char*)malloc(1024), (byte4)nread);

	memcpy(forwardBuf.base, buffer->base, nread);

	char client[17] = { 0 };
	
	uv_ip4_name(&idTable[temp].client, client, 16);

//	printf("**************%s\n", client);

	uv_ip4_addr(
		client,
		nhswap_s(idTable[temp].client.sin_port),
		&clientEP
	);

	uv_udp_send(
		forwardResponse,
		&localSocket,
		&forwardBuf, 1,
		(const struct sockaddr*)&clientEP,
		onSend2Client
	);

	free(buffer->base);
}

void onSend2Client (
	uv_udp_send_t*	req,
	int				status
) {
	// uv_udp_recv_start(req->handle, allocBuffer, onReadRequest);
	if (status != 0) {
		fprintf(stderr, 
			"*[send_client_error] %s\n", uv_strerror(status));
	}
}

void onSend2Server(
	uv_udp_send_t* req,
	int				status
) {
	if (status != 0) {
		fprintf(stderr,
			"*[send_server_error] %s\n", uv_strerror(status));
	}
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
		PRINTERR("failed to load config value : EXTERNAL_ENDPOINT_IPV4");
		exit(-1);
	}
	// 仅校验长度
	if (strlen(data) > 15) {
		PRINTERR("external_endpoint out of range");
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
		PRINTERR("failed to load config value : LOCAL_ENDPOINT_IPV4");
		exit(-1);
	}
	if (strlen(data) > 15) {
		PRINTERR("local_endpoint out of range");
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

// 将请求ID转换为新的ID，并将信息填入ID转换表中
byte2 GetNewID(
	byte2				oldID,
	struct sockaddr_in* addr,
	BOOL				isDone,
	int					offset,
	int					joinTime,
	char*				url
) {
	idTable[idRowCount].prevID		= oldID;
	idTable[idRowCount].client		= *addr;
	idTable[idRowCount].finished	= isDone;
	idTable[idRowCount].offset		= offset;
	idTable[idRowCount].joinTime	= joinTime;
	strcpy(idTable[idRowCount].url, url);
	
	idRowCount = (idRowCount+1) % MAX_AMOUNT;

	return (byte2)((idRowCount + MAX_AMOUNT - 1) % MAX_AMOUNT);
}

void loadDNSTableData() {
	FILE* pFile = NULL;
	dnsHashTable = NewHashTable(MAX_AMOUNT);

	if (NULL == (pFile = fopen("dns_table.txt", "r"))) {
		PRINTERR("failed to open dns_table.txt");
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