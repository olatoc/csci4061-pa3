/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Oliver Latocki, Jonathan Sulisz
* x500: latoc004, sulis005 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

//initialize producer process
void init_producer(char *filename){
    p_args_t *p_thread_args = (p_args_t *)malloc(sizeof(p_args_t));

    //send test filename to producer
    strcpy(p_thread_args->filename, filename);
    pthread_create(&p_thread_id, NULL, &produce, (void*)p_thread_args);
}

//do producer functionality
void *produce(void *filename){
    pthread_mutex_lock((cq->mutex));
    if (log_flag){
        logfp = fopen("log.txt", "a");
        fprintf(logfp, "Producer\n");
        fclose(logfp);
    }
    pthread_mutex_unlock((cq->mutex));
    read_file(filename);
    return NULL;
}

//read contents of file into shared queue
void read_file(char *filename){
    eof = 0;
	FILE *fp;
    char *line;
    size_t len = 0;
    fp = fopen(filename, "r");
    if (fp == NULL){
        exit(1);
	}
    //keep track of line number
    int counter = 0;
    //insert contents of file line by line into shared queue synchronously
    while (1) {
        pthread_mutex_lock((cq->mutex));
        if ((getline(&line, &len, fp)) == -1){
            eof = 1;
            if (log_flag){
                logfp = fopen("log.txt", "a");
                fprintf(logfp, "Producer: %d\n", -1);
                fclose(logfp);
            }
            pthread_cond_broadcast((cq->cond));
            pthread_mutex_unlock((cq->mutex));
            fclose(fp);
            pthread_exit(NULL);
            break;
        } else{
            //write producer line number to log file
            if (log_flag){
                logfp = fopen("log.txt", "a");
                fprintf(logfp, "Producer: %d\n", counter);
                fclose(logfp);
            }
            q_insert(line, counter); //CRITICAL SECTION
            counter++;
            pthread_cond_signal((cq->cond));
            pthread_mutex_unlock((cq->mutex));
        }
        usleep(rand() % 100);
    }
}