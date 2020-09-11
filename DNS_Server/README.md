@[TOC](<center>DNS中继服务器实验报告<br><h6>作者:RYF GZR<h6></center>)

## 1. 功能设计 ##
### 1.1 不良网站拦截 ###
* 对查询结果的ip进行检测，如果为0.0.0.0，则向客户端返回“域名不存在”的报错信息。
### 1.2 服务器功能 ###
*  如果查询结果的ip为普通ip地址，则向客户端返回这个地址。
> 以下为功能1.1和1.2的实现代码

```cpp
// 不良网站拦截
if (
/*result->Type == A &&*/
// 检测查询结果的ip是否为0.0.0.0
strcmp(result->Data, "0.0.0.0") == 0
) {
// 打印“域名不存在的”错误信息
printf("\t[notification] domain was found in the local cache, but it is banned\n");
// 回答数为0，即屏蔽
temp = nhswap_s(0x0000);
}
else {
// 打印查询到的ip地址
printf("\t[result found] destnation result is: %s\n", result->Data);
// 服务器响应，回答数为1
temp = nhswap_s(0x0001);
}
```
### 1.3 中继功能 ###
* 如果本地缓存缺失，则构建请求报文送往外部dns服务器

```cpp	
if (result && result->TTL > ToSecond(&sysTimeLocal)) {该部分代码略}
else {
	if (result) {

	}
	// 本地缓存中缺失，构建请求报文送往外部dns服务器
	
	// 打印“本地缓存缺失，向外部服务器发送请求”
	PRINTERR("\t[notification] local cache missed, sending request to external server");
	// 获取ID
	pHeader->Id = nhswap_s(
		GetNewID(
			nhswap_s(pHeader->Id),
			(struct sockaddr_in*)addr,
			FALSE, (int)nread, 0, pUrl
		)
	);
	
	// 打印ID对应的请求信息
	DisplayIDTransInfo(&idTable[nhswap_s(pHeader->Id)]);

	// 转发request报文

	// 分配空间
	uv_udp_send_t* sendRequest =
		malloc(sizeof(uv_udp_send_t));
	uv_buf_t		requestBuf =
		uv_buf_init((char*)malloc(1024), (byte4)nread);

	// 发送报文
	memcpy(requestBuf.base, buffer->base, nread);
	uv_udp_send(
		sendRequest,
		&serverSocket,
		&requestBuf, 1,
		(const struct sockaddr*)&serverEP,
		onSend2Server
	);
}
```
* 支持打印CNAME AAAA A类型的查询结果信息<br>
采用switch结构对查询到的结构类型进行分支
> 打印并缓存第一个IPv4类型的结果
```cpp
// 如果是IPv4类型 
case A: {
	char result[64] = { '\0' };
	int  index		= 0;
	// 获取IPv4地址
	for (int j = 1; j <= ansHeader.DataLength; j++) {
		sprintf(
			&result[0] + index,
			"%u", *(byte*)(ptrOffset + pAnsHeader + j - 1));
		index = (int)strlen(result);
		if (j != ansHeader.DataLength) {
			result[index++] = '.';
		}
	}
	
	// 打印IPv4地址
	printf("\t\t<Answer %d> A: IPv4地址 {\n", i);
	printf("\t\t\tQuery result:\t%s\n", result);
	
	{
		// 如果未加入hashTable，则将key(url) & value(ipaddr)加入hashTable
		TransDNSRow* pDnsCache = FindValueByKey(dnsHashTable, idTable[temp].url);
		if (!pDnsCache) {
			// 获取TTL
			SyncTime(&sysTime, &sysTimeLocal);
			byte4 TTL = ToSecond(&sysTimeLocal) + ansHeader.TTL;

			// 填充DNS表信息
			strcpy(dnsTable[dnsRowCount].Data, result);
			strcpy(dnsTable[dnsRowCount].Domain, idTable[temp].url);
			
			dnsTable[dnsRowCount].Type = A;
			dnsTable[dnsRowCount].TTL  = TTL;
			
			// 插入哈希表
			InsertHashItem(dnsHashTable, idTable[temp].url, &dnsTable[dnsRowCount]);
			
			// DNS表行数增加，并防止溢出
			dnsRowCount++;
			dnsRowCount %= MAX_AMOUNT;
		}
	}
} break;
```		
> 打印IPV6类型结果

```cpp
// 如果是IPv6类型
case AAAA: {
	char result[64] = { '\0' };
	int	 index		= 0;
	bool firstZFlag = false;
	// 获取IPv6地址
	for (int j = 0; j < ansHeader.DataLength; j += 2) {
		byte2 ipValue = nhswap_s(
			*(byte2*)(ptrOffset + pAnsHeader + j)
		);
		if (ipValue == 0) {
			if (!firstZFlag) {
				result[index++] = ':';
				firstZFlag = true;
			}
			continue;
		}
			
		sprintf(&result[0] + index, "%x", ipValue);
		index = (int)strlen(result);

		if (j+2 != ansHeader.DataLength) {
			result[index++] = ':';
		}
	}
	
	// 打印IPv6地址
	printf("\t\t<Answer %d> AAAA: IPv6地址 {\n", i);
	printf("\t\t\tQuery result:\t%s\n", result);
} break;
```
> 打印CNAME类型结果

```cpp
// 如果是CNAME类型
case CNAME: {
	// 获取Url
	char* result = ParseUrlFromData(
		buffer->base,
		ptrOffset + pAnsHeader, ansHeader.DataLength
	);
	// 打印地址
	printf("\t\t<Answer %d> CNAME: 别名地址 {\n", i);
	printf("\t\t\tQuery result:\t%s\n", result);
} break;
```		
### 1.4 调试信息说明 ###
#### 1.4.1 调试级别1 ####
* 输出查询的域名以及对应的ip地址
#### 1.4.2 调试级别2 ####
* 输出时间坐标&&序号&&客户端ip地址&&查询的域名
#### 1.4.3 调试级别3 ####
* 输出数据报内容&&接收和发送数据包的地址以及端口&&查询的ip地址以及域名
### 1.5 多用户并发查询 ###
* 允许多个客户端的并发查询<br>
实现方法:基于libuv提供的异步I/O,onReadRequest和onReadResponse均为异步信号处理,从而实现了多用户并发查询。
### 1.6 超时处理 ###
* 缓存失效

```cpp
// 检测本地缓存是否缺失和TTL是否超时
if (result && result->TTL > ToSecond(&sysTimeLocal)) {此部分代码略}
else {
	// 如果TTL超时，本地缓存命中说明缓存失效
	if (result) {
		// 删除缓存数据，向外部服务器发送request报文
		RemoveHashItemByNode(dnsHashTable, resultNode);
		PRINTERR("\t*[notification] TTL over limit, removing cache data...");
		PRINTERR("\t\t\t\tre-sending request to external server...");
	}
```			
* 外部DNS应答迟返回

