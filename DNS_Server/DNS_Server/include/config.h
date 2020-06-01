#pragma once

#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

#include <uv.h>

// len of char array 
#define LENGTH				65
#define MAX_AMOUNT			300
#define DNS_SERVER_PORT		53
#define BUFFER_SIZE			1024
#define LOCAL_HOST			"127.0.0.1"
#define EXTERN_DNS_HOST		"192.168.1.1"

typedef unsigned short	byte2;
typedef unsigned int	byte4;
typedef unsigned char	byte;

typedef enum BOOL_t {
	false = 0,
	true
} bool;

// DNS报文头
typedef struct DNSHeader_t {
	byte2		Id;
	byte2		Flags;
	byte2		QueryNum;
	byte2		AnswerNum;
	byte2		AuthorNum;
	byte2		AdditionNum;
} DNSHeader, *pDNSHeader;

// DNS域名解析表的Row
typedef struct TransDNSRow_t {
	byte2		IP;
	char		Domain[LENGTH];
} TransDNSRow;

// DNS域名表
typedef TransDNSRow DNSTable[MAX_AMOUNT];

// ID转换表的Row
typedef struct TransIDRow_t {
	byte2		prevID;
	bool		finished;
	SOCKADDR_IN client;
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