#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"
#include "query.h"
#include "hash.h"
#include "sort.h"
#include "order.h"
#include "graph.h"
Train trains[MAX_TRAINS];
int train_count;
HashTable train_hash;
extern Order* order_head;

void search_trains();
void book_ticket();
void cancel_ticket();
void change_ticket();
void query_orders();
void waitlist_status();

/*车次查询子结构*/
void search_by_train_number();
void advanced_sort();
void plan_optimal_route();



int main(){
    system("chcp 65001 > nul");
    setvbuf(stdin, NULL, _IONBF, 0);
    train_count = load_trains("trains.txt", trains, MAX_TRAINS);
    build_hash_table(&train_hash, trains, train_count);
    load_orders("orders.txt");
    sync_serial_counter();

    int choice;
    do
    {
        printf("\n========== 列车订票系统 ==========\n");
        printf("1. 车次查询\n");
        printf("2. 车票预订\n");
        printf("3. 退票\n");
        printf("4. 改签\n");
        printf("5. 订单查询\n");
        printf("6. 候补队列查询\n");
        printf("0. 退出系统\n");
        printf("请选择： \n");

        scanf("%d",&choice);

        switch(choice)
        {
        case 1:
            search_trains();
            break;

        case 2:
            book_ticket();
            break;

        case 3:
            cancel_ticket();
            break;

        case 4:
            change_ticket();
            break;

        case 5:
            query_orders();
            break;

        case 6:
            waitlist_status();
            break;

        case 0:
            save_trains("trains.txt",trains,train_count);
            save_orders("orders.txt");
            printf("系统已退出。\n");
            break;

        default:
            printf("输入错误！\n");
        }
    }while(choice != 0);

    return 0;
}

void search_trains(){
    int choice;
    do{
        printf("1 按出发站 + 到达站查询\n");
        printf("2 按车次号精确查询\n");
        printf("3 多维度排序\n");
        printf("4 最优路径规划\n");
        printf("0. 返回主菜单\n");

        scanf("%d",&choice);
        switch(choice){
            case 1:
                printf("读取用户输入的出发站和到达站\n");
                char from[32];
                char to[32];
                printf("请输入出发站\n");
                scanf("%31s",from);
                printf("请输入到达站\n");
                scanf("%31s",to);
                search_by_stations(trains, train_count, from, to);
                break;
            case 2:
                search_by_train_number();
                break;
            case 3:
                advanced_sort();
                break;
            case 4:
                plan_optimal_route();
                break;
            case 0:
                break;
            default:
                printf("输入错误！\n");
        }
    }while(choice != 0);
}

/*函数定义*/
void search_by_train_number(){
    char train_id[16];
    printf("请输入车次号：");
    scanf("%15s", train_id);
    int bucket = hash(train_id, HASH_TABLE_SIZE);
    HashNode* p = train_hash.buckets[bucket];
    printf("%-16s %-32s %-32s %-20s %-20s %-16s %-16s\n",
                "车次", "出发站", "到达站", "发车时间", "到达时间", "耗时", "余票");
    while(p != NULL){
        if(strcmp(p->train_id, train_id) == 0){
            printf("%-16s %-32s %-32s %-20s %-20s %-16d %-16d\n",
                p->train_id,
                trains[p->index].departure,
                trains[p->index].destination,
                trains[p->index].depart_time,
                trains[p->index].arrive_time,
                trains[p->index].duration,
                trains[p->index].total_seats);
            return;
        }
        p = p->next;
    }
    printf("未找到该车次\n");
}

void advanced_sort(){
    printf("请选择排序算法\n"
           "1.快速排序\n"
           "2.归并排序\n");
    int choice;
    scanf("%d", &choice);
    switch(choice){
        case 1:
            printf("请选择排序维度\n"
                   "1.发车时间\n"
                   "2.全程耗时\n"
                   "3.票价\n"
                   "4.到达时间\n");
            int dimension;
            scanf("%d", &dimension);
            if(dimension >= 1 && dimension <= 4){
                quicksort(trains, 0, train_count - 1, dimension);
                printf("%-16s %-32s %-32s %-20s %-20s %-16s %-16s\n",
                            "车次", "出发站", "到达站", "发车时间", "到达时间", "耗时", "余票");
                for(int i = 0; i < train_count; i++){
                    printf("%-16s %-32s %-32s %-20s %-20s %-16d %-16d\n",
                        trains[i].train_id,
                        trains[i].departure,
                        trains[i].destination,
                        trains[i].depart_time,
                        trains[i].arrive_time,
                        trains[i].duration,
                        trains[i].total_seats);
                }
            }else{
                printf("输入错误！");
            }
            break;
        case 2:
            printf("请选择排序维度\n"
                   "1.发车时间\n"
                   "2.全程耗时\n"
                   "3.票价\n"
                   "4.到达时间\n");
            scanf("%d", &dimension);
            if(dimension >= 1 && dimension <= 4){
                mergesort(trains, 0, train_count - 1, dimension);
                printf("%-16s %-32s %-32s %-20s %-20s %-16s %-16s\n",
                            "车次", "出发站", "到达站", "发车时间", "到达时间", "耗时", "余票");
                for(int i = 0; i < train_count; i++){
                    printf("%-16s %-32s %-32s %-20s %-20s %-16d %-16d\n",
                        trains[i].train_id,
                        trains[i].departure,
                        trains[i].destination,
                        trains[i].depart_time,
                        trains[i].arrive_time,
                        trains[i].duration,
                        trains[i].total_seats);
                }
            }else{
                printf("输入错误");
            }
            break;
            default:
                printf("请检查您的输入");
        }
}








void plan_optimal_route(){
    char from[32], to[32];
    printf("请输入出发站：");
    scanf("%31s", from);
    printf("请输入到达站：");
    scanf("%31s", to);

    Graph g;
    build_graph(&g, trains, train_count);

    int start = find_station_index(&g, from);
    int end = find_station_index(&g, to);
    if(start < 0 || end < 0){
        printf("站点不存在\n");
        return;
    }

    RouteResult r = dijkstra(&g, start, end, 1);
    if(!r.found){
        printf("未找到可达路径\n");
        return;
    }

    printf("\n========== 最优路径规划 ==========\n");
    printf("总耗时: %d 分钟\n", r.total_weight);
    printf("路径: ");
    for(int i = 0; i < r.path_len; i++){
        printf("%s", g.vertices[r.path[i]].station_name);
        if(i < r.path_len - 1)
            printf(" --(%s)--> ", r.train_on_seg[i]);
    }
    printf("\n共 %d 站，%d 次换乘\n", r.path_len, r.path_len - 2);
}

void book_ticket(){
    char train_id[16];
    int seat_type;
    char passenger_name[32];
    char id_card[19];

    printf("请输入车次号：");
    scanf("%15s", train_id);
    printf("请选择席别 (0=硬座 1=软座 2=硬卧 3=软卧):");
    scanf("%d", &seat_type);
    printf("请输入乘客姓名：");
    scanf("%31s", passenger_name);
    printf("请输入身份证号(18位):");
    scanf("%18s", id_card);

    char* result = create_order(train_id, seat_type, passenger_name, id_card);
    if(result != NULL){
        printf("订票成功！订单号：%s\n", result);
    }else{
        printf("订票失败，请检查输入信息。\n");
    }
}
void cancel_ticket(){
    printf("请输入您要取消的车次");
    char id[25];
    scanf("%s",id);
    float tui = cancel_order(id);
    printf("%f元已原路退回",tui);
}
void change_ticket(){
    char old_order_id[25];
    char new_train_id[16];
    int new_seat_type;

    printf("请输入原订单号：");
    scanf("%24s", old_order_id);
    printf("请输入新车次号：");
    scanf("%15s", new_train_id);
    printf("请选择新席别 (0=硬座 1=软座 2=硬卧 3=软卧)：");
    scanf("%d", &new_seat_type);

    char* result = change_order(old_order_id, new_train_id, new_seat_type);
    if(result != NULL){
        printf("改签成功！新订单号：%s\n", result);
    }else{
        printf("改签失败，请检查输入信息。\n");
    }
}
void query_orders(){
    printf("%-20s %-10s %-10s %-20s %-6s %-6s %-8s %-6s %-20s\n",
      "订单号", "车次", "姓名", "身份证", "席别", "座位", "票价", "状态", "下单时间");
    if(order_head == NULL){
      printf("暂无订单\n");
      return;
    }
    for(Order* p = order_head; p != NULL; p = p->next){
        printf("%s\t%s\t%s\t%s\t%d\t%d\t%f\t%d\t%s",
            p->order_id,
            p->train_id,
            p->passenger_name,
            p->id_card,
            p->seat_type,
            p->seat_no,
            p->price,
            p->status,
            p->order_time);
        printf("\n");
    }
}
void waitlist_status(){
    char train_id[16];
    int seat_type;
    printf("请输入车次号：");
    scanf("%15s", train_id);
    printf("请输入席别 (0=硬座 1=软座 2=硬卧 3=软卧)：");
    scanf("%d", &seat_type);
    if(seat_type < 0 || seat_type > 3){
        printf("席别不合法\n");
        return;
    }
    int index = hash_lookup(&train_hash, train_id);
    if(index == -1){
        printf("车次不存在\n");
        return;
    }
    show_queue(&trains[index].seats[seat_type].waitlist);
}
