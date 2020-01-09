/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Oliver Latocki, Jonathan Sulisz
* x500: latoc004, sulis005 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

// implement shared queue, final histogram here.. 

//This file contains queue functions, as well as the function to produce the final result

//initialize queue, creating dummy head node and initializing values
void init_queue(){
    q = (queue *)malloc(sizeof(queue));
    q->head = (node *)malloc(sizeof(node));
    q->tail = q->head;
    q->head->next = NULL;
    q->empty = 1;
}

//insert a line buffer into the queue at the tail, set the empty flag to false
void q_insert(char *line, int line_num){
    node *new = (node*)malloc(sizeof(node));
    q->tail->next = new;
    q->tail = new;
    new->line = (char*)malloc(sizeof(char)*strlen(line) + 2);
    new->line_num = line_num;
    strcpy(new->line, line);
    new->next = NULL;
    q->empty = 0;
}

//extract a node from the queue, set empty flag if queue is empty
node *q_extract(){
    node *ret = q->head->next;
    node *temp_head = q->head;
    q->head = q->head->next;
    if (q->head->next == NULL){
        q->empty = 1;
    }
    free(temp_head->line);
    free(temp_head);
    return ret;
}

//write output file "result.txt" containing the final histograms
void write_final_result(int *word_stats){
	FILE *fp;
	char filename[128];
	int k = 0;
	sprintf(filename, "result.txt");
	fp = fopen(filename, "w+");
	for (int i = 0; i < 26; i++){
		fprintf(fp, "%c: %d\n", 97 + i, word_stats[i]);
	}
	fclose(fp);
}