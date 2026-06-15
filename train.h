#ifndef TRAIN_H      
#define TRAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#define MAX_TRAINS 500
#define MAX_STATION_NAME 32
#define MAX_TRAIN_ID 16
#define MAX_ORDERS 5000
#define MAX_SEATS_PER_TYPE 2000
#define SEAT_TYPE_COUNT 4
#define MAX_TIME_STR 20 
#define MAX_SEAT_NAME  16


typedef struct SeatType{
    char Seat_Name[MAX_SEAT_NAME];
    float price;
    int remaining;
    bool status[MAX_SEATS_PER_TYPE];
    Queue waitlist; 
}SeatType;

typedef struct Train{
    char train_id[MAX_TRAIN_ID];
    char departure[MAX_STATION_NAME];
    char destination[MAX_STATION_NAME];
    char depart_time[MAX_TIME_STR];
    char arrive_time[MAX_TIME_STR];
    int duration;
    int total_seats;
    SeatType seats[SEAT_TYPE_COUNT];
}Train;

int load_trains(const char* filename, Train trains[], int max_count);
int save_trains(const char* filename, Train trains[], int count);

#endif