#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "train.h"
#include "hash.h"
#include "order.h"
extern HashTable train_hash;
extern Train trains[];
extern int train_count;

Order* order_head = NULL;
static int serial_counter = 0;

void sync_serial_counter(void){
    int max_serial = 0;
    for(Order* p = order_head; p != NULL; p = p->next){
        int len = strlen(p->order_id);
        if(len >= 4){
            int serial = atoi(p->order_id + len - 4);
            if(serial > max_serial) max_serial = serial;
        }
    }
    serial_counter = max_serial;
}

char* create_order(const char* train_id, int seat_type,
                     const char* passenger_name, const char* id_card)
{
    if(train_id == NULL || train_id[0] == '\0'){
        printf("您输入的车次号不合法\n");
        return NULL;
    }
    if(seat_type < 0 || seat_type > 3){
        printf("您输入的席位不合法\n");
        return NULL;
    }
    if(passenger_name == NULL || passenger_name[0] == '\0'){
        printf("您输入的姓名不合法\n");
        return NULL;
    }
    if(strlen(id_card)!=18){
        printf("输入的身份证号不合法\n");
        return NULL;
    }

    int index = hash_lookup(&train_hash, train_id);
    if(index == -1){
        printf("车次不存在\n");
        return NULL;
    }
    Train* train = &trains[index];

    if(train->seats[seat_type].remaining == 0){
        printf("该席别已售完\n");
        printf("是否加入候补？(1=是 0=否): ");
        int choice;
        scanf("%d", &choice);
        if(choice == 1){
            enqueue(&train->seats[seat_type].waitlist, passenger_name, id_card);
            printf("已加入候补队列，当前排位 %d\n", train->seats[seat_type].waitlist.count);
        }
        return NULL;
    }

    int seat_no = -1;
    for(int i = 0; i < MAX_SEATS_PER_TYPE; i++){
        if(train->seats[seat_type].status[i] == false){
            train->seats[seat_type].status[i] = true;
            train->seats[seat_type].remaining--;
            seat_no = i;
            break;
        }
    }
    if(seat_no == -1){
        printf("座位分配失败\n");
        return NULL;
    }

    serial_counter++;

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char date_str[9];
    snprintf(date_str, sizeof(date_str), "%04d%02d%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

    int tid_len = strlen(train_id);
    const char* last4 = train_id + (tid_len > 4 ? tid_len - 4 : 0);

    char order_id[ORDER_ID];
    snprintf(order_id, sizeof(order_id), "%s%s%04d",
             date_str, last4, serial_counter);

    float price = train->seats[seat_type].price;
    Order* node = (Order*)malloc(sizeof(Order));
    if(node == NULL){
        printf("系统内存不足\n");
        return NULL;
    }
    strncpy(node->order_id, order_id, ORDER_ID - 1);
    node->order_id[ORDER_ID - 1] = '\0';
    strncpy(node->train_id, train_id, MAX_TRAIN_ID - 1);
    node->train_id[MAX_TRAIN_ID - 1] = '\0';
    strncpy(node->passenger_name, passenger_name, PASSENGER_NAME - 1);
    node->passenger_name[PASSENGER_NAME - 1] = '\0';
    strncpy(node->id_card, id_card, ID_CARD - 1);
    node->id_card[ID_CARD - 1] = '\0';
    node->seat_type = seat_type;
    node->seat_no = seat_no;
    node->price = price;
    node->status = 1; 
    
    char time_buf[ORDER_TIME];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M", t);
    strncpy(node->order_time, time_buf, ORDER_TIME - 1);
    node->order_time[ORDER_TIME - 1] = '\0';

    node->next = order_head;
    order_head = node;
    return node->order_id;
}

int load_orders(const char* filename){
    FILE* fp = fopen(filename,"r");
    if(fp == NULL){
        return 0;   
    }
    char line[1024];
    int count = 0;

    while(fgets(line, sizeof(line), fp) && count < MAX_ORDERS){
        line[strcspn(line,"\n")] = '\0';

        Order* node = (Order*)malloc(sizeof(Order));
        if(node == NULL) break;

        int parsed = sscanf(line,"%s %s %s %s %d %d %f %d %s",
                            node->order_id,
                            node->train_id,
                            node->passenger_name,
                            node->id_card,
                            &node->seat_type,
                            &node->seat_no,
                            &node->price,
                            &node->status,
                            node->order_time);
        if(parsed != 9){
            free(node);       
            continue;
        }

        node->next = order_head;
        order_head = node;
        count++;
    }
    fclose(fp);
    return count;
}

int save_orders(const char* filename){
    FILE* fp = fopen(filename, "w");
    if(fp == NULL) return -1;
    for(Order* p = order_head; p != NULL; p = p->next){
        fprintf(fp, "%s\t%s\t%s\t%s\t%d\t%d\t%f\t%d\t%s",
            p->order_id,
            p->train_id,
            p->passenger_name,
            p->id_card,
            p->seat_type,
            p->seat_no,
            p->price,
            p->status,
            p->order_time);
        fprintf(fp,"\n");
    }
    fclose(fp);
    return 0;
}

Order* find_order_by_id(const char *id){
    for(Order* p = order_head; p != NULL; p = p->next){
        if(strcmp(p->order_id, id) == 0){
            return p;
        }
    }
    return NULL;
}

float cancel_order(const char* order_id){
    Order* order = find_order_by_id(order_id);
    if(order == NULL){
        printf("查找失败");
        return -1;
    }else if(order->status != 1){
        printf("订单已退,无法继续退票");
        return -1;
    }
    int index = hash_lookup(&train_hash, order->train_id);
    if(index == -1){
        printf("车次不存在\n");
        return -1;
    }else{
          Train *train = &trains[index];
          train->seats[order->seat_type].status[order->seat_no] = false;  
          train->seats[order->seat_type].remaining++;
          float tuikuan = train->seats[order->seat_type].price*0.8;
          order->status = 2;
          if(train->seats[order->seat_type].waitlist.count > 0){
            WaitNode* w = dequeue(&train->seats[order->seat_type].waitlist);
            if(w != NULL){
                char* new_id = create_order(order->train_id, order->seat_type,
                                            w->passenger_name, w->id_card);
                if(new_id != NULL){
                    printf("候补兑付成功！订单号：%s\n", new_id);
                }
                free(w);  // WaitNode 出队后释放
            }
        }
          return tuikuan;
    }
}

char* change_order(const char *old_order_id,
                    const char *new_train_id, int new_seat_type){
    Order* order = find_order_by_id(old_order_id);
    if(order == NULL){
        printf("查找失败\n");
        return NULL;
    }
    if(order->status != 1){
        printf("订单状态异常，无法改签\n");
        return NULL;
    }
    int index = hash_lookup(&train_hash, order->train_id);
    if(index == -1){
        printf("车次不存在\n");
        return NULL;
    }
    Train *train = &trains[index];
    train->seats[order->seat_type].status[order->seat_no] = false;
    train->seats[order->seat_type].remaining++;
    order->status = 2;
    if(train->seats[order->seat_type].waitlist.count > 0){
        WaitNode* w = dequeue(&train->seats[order->seat_type].waitlist);
        if(w != NULL){
            char* new_id = create_order(order->train_id, order->seat_type,
                                        w->passenger_name, w->id_card);
            if(new_id != NULL){
                printf("候补兑付成功！订单号：%s\n", new_id);
            }
            free(w);  // WaitNode 出队后释放
        }
    }

    char *new_id = create_order(new_train_id, new_seat_type,
                     order->passenger_name, order->id_card);
    if(new_id == NULL){
        train->seats[order->seat_type].status[order->seat_no] = true;
        train->seats[order->seat_type].remaining--;
        order->status = 1;
        return NULL;
    }

    return new_id;
}