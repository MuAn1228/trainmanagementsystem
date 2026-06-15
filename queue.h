#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
typedef struct WaitNode {
      char passenger_name[16];
      char id_card[19];
      char register_time[20];
      struct WaitNode* next;
  }WaitNode;
typedef struct Queue {
      struct WaitNode* front;
      struct WaitNode* rear;
      int count;
  }Queue;
void enqueue(Queue* q, const char* name, const char* id_card);
WaitNode* dequeue(Queue* q);
void show_queue(Queue* q);
void free_queue(Queue* q);

#endif