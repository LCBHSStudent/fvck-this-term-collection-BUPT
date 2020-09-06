#pragma once

#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

#include <uv.h>

#define ARR_LENGTH			64				// len of char array 
#define MAX_AMOUNT			1024
#define BUFFER_SIZE			1024
#define ANY_HOST			"0.0.0.0"
#define INI_PATH			"config.ini"

#define DEBUG_FLAG

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
	byte2		Name;			// 不知道是啥的name
	byte2		Type;			// 查询结果类型CNAME & A & AAAA...
	byte2		Class;			// In & Out?
	byte4		TTL;			// Effective Time
	byte2		DataLength;		// Length of response answer
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

typedef struct TIME_t {
	byte	Day;
	byte    Hour;
	byte	Minute;
	byte	Second;
	byte4   Milliseconds;
} TIME;

#endif