/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Oliver Latocki, Jonathan Sulisz
* x500: latoc004, sulis005 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

void init_consumers(int consumer_count){
    //allocate thread ID array and thread argument array
    c_thread_id = (pthread_t*)malloc(sizeof(pthread_t)*consumer_count);
    c_thread_args = (c_args_t**)malloc(sizeof(c_args_t*)*consumer_count);
    for (int i = 0; i < consumer_count; i++){
        c_thread_args[i] = (c_args_t*)malloc(sizeof(c_args_t) + 1);
    }
    //fill consumer arg struct with thread ID (0 to consumer# -1)
    for (int i = 0; i < consumer_count; i++){
        c_thread_args[i]->id = i;
    }
    //create consumer threads
    for (int i = 0; i < consumer_count; i++){
        pthread_create(&c_thread_id[i], NULL, consume, (void*)c_thread_args[i]);
    }
}

//the function is called by each consumer thread
//extract line from queue and perform word statistics on it in thread-safe way
void *consume(void *arg_ptr){
    c_args_t *args = arg_ptr;
    //print consumer ID upon its creation to log.txt
    pthread_mutex_lock((cq->mutex));
    if (log_flag){
        logfp = fopen("log.txt", "a");
        fprintf(logfp, "Consumer %d\n", args->id);
        fclose(logfp);
    }
    pthread_mutex_unlock((cq->mutex));
    //repeatedly check if queue is empty and extract nodes in thread-safe way
    while (1){
        pthread_mutex_lock((cq->mutex));
        while (q->empty){
            if (eof){ // check for EOF, exit safely
                //print consumer ID and line number it is currently working ok to log.txt
                if (log_flag){
                    logfp = fopen("log.txt", "a");
                    fprintf(logfp, "Consumer %d: %d\n", args->id, -1);
                    fclose(logfp);
                }
                pthread_mutex_unlock((cq->mutex));
                pthread_exit(NULL);
                break;
            }else{
                pthread_cond_wait(cq->cond, cq->mutex);
                continue;
            }
            break;
        }
        node *temp_node = q_extract(); //CRITICAL SECTION; extract from queue
        char *line = temp_node->line;
        if (log_flag){
            logfp = fopen("log.txt", "a");
            fprintf(logfp, "Consumer %d: %d\n", args->id, temp_node->line_num);
            fclose(logfp);
        }
        //print consumer ID and line number it is currently working ok to log.txt
        stats(line, args->id); //perform word stats on line
        pthread_mutex_unlock((cq->mutex));
        usleep(rand() % 100);
    }

}

//count number of occurances of words which start with each letter of the alphabet
//treat non-alphabetical characters as spaces
void stats(char *line, int id){
    char c;
    int i = 0;
    //turn non-alphabet characters to spaces
    while ((c = line[i]) != '\0'){
        if (!((c >= 65 && c <= 90) || (c >= 97 && c <= 122))){
			line[i] = ' ';
		}
        i += 1;
    }
    //do stats
    char* token;
    while ((token = __strtok_r(line, " ", &line))){
        c = token[0];
		if (c >= 65 && c <= 90){
			/* handle capital letters */
			word_stats[id][c - 65] += 1;
            
		} else if(c >= 97 && c <= 122){
			/* handle lowercase letters */
			word_stats[id][c - 97] += 1;
		}
	}
}