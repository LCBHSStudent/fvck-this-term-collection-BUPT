#pragma once

#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

#include <uv.h>

#define ARR_LENGTH			64				// len of char array 
#define MAX_AMOUNT			300
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

// DNS报文头
typedef struct DNSHeader_t {
	byte2		Id;				// 标识数
	byte2		Flags;			// 标志
	byte2		QueryNum;		// 请求序号
	byte2		AnswerNum;		// 回答序号
	byte2		AuthorNum;		// 权威序号
	byte2		AdditionNum;	// 附加RR数
} DNSHeader, *pDNSHeader;

// DNS域名解析表的Row
typedef struct TransDNSRow_t {
	char		IP[ARR_LENGTH];			// IP地址
	char		Domain[ARR_LENGTH];		// 域名
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