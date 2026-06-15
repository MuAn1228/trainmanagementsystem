#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"

int search_by_stations(Train trains[], int count, const char* from, const char* to){
    int matched = 0;
    printf("%-16s %-32s %-32s %-20s %-20s %-16s %-16s\n",
                "车次", "出发站", "到达站", "发车时间", "到达时间", "耗时", "余票");
    for(int i = 0; i < count; i++){
        if(strcmp(from,(trains[i]).departure)==0 && strcmp(to,(trains[i]).destination)==0){
            printf("%-16s %-32s %-32s %-20s %-20s %-16d %-16d\n",
                trains[i].train_id,
                trains[i].departure,
                trains[i].destination,
                trains[i].depart_time, 
                trains[i].arrive_time,
                trains[i].duration,
                trains[i].total_seats);
            matched++;
        }
        }
        if(matched == 0){
            printf("无符合条件的车次");
    }
    return matched;
}

