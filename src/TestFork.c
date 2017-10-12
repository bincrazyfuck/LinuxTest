/*
 * TestFork.c
 *
 *  Created on: Oct 12, 2017
 *      Author: hadoop
 */
/*
 ============================================================================
 Name        : TestFork.c
 Author      : Bin
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "TestFork.h"
#include "hashmap.h"

static int forkNum=0;
void crFork(int cpuNum,int     *ptr);
int sem(char* key);
Hashmap* hm;
int size=100;
int main(void) {
	int cpuNum=4;
	printf("RootPID = %d\n", getpid());
	int     *ptr;
	ptr = (int*)mmap(NULL, sizeof(int[size]), PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
	sem_t* mutex;
	char *key="asd";
//	hm=hash_map_create(hash_hash_string, hash_compare_string, 128);
	crFork(cpuNum,ptr);
	return 0;
//	char* key="/fucksem";
//	sem(key);
}

int sem(char* key)
{
//    int     fd, i;
    int i, nloop = 10;//, zero = 0;
    int     *ptr;
	sem_t* mutex=sem_open(key,O_CREAT, 0644, 1);

    //  open a file and map it into memory
//    fd = open("log.txt", O_RDWR | O_CREAT, S_IRWXU);
//    write(fd,&zero,sizeof(int));

    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);

//    close(fd);

    /* create, initialize semaphore */
//    if(sem_init(&mutex, 1, 1) < 0) //
//    {
//        perror("semaphore initilization");
//        exit(0);
//    }

    if (fork() == 0)
    {   /* child process*/
        for (i = 0; i < nloop; i++)
        {
            sem_wait(mutex);

            printf("child: %d\n", (*ptr)++);
            //sleep(1);
            sem_post(mutex);
        }
        exit(0);
    }


    /* back to parent process */
    for (i = 0; i < nloop; i++)
    {
        sem_wait(mutex);
        printf("parent: %d\n", (*ptr)++);
        //sleep(1);
        sem_post(mutex);
    }
    sem_close(mutex);
    sem_unlink(key);
    exit(0);
}

void crFork(int cpuNum,int *ptr){
	char* key="asd";
		if(fork()!=0){
			if(forkNum++<cpuNum){
				crFork(cpuNum,ptr);
			}
			else{
				sleep(3);
//				for(;;){
					int j;
					for(j=0;j<size;j++){
						if(ptr[j]){
							printf("ptr%d is %d\n",j,ptr[j]);
						}
					}
					munmap(ptr,sizeof(int[size]));
//					if(*ptr==8000000){
//						printf("ptr is %d\n",*ptr);
//						munmap(ptr,sizeof(int));
//						sem_t *mutex=sem_open(key,O_RDWR|O_CREAT, 0644, 1);
//						sem_close(mutex);
//						sem_unlink(key);
//						break;
//					}
//				}
			}
		}
		else{
			if(forkNum==cpuNum){
				exit(0);
			}
			int j=(int)getpid();
			int ss=j+size;
			printf("PID = %d, Parent PID = %d,j=%d,ss=%d\n", getpid(), getppid(),j,ss);
			for(;j<ss;j++){
				char seccc[10];
				snprintf(seccc, sizeof(seccc), "%s%d", key, j);
				if(ptr[j-getpid()]){
//					printf("existxxPID = %d, Parent PID = %d, s = %s\n",getpid(), getppid(),ptr[j-getpid()]);
					continue;
				}
				else{
					sem_t *mutex=sem_open(seccc,O_CREAT|O_RDWR, 0644, 1);
					if (mutex == SEM_FAILED)
					{
						printf("sem_open failed! (%s)\n", strerror(errno));
						continue;
					}
					sem_wait(mutex);
					if(ptr[j-getpid()]){
//						printf("existPID = %d, Parent PID = %d, ptr[%d] = %d\n",getpid(), getppid(),j-getpid(),ptr[j-getpid()]);
						sem_post(mutex);
						sem_close(mutex);
						sem_unlink(seccc);
						continue;
					}
					ptr[j-getpid()]=j;
					if(ptr[j-getpid()])
					printf("create::PID = %d, Parent PID = %d,ptr[%d] = %d\n",getpid(), getppid(),j-getpid(),ptr[j-getpid()]);
					sem_post(mutex);
					sem_close(mutex);
					sem_unlink(seccc);
				}

			}
//			printf("PID = %d, Parent PID = %d,",getpid(), getppid());
//
//			sem_wait(mutex);
//			int i;
//			for(i=0;i<1000000;i++)
//				(*ptr)++;
//			sem_post(mutex);
//			printf("ptr is %d\n",);
//			sleep(10);
		}
}




