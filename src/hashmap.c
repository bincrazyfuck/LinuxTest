/*
 * hashmap.c
 *
 *  Created on: 2017年10月12日
 *      Author: zoubin
 */
#include <string.h>
#include <malloc.h>
#include <stdint.h>
#include <sys/mman.h>
#include "hashmap.h"
typedef struct str_node{
	int hash;
	void* key;
	void* data;
	struct str_node* next;
}MapNode;

struct str_hashmap{
	Comparator compare;
	Hash hash;
	int size;
	MapNode* node[0];
};

typedef struct{
	KeyPair kp;
	Hashmap* hm;
	int index;
	MapNode* next;
}Iterator;

int hash_compare_string(void* a, void* b){
	return strcmp(a, b);
}

int hash_hash_string(void* key){
	char* c=key;
	int32_t h=0;
	while(*c!='\0'){
		h=31*h+*c++;
	}
	return h;
}

Hashmap* hash_map_create(Hash h, Comparator c, int size){
//	Hashmap* hm=malloc(sizeof(Hashmap)+sizeof(MapNode*)*size);
	Hashmap* hm=(Hashmap*)mmap(NULL, sizeof(Hashmap)+sizeof(MapNode*)*size, PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
	//printf("%d",sizeof(Hashmap)+sizeof(MapNode*)*size);
	hm->compare=c;
	hm->hash=h;
	hm->size=size;
	int i;
	for(i=0;i<size;i++){
		hm->node[i]=NULL;
	}
	return hm;
}

bool hash_map_set(Hashmap* hm, void* key, void* data){
	int hash=hm->hash(key);
	int bucket=(unsigned int)hash%hm->size;
	MapNode** n=&hm->node[bucket];
	while(*n!=NULL){
		MapNode* c=*n;
		if(c->hash==hash&&hm->compare(c->key,key)==0){
			c->data=data;
			return false;
		}
		n=&c->next;
	}
	*n=malloc(sizeof(MapNode));
	MapNode* mn=*n;
	mn->hash=hash;
	mn->key=key;
	mn->data=data;
	mn->next=NULL;
	return true;
}

void* hash_map_get(Hashmap* hm, void* key){
	int hash=hm->hash(key);
	int bucket=(unsigned int)hash%hm->size;
	MapNode** n=&hm->node[bucket];
	while(*n!=NULL){
		MapNode* c=*n;
		if(c->hash==hash&&hm->compare(c->key,key)==0){
			return c->data;
		}
		n=&c->next;
	}
	return NULL;
}

void* hash_map_remove(Hashmap* hm, void* key){
	int hash=hm->hash(key);
	int bucket=(unsigned int)hash%hm->size;
	MapNode** n=&hm->node[bucket];
	while(*n!=NULL){
		MapNode* c=*n;
		if(c->hash==hash&&hm->compare(c->key,key)==0){
			*n=c->next;
			void* data=c->data;
			free(c);
			return data;
		}
		n=&c->next;
	}
	return NULL;
}

void hash_map_destory(Hashmap* hm){
	int i;
	for(i=0;i<hm->size;i++){
		MapNode* mn=hm->node[i];
		while(mn!=NULL){
			MapNode* m=mn;
			mn=m->next;
			free(m);
		}
	}
	free(hm);
}

KeyPair* hash_map_iterator(Hashmap* hm){
	Iterator* i=malloc(sizeof(Iterator));
	i->kp.key=NULL;
	i->kp.value=NULL;
	i->hm=hm;
	i->index=-1;
	i->next=NULL;
	return &i->kp;
}

KeyPair* hash_map_next(KeyPair* it){
	Iterator* i=(Iterator*)it;
	if(i->next){
		i->kp.key=i->next->key;
		i->kp.value=i->next->data;
		i->next=i->next->next;
		return &i->kp;
	}else{
		for(i->index++;i->index<i->hm->size;i->index++){
			MapNode* mn=i->hm->node[i->index];
			if(mn!=NULL){
				i->kp.key=mn->key;
				i->kp.value=mn->data;
				i->next=mn->next;
				return &i->kp;
			}
		}
		free(i);
		return NULL;
	}
}

