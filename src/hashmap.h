/*
 * hashmap.h
 *
 *  Created on: 2017年10月12日
 *      Author: zoubin
 */

#ifndef SRC_HASHMAP_H_
#define SRC_HASHMAP_H_
#include <stdbool.h>
typedef int (*Comparator)(void*, void*);
typedef int (*Hash)(void*);
typedef struct str_hashmap Hashmap;
typedef struct{
    void* key;
	void* value;
}KeyPair;
int hash_hash_string(void*);
int hash_compare_string(void*, void*);
Hashmap* hash_map_create(Hash, Comparator, int);
bool hash_map_set(Hashmap*, void*, void*);
void* hash_map_get(Hashmap*, void*);
void* hash_map_remove(Hashmap*, void*);
void hash_map_destory(Hashmap*);
KeyPair* hash_map_iterator(Hashmap* hm);
KeyPair* hash_map_next(KeyPair* it);
#endif /* SRC_HASHMAP_H_ */