```cpp
// check whether is out of the time limit
SyncTime(&sysTime, &sysTimeLocal);
int curTime = (int)ToSecond(&sysTimeLocal);
// 检测应答时间是否超过2秒
if (curTime - idTable[temp].joinTime > 2) {
	// 删除缓存并打印超时信息
	free(buffer->base);
	PRINTERR("**[warning] Get over-time external dns server response");
	PRINTERR("**[warning] response data dropped");
	return;
}
```		
## 2. 模块划分 ##
DNS服务器主模块包含四个子模块
### 2.1 命令行参数处理 ###
* 根据命令行输入的参数，设置标志数据，控制消息的输出。通过不同的命令行参数，使DNS服务器显示不同程度的提示和调试信息。
### 2.2 本地解析 ###
* 在本地缓存文件中，查找从应用程序来的请求解析的域名，得到其对应的ip地址。然后构造DNS应答数据包返回给应用程序。
### 2.3 外部DNS服务器解析 ###
* 在本地解析失败后（本地缓存缺失），将应用程序发送的DNS请求报文转发给外部DNS服务器，然后接收应答，根据此应答对应用程序进行应答。
### 2.4 解析响应报文的answer字段 ###
* 对外部DNS服务器发送的响应报文的answer字段进行解析，对查询到的结果类型进行分支处理。
## 3. 系统流程图 ##
![程序流程图](https://img-blog.csdnimg.cn/20200910170653861.png)

## 4. 程序运行说明 ##
1. 配置ini文件
2. 运行DNS_Server程序
3. 正常使用nslookup，ping，ftp，IE等，名字解析正常
4. 局域网上的其他计算机将域名服务器指向DNS中继服务器的IP地址，ftp，IE等均能正常工作
## 5. 测试用例及运行结果 ##
>nslookup  
>server 127.0.0.1
1. 屏蔽dns_table.txt内记录为0.0.0.0
> dns_table.txt > 0.0.0.0 financial.washingtonpost.com  

控制台打印信息
```bash
"[systime_info]: current system time: 2020-09-10 17:39:40:771
        [client's request url] financial.washingtonpost.com
        [id_transaction_info]
                [join_time] 927580
                [client_addr] 127.0.0.1
                [previous_id] 6
                [request_url] financial.washingtonpost.com
                [is_finished] 1
                [request_offset] 46
        *[notification] domain was found in the local cache, but it is banned
```
2. 查询未位于dns_table.txt内的域名 (eg: www.baidu.com)
>www.baidu.com  

客户端收到响应
```bash
服务器:  localhost
Address:  127.0.0.1

非权威应答:
名称:    www.baidu.com
Addresses:  182.61.200.6
          182.61.200.7
```
服务端打印信息  

——debug_level_1——输出查询到的域名信息
```bash
"*[result found] destnation result is: 182.61.200.6"
```

——debug_level_2——输出查询信息
```bash
"[client's request url] www.baidu.com
        [id_transaction_info]
                [join_time] 928007
                [client_addr] 127.0.0.1
                [previous_id] 8
                [request_url] www.baidu.com
                [is_finished] 1
                [request_offset] 31
```

——debug_level_3——报文原生信息
```bash
>>>>>>>>START OF RAW RESPONSE DATA
"0x00 0x04 0x81 0x80 0x00 0x01 0x00 0x03 0x00 0x00 0x00 0x00 0x03 0x77 0x77 0x77 0x05 0x62 0x61 0x69 0x64 0x75 0x03 0x63 0x6f 0x6d 0x00 0x00 0x01 0x00 0x01 0xc0 0x0c 0x00 0x05 0x00 0x01 0x00 0x00 0x03 0x90 0x00 0x0f 0x03 0x77 0x77 0x77 0x01 0x61 0x06 0x73 0x68 0x69 0x66 0x65 0x6e 0xc0 0x16 0xc0 0x2b 0x00 0x01 0x00 0x01 0x00 0x00 0x00 0x36 0x00 0x04 0xb6 0x3d 0xc8 0x06 0xc0 0x2b 0x00 0x01 0x00 0x01 0x00 0x00 0x00 0x36 0x00 0x04 0xb6 0x3d 0xc8 0x07"
>>>>>>>>>>END OF RAW RESPONSE DATA
```
————域名Answer字段解析
```bash
"[GET EXTERNAL DNS SERVER ANSWER] (count: 3)
                <Answer 1> CNAME: 别名地址 {
                        Query result:   www.a.shifen.com
                }

                <Answer 2> A: IPv4地址 {
                        Query result:   182.61.200.6
                }

        [id_transaction_info]
                [join_time] 928007
                [client_addr] 127.0.0.1
                [previous_id] 7
                [request_url] www.baidu.com
                [is_finished] 1
                [request_offset] 31

                <Answer 3> A: IPv6地址 {
                        Query result:   182.61.200.7
                }

        [id_transaction_info]
                [join_time] 928007
                [client_addr] 127.0.0.1
                [previous_id] 7
                [request_url] www.baidu.com
                [is_finished] 1
                [request_offset] 31"
```

3. 经过一段时间后再次查询 www.baidu.com，出现缓存失效现象
>www.baidu.com  

```bash
"[systime_info]: current system time: 2020-09-10 17:58:45:339
        [client's request url] www.baidu.com
        *[notification] TTL over limit, removing cache data...
                                re-sending request to external server...
        [id_transaction_info]
                [join_time] 928725
                [client_addr] 127.0.0.1
                [previous_id] 9
                [request_url] www.baidu.com
                [is_finished] 0
                [request_offset] 31
```

4. 收到外部服务器的超时返回报文，drop此报文段
```bash
"[systime_info]: current system time: 2020-09-10 18:10:24:810
        [client's request url] www.baidu.com
        **[warning] Get over-time external dns server response
        **[warning] response data dropped
```
## 6. 团队分工 ##
**GZR**  
1. 编写程序主要事件处理逻辑  
2. 设计部分缓存数据结构  
3. 辅助进行浏览器网页浏览测试  
4. 设计测试用例以及测试场景

**RYF**  
1. 编写程序功能辅助函数  
2. 分析报文格式  
3. 负责程序内存与性能测试  
4. 编写额外功能实现用数据结构  
## 7. 遇到的问题及心得体会 ##
RYF：  
&emsp;&emsp;本次计算机网络课程实验中，我通过编写处理网络报文与socket数据相关的各类辅助函数，使得对于计算机网络通信有了更深层次的了解。结合本次使用的libuv，使我对epool模型也有了初步的认识。本次实验中我主要遇到的难点是对原生报文进行处理，通过wireshark抓包分析与观察规则相配合才能对dns报文的格式有一定掌握，从而正确地从其中萃取出所需的信息；此外，本次实验也大大巩固了我的c语言编程能力，我从中受益匪浅。<br>

GZR
&emsp;&emsp;对于网络编程经验很少的我，这次计算机网络课程实验无疑对我是一次考验，首先感谢RYF同学对我的悉心指导，帮助我解决了许多实验中遇到的困难，同时也帮助我提高了c语言编程能力。在实验刚开始的阶段，我对DNS服务的运作流程并不清晰，对dns报文格式也并不熟悉，因此花费了大量时间去熟悉报文格式，并且使用wireshark抓包进行实际地观察。之后在RYF同学的指导下，对DNS服务的运作以及细节渐渐熟悉，顺利编写了程序主要事件处理逻辑。本次实验也是我第一次使用libuv，libuv提供的各种接口极大程度上方便了我们实验的进行，由此感受到每一次实验的成功都包含着前人的铺垫，感谢他们的付出，感谢老师的倾囊相授，感谢队友RYF同学的悉心指导。
## 8. 源代码数据结构 ##
### 8.1 哈希表 ###
* 哈希表结构

```cpp
// 在此处仅引用内存
typedef struct Node_t {
	char*			key;
	void*			value;
	struct Node_t*	prev;
	struct Node_t*	next;
} Node;

typedef struct HashTable_t {
	size_t	size;
	Node*	data;
} HashTable;
```		
* 哈希表成员函数

```cpp
// 新建哈希表
HashTable NewHashTable(size_t size);
// 删除哈希表
BOOL DeleteHashTable(HashTable table);
// 删除哈希表中指定元素
BOOL RemoveHashItem(HashTable table, char* key);
// 删除哈希表中节点
BOOL RemoveHashItemByNode(HashTable table, Node* node);
// 在哈希表中插入元素
BOOL InsertHashItem(HashTable table, char* key, void* value);
// 在哈希表中找到指定元素
void* FindValueByKey(HashTable table, char* key);
// 在哈希表中找到指定节点
Node* FindNodeByKey(HashTable table, char* key);
```		
### 8.2 配置文件辅助函数 ###
* 对配置文件进行处理的辅助函数	

```cpp
/*
 * @prarms：        title
 *                      配置文件中一组数据的标识
 *                  key
 *                      这组数据中要读出的值的标识
 *                  filename
 *                      要读取的文件路径
 * @return：        有效字符串 / NULL
 */
char* GetIniKeyString(
	const char* title,
	const char* key,
	const char* filename
);

/*
 * @params：        title
 *                      配置文件中一组数据的标识
 *                  key
 *                      这组数据中要读出的值的标识
 *                  filename
 *                      要读取的文件路径
 * @return：         有效INT值 / 0
 */
int GetIniKeyInt(
	const char* title,
	const char* key,
	const char* filename
);

/*
 * @params：        title
 *                      配置文件中一组数据的标识
 *                  key
 *                      这组数据中要读出的值的标识
 *                  val
 *                      更改后的值
 *                  filename
 *                      要读取的文件路径
 * @return：         success: 0 / failed: -1
 */
int PutIniKeyString(
	const char* title,
	const char* key,
	const char* val,
	const char* filename
);

/*
 * @params：        title
 *                      配置文件中一组数据的标识
 *                  key
 *                      这组数据中要读出的值的标识
 *                  val
 *                      更改后的值
 *                  filename
 *                      要读取的文件路径
 * @return：         success: 0 / failed: -1
 */
int PutIniKeyInt(
	const char* title,
	const char* key,
	int			val,
	const char* filename
);
```			
### 8.3 工具类辅助函数 ###
* 各种工具类辅助函数

```cpp
// 获取系统当前时间
void 
	SyncTime(SYSTEMTIME* sysTime, TIME* sysTimeLocal);
// 获取Url
char* 
	ParseUrlFromData(char* baseBuffer, char* buffer, int size);
// 获取Data长度
int
	Len2RespDataEnd(char* pData);
// 找到answer字段的起始位置
void* 
	FindAnswerStart(byte* pData);
// 计算时间差值
int32_t 
	GetTimeSubValue(TIME* pFormer, TIME* pLater);
// 将时间转换成秒
byte4
	ToSecond(TIME* pTime);

// 打印当前系统时间
void 
	DisplayTime(SYSTEMTIME* sysTime);
// 打印ID对应的请求信息
void 
	DisplayIDTransInfo(TransIDRow* info);

// 网络字节序转换
byte2 
	nhswap_s(byte2 value);
byte4 
	nhswap_l(byte4 value);

// 字符串转整型
long
	atol_t(const char* nptr);
// IPv4字符串转为无符号long
ULONG 
	inet_addr_t(IN char* cp);	
```			
### 8.4 config中数据结构 ###
```cpp
#define ARR_LENGTH			64				// 字符数组长度 
#define MAX_AMOUNT			1024			// DNS表和ID表最大长度
#define BUFFER_SIZE			1024			// 缓冲区大小
#define ANY_HOST			"0.0.0.0"		// IPv4_ANY
#define INI_PATH			"config.ini"	// 配置文件

#define DEBUG_FLAG							// debug标志

typedef unsigned short	byte2;
typedef unsigned int	byte4;
typedef unsigned char	byte;

typedef enum BOOL_t {
	false = 0,
	true
} bool;

enum ANS_TYPE {
	A		= 1,				// IPV4地址
	NS		= 2,				// 域名服务器
	CNAME	= 5,				// 别名
	SOA		= 6,				// 授权
	WKS		= 11,				// 熟识服务
	PTR		= 12,				// IP转换为域名
	HINFO	= 13,				// 主机信息
	MX		= 15,				// 邮件交换
	AAAA	= 28,				// IPv6	
	AXFR	= 252,				// 整个区的请求
	ANY		= 255				// 对所有记录的请求
};

// DNS报文头
typedef struct DNSHeader_t {
	byte2		Id;				// 标识数
	byte2		Flags;			// 标志
	byte2		QueryNum;		// 请求序号
	byte2		AnswerNum;		// 回答序号
	byte2		AuthorNum;		// 权威序号
	byte2		AdditionNum;	// 附加RR数
} DNSHeader, *pDNSHeader;

// #pragma pack(2)
// DNS报文中Answer段头部 注意由于内存对齐导致的数据无法取到问题
typedef struct DNSAnswerHeader_t {
	byte2		Name;			// Name
	byte2		Type;			// 查询结果类型CNAME & A & AAAA...
	byte2		Class;			// In & Out
	byte4		TTL;			// TTL
	byte2		DataLength;		// 响应报文answer字段长度
} DNSAnswerHeader, *pDNSAnswerHeader;
// #pragma pack()

// DNS域名解析表的Row
typedef struct TransDNSRow_t {
	byte2		Type;					// Answer Type
	byte4		TTL;					// 有效期
	byte		Data[ARR_LENGTH];		// IP地址
	byte		Domain[ARR_LENGTH];		// 域名
} TransDNSRow;

// DNS域名表
typedef TransDNSRow DNSTable[MAX_AMOUNT];

// ID转换表的Row
typedef struct TransIDRow_t {
	byte2				prevID;				// 原ID
	bool				finished;			// 是否完成解析
	struct sockaddr_in	client;				// 客户套接字地址
	int					joinTime;			// 加入转换表的时刻
	int					offset;				// 客户发送报文的字节数
	char				url[ARR_LENGTH];	// 客户要查询的url
} TransIDRow;

// ID转换表
typedef TransIDRow IDTable[MAX_AMOUNT];

// 时间结构
typedef struct TIME_t {
	byte	Day;
	byte    Hour;
	byte	Minute;
	byte	Second;
	byte4   Milliseconds;
} TIME;
```		
### 8.5 main中数据结构 ###
* 自建全局变量

```cpp
static IDTable		idTable  = { 0 };		// Id转换表
static DNSTable		dnsTable = { 0 };		// DNS域名转换表
static HashTable	dnsHashTable;			// DNS域名hash

static size_t		idRowCount  = 0;		// Id转换表行数
static size_t		dnsRowCount = 0;		// DNS转换表行数

static SYSTEMTIME	sysTime;				// 系统时间
static TIME			sysTimeLocal;			// 保存系统时间的独立变量

static char EXTERN_SERVER_HOST[16];			// 外部服务器地址
static char LOCAL_SERVER_HOST[16];			// 本地服务器地址
static int	DNS_SERVER_PORT = 0;			// DNS服务器端口
```		
* 自建函数

```cpp
// 对查询事务进行ID转换，便于进行查询信息的缓存
// 将请求ID转换为新的ID，并将信息填入ID转换表中
byte2 GetNewID(
	byte2				oldID,
	struct sockaddr_in* addr,
	BOOL				isDone,
	int					offset,
	char*				domain
);
// 读取DNS本地解析文件与生成hash表
void loadDNSTableData();
// 加载配置文件
void initConfig();
// 退出时清理
void cleanup();
```		
* libuv全局变量

```cpp
static uv_loop_t*	loop = NULL;		// 循环标志
static uv_udp_t		localSocket;		// 本地DNS通信Socket
static uv_udp_t		serverSocket;		// 远程DNS通信Socket

// endpoints
static struct sockaddr_in localEP;		// 本地
static struct sockaddr_in serverEP;		// 服务端
static struct sockaddr_in clientEP;		// 客户端
```		
* libuv函数

```cpp
// 读取请求报文 
void onReadRequest (
	uv_udp_t*				req,
	ssize_t					nread,
	const uv_buf_t*			buffer,
	const struct sockaddr*	addr,
	unsigned				flags
);

// 读取响应报文
void onReadResponse (
	uv_udp_t* req,
	ssize_t					nread,
	const uv_buf_t*			buffer,
	const struct sockaddr*	addr,
	unsigned				flags
);

// 分配缓冲区
void allocBuffer(
	uv_handle_t*	handle,
	size_t			suggested_size,
	uv_buf_t*		buffer
);

// 检测向client发送是否成功
void onSend2Client(
	uv_udp_send_t*	req,
	int				status
);

// 检测向server发送是否成功
void onSend2Server(
	uv_udp_send_t* req,
	int				status
);
```		
