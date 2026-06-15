#ifndef ORDER_H
#define ORDER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"
#define ORDER_ID 20
#define PASSENGER_NAME 16
#define ID_CARD 19
#define ORDER_TIME 20
typedef struct Order {
    char order_id[ORDER_ID];        
    char train_id[MAX_TRAIN_ID];        
    char passenger_name[PASSENGER_NAME];  
    char id_card[ID_CARD];         
    int  seat_type;           
    int  seat_no;             
    float price;          
    int  status;              
    char order_time[ORDER_TIME];       
    struct Order* next;      
} Order;

char* create_order(const char* train_id, int seat_type,
                   const char* passenger_name, const char* id_card);
int load_orders(const char* filename);
int save_orders(const char* filename);

Order* find_order_by_id(const char *id);
float cancel_order(const char* order_id);
char* change_order(const char *old_order_id, const char *new_train_id, int new_seat_type);
void sync_serial_counter(void);

#endif