#include "hash_table.h"

HashTable NewHashTable(size_t size) {
	HashTable _temp;
	_temp.size = size;
	_temp.data = (Node*)malloc(size * sizeof(Node));

	memset(_temp.data, 0, sizeof(Node) * size);

	return _temp;
}

#pragma warning(disable:6001)

BOOL DeleteHashTable(HashTable table) {
	if (table.size == 0) {
		return false;
	}
	else {
		size_t tableSize = table.size;
		size_t index = 0;
		Node* node = NULL;

		for (index = 0; index < table.size; index++) {
			node = table.data[index].next;
			while (node != NULL) {
				Node* temp = node;
				node = node->next;
				free(temp);
			}
		}
		free(table.data);
		table.size = 0;

		return true;
	}
}

BOOL InsertHashItem(HashTable table, char* key, char* value) {
	if (table.size == 0)
		return false;

	size_t index = ELFHash(key, strlen(key)) % table.size;
	Node* node = &table.data[index];

	if (node->key == NULL || node->value == NULL) {
		table.data[index].key = key;
		table.data[index].value = value;
	}
	else {
		while (node->next != NULL) {
			node = node->next;
		}
		Node* temp = (Node*)malloc(sizeof(Node));
		if (temp != NULL) {
			temp->key	= key;
			temp->value = value;
			temp->next	= NULL;
		}
		node->next  = temp;
	}

	return true;
}

char* FindItemByKey(HashTable table, char* key) {
	if (table.size == 0 || key == NULL)
		return NULL;

	size_t index = ELFHash(key, strlen(key)) % table.size;
	Node* node = &table.data[index];
	
	while (node != NULL) {
		if (node->key == NULL || node->value == NULL) {
			return NULL;
		}
		if (strcmp(key, node->key) == 0) {
			return node->value;
		}
		else {
			node = node->next;
		}
	}

	return NULL;
}

// ----------------ELF HASH----------------- //

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
		temp = 0,
		cnt = 0;
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