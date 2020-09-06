#pragma once

#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <config.h>

size_t ELFHash(void* pData, size_t size);

// ------------TYPE DEFINATION------------- //

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

// ------------MEMBER FUNCTIONS------------ //

HashTable NewHashTable(size_t size);

BOOL DeleteHashTable(HashTable table);

BOOL RemoveHashItem(HashTable table, char* key);

BOOL RemoveHashItemByNode(HashTable table, Node* node);

BOOL InsertHashItem(HashTable table, char* key, void* value);

void* FindValueByKey(HashTable table, char* key);

Node* FindNodeByKey(HashTable table, char* key);

#endif