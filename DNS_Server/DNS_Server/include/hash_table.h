#pragma once

#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <config.h>

typedef struct Node_t {
	char*			name;
	char*			dest;
	struct Node_t*	next;
} Node;

typedef struct HashTable_t {
	size_t	tableSize;
	Node*	tablePtr;
} HashTable;

HashTable NewHashTable(size_t size) {
	HashTable _temp;
	_temp.tableSize = size;
	_temp.tablePtr  = (Node*)malloc(size * sizeof(Node));
	return _temp;
}

BOOL DeleteHashTable(HashTable* table) {
	if (table == NULL) {
		return false;
	}
	else {
		size_t tableSize = table->tableSize;
		
		return true;
	}
}

// 取hash类型的总bit数
#define TOTAL_BITS \
	(sizeof(size_t) * 8)
// 取低位bit数
#define THREE_QUARTERS \
	((TOTAL_BITS * 3) / 4)
// 取高位bit数
#define ONE_EIGHTH \
	sizeof(size_t)
// 取
#define HIGH_BITS \
	(((size_t)-1) << (TOTAL_BITS - ONE_EIGHTH))

size_t ELFHash(void* pData, size_t size) {
	size_t	
		magic = 0,
		temp  = 0,
		cnt   = 0;
	register size_t
		hash = 0;
	byte*  
		data = (byte*)pData;

	while (cnt < size) {
		temp = (size_t) * (data + cnt++);
		hash = (hash << ONE_EIGHTH) + temp;
		if ((magic = hash & HIGH_BITS) != 0) {
			hash ^= (magic >> THREE_QUARTERS);
			hash &= ~magic;
		}
	}

	return hash;
}

#endif