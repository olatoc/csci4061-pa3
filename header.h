/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Oliver Latocki, Jonathan Sulisz
* x500: latoc004, sulis005 */

/*
header.h, header for all source files
it will: 
- structure definition
- global variable, lock declaration (extern)
- function declarations
*/

#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
// header here..


//queue node
typedef struct node {
    struct node *next;
    char *line; //line buffer
    int line_num;
} node;

//queue structure, containing pointers to head and tail, contains empty flag
typedef struct queue {
    struct node *head;
    struct node *tail;
    int empty;
} queue;

//buffer containing condition variable pointer and mutex pointer
typedef struct condBuffer {
	pthread_cond_t* cond;
	pthread_mutex_t* mutex;
} condBuffer;

//globals
condBuffer* cq;

queue *q;

int eof;

int log_flag;

int **word_stats;
int *total_word_stats;

//queue functions
void init_queue();
void q_insert(char *line, int line_num);
node *q_extract();

//producer arg struct
typedef struct P_Thread_Args {
    char filename[64]; //path to testing file
} p_args_t;

//consumer arg struct
typedef struct C_Thread_Args {
    int id; //consumer ID
} c_args_t;

//producer thread ID; consumer thread IDs; consumer thread args
pthread_t p_thread_id;
pthread_t *c_thread_id;
c_args_t **c_thread_args;

//log.txt file pointer
FILE *logfp;

//producer and consumer function prototypes
void init_producer(char *filename);
void *produce(void *filename);
void read_file(char *filename);

void init_consumers(int consumer_count);
void *consume(void *id);
void stats(char *line, int id);

//write to result.txt
void write_final_result(int *word_stats);
#endif
