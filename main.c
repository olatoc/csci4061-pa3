/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Oliver Latocki, Jonathan Sulisz
* x500: latoc004, sulis005 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

int main(int argc, char *argv[]){

    //argument checking; only allow >3 arguments; exit if invalid
    if (argc < 3){
        fprintf(stderr,"incorrect argument number\n");
        exit(1);
    }
    if (argc == 4){
        if (strncmp(argv[3], "-p", 2) == 0){
            log_flag = 1; //flag to print log
        } else{
            fprintf(stderr,"invalid option\n");
            exit(1);
        }
    }
    else{
        log_flag = 0;
    }
    char *filename = argv[2];
    int consumer_count = atoi(argv[1]);

    init_queue();

    //initialize final output array to send to histogram
    word_stats = (int**)malloc(sizeof(int*)*consumer_count);
    for (int i = 0; i < consumer_count; i++){
        word_stats[i] = (int*)malloc(sizeof(int)*26);
    }
    total_word_stats = (int*)malloc(sizeof(int)*26);

    //initialize condition variable and mutex lock
    cq = (condBuffer*) malloc(sizeof(condBuffer));
	cq->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
	cq->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	pthread_cond_init(cq->cond, NULL);
	pthread_mutex_init(cq->mutex, NULL);

    if (log_flag){
        logfp = fopen("log.txt", "w");
        fclose(logfp);
    }
    //intialize the producer thread
    init_producer(filename);

    //initialize consumer threads
    init_consumers(consumer_count);

    //join producer
    pthread_join(p_thread_id, NULL);
    //join all consumers
    for (int i = 0; i < consumer_count; i++){
        //printf("JOINING CONSUMER %d\n", i);
        pthread_join(c_thread_id[i], NULL);
    }

    //free malloc'd pointers
    for (int i = 0; i < consumer_count; i++){
        free(c_thread_args[i]);
    }
    free(c_thread_args);
    free(q->head);

    free(cq->cond);
    free(cq->mutex);
    free(cq);
    
    //write final histogram to output file
    for (int i = 0; i < consumer_count; i++){
        for (int j = 0; j < 26; j++){
            total_word_stats[j] += word_stats[i][j];
        }
    }
    write_final_result(total_word_stats);
}
