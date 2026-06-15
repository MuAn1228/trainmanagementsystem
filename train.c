#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"
#include "query.h"

int load_trains(const char* filename, Train trains[], int max_count){
    FILE* fp = fopen(filename,"r");
    if(fp == NULL){
        printf("文件打开失败\n");
        return -1;
    }
    const char* seat_names[SEAT_TYPE_COUNT] = {
        "Hard Seat",
        "Soft Seat",
        "Hard Sleeper",
        "Soft Sleeper"
    };
    char line[1024];
    int count = 0;
    
    while(count < max_count && fgets(line,sizeof(line),fp)){
        line[strcspn(line,"\n")] = '\0';
        Train *train = &trains[count];
        int parsed = sscanf(line,"%s %s %s %s %s %d %d",
                            train->train_id,
                            train->departure,
                            train->destination,
                            train->depart_time,
                            train->arrive_time,
                            &train->duration,
                            &train->total_seats);
        if(parsed != 7){
            continue;
        }
        char buffer[1024];
        strcpy(buffer,line);
        char* token = strtok(buffer," \t");
        int field_index = 0;
        while(token && field_index < 7){
            token = strtok(NULL," \t");
            field_index++;
        }

        int seat_index;
        for(seat_index = 0; seat_index < SEAT_TYPE_COUNT; seat_index++){
            if(token == NULL) break;
            float price = (float)atof(token);
            token = strtok(NULL," \t");
            if(token == NULL) break;

            int total_seat_type = atoi(token);
            token = strtok(NULL," \t");
            SeatType* st = &train->seats[seat_index];
            st->waitlist.front = NULL;
            st->waitlist.rear = NULL;
            st->waitlist.count = 0;
            strncpy(st->Seat_Name, seat_names[seat_index], MAX_SEAT_NAME-1);
            st->Seat_Name[MAX_SEAT_NAME-1] = '\0';
            st->price = price;
            st->remaining = total_seat_type;

              for(int j = 0; j < MAX_SEATS_PER_TYPE; j++) {
                st->status[j] = false;
              }
        }
        count++;
    }
    fclose(fp);
    return count;
}



int save_trains(const char* filename, Train trains[], int count){
    FILE* fp = fopen(filename, "w");
    if(fp == NULL) return -1;
    for(int i = 0; i < count; i++){
        fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%d\t%d",
            trains[i].train_id,
            trains[i].departure,
            trains[i].destination,
            trains[i].depart_time,
            trains[i].arrive_time,
            trains[i].duration,
            trains[i].total_seats);
            for(int j = 0; j < SEAT_TYPE_COUNT; j++){
            fprintf(fp, "\t%.1f\t%d",
                trains[i].seats[j].price,
                trains[i].seats[j].remaining);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    return 0;
}



