#pragma once
#include <config.h>

#define PRINTERR(MSG) fprintf(stderr, MSG "\n")

void 
	SyncTime(SYSTEMTIME* sysTime, TIME* sysTimeLocal);
char* 
	ParseUrlFromData(char* baseBuffer, char* buffer, int size);
int
	Len2RespDataEnd(char* pData);
void* 
	FindAnswerStart(byte* pData);
bool
	isIPv4Str(const char* pData);

void 
	DisplayTime(SYSTEMTIME* sysTime);
void 
	DisplayIDTransInfo(TransIDRow* info);

byte2 
	nhswap_s(byte2 value);
byte4 
	nhswap_l(byte4 value);
long
	atol_t(const char* nptr);
ULONG 
	inet_addr_t(IN char* cp);