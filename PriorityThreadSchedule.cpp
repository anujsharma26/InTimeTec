#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct thread_node {
    char name[10];
    int priority;
    struct thread_node *next;
};
typedef struct thread_node thread_node;

struct priority_queue {
    thread_node *front, *rear;
};
typedef struct priority_queue priority_queue;

//Get new queue
priority_queue* create_queue() {
    priority_queue *queue = malloc(sizeof(priority_queue));
    queue->front = NULL;
    queue->rear = NULL;

    return queue;
}

thread_node* create_node(char *thread_name, int thread_priority) {
    thread_node *thread;
    thread = (thread_node *) malloc(sizeof(thread_node));
    thread->priority = thread_priority;
    strcpy (thread->name, thread_name);
    thread->next = NULL;

    return thread;
}

void enqueue(priority_queue *queue, thread_node *thread) {
    priority_queue * arg_queue = queue;
    thread_node * arg_node = thread;
    thread_node * prev_node = NULL;
    if (arg_queue -> front == NULL) {
        arg_queue -> front = arg_node;
        arg_queue -> rear = arg_node;
        return;
    }
    thread_node * curr_node = arg_queue -> front;
    while (curr_node && curr_node -> priority <= arg_node -> priority) {
        prev_node = curr_node;
        curr_node = curr_node -> next;
    }

    if (prev_node != NULL){
        arg_node -> next = prev_node -> next;
        prev_node -> next = arg_node;
        return;
    }
    arg_node -> next = arg_queue -> front;
    arg_queue -> front = arg_node;
}


/*=======DO NOT MODIFY THE CODE BELOW =======*/

thread_node* dequeue(priority_queue *queue) {
    priority_queue * arg_queue = queue;
    thread_node * outgoing_node = arg_queue -> front;
    if (outgoing_node == NULL){
        return NULL;
    }
    arg_queue -> front = outgoing_node -> next;
    return outgoing_node;
}



void display(thread_node *thread) {
    printf("%s %d", thread->name, thread->priority);
    printf("\n");
}

int main(int argc, char * argv[])
{
    int thread_count, thread_priority;
    char thread_name[10];

    priority_queue *queue = create_queue();

    scanf("%d", &thread_count);

    thread_node *thread[thread_count];

    //Input the Thread priority table and put it in queue.
    for (int index = 0; index < thread_count; index++) {
        scanf("%s %d",thread_name,&thread_priority);

        thread[index] = create_node(thread_name, thread_priority);

        enqueue(queue, thread[index]);
    }

    for (int index = 0; index < thread_count; index++) {
        //dequeue will return threads
        display(dequeue(queue));
    }
}