#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include <time.h>     
#include "queue.h"

void enqueue(Queue* q, const char* name, const char* id_card){
    WaitNode* n = malloc(sizeof(WaitNode));
    if(n == NULL) return;
    n->next = NULL;
    if(q->front == NULL){
        q->front = n;
        q->rear = n;
    }else{
        q->rear->next = n;
        q->rear = n;
    }
    strncpy(n->passenger_name, name, 15);
    n->passenger_name[15] = '\0';
    strncpy(n->id_card, id_card, 18);
    n->id_card[18] = '\0';
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(n->register_time, sizeof(n->register_time), "%Y-%m-%d %H:%M", t);
    q->count++;
}

WaitNode* dequeue(Queue* q){
    if(q->front == NULL || q->count == 0){
        return NULL;
    }
    WaitNode* temp = q->front;
    q->front = q->front->next;
    if(q->front == NULL){
        q->rear = NULL;
    }
    q->count--;
    return temp;
}

void show_queue(Queue* q){
    if(q->front == NULL){
          printf("当前无人候补\n");
          return;
      }
    printf("排位\t姓名\t\t身份证\t\t登记时间\n");
      int pos = 1;
      for(WaitNode* p = q->front; p != NULL; p = p->next){
          printf("%d\t%s\t%s\t%s\n", pos, p->passenger_name, p->id_card, p->register_time);
          pos++;
      }
      printf("当前 %d 人候补\n", q->count);
  }

void free_queue(Queue* q){
    WaitNode* p = q->front;
    while(p != NULL){
        WaitNode* next = p->next;
        free(p);
        p = next;
    }
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}