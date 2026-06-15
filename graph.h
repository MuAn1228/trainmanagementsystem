#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "train.h"

#define MAX_VERTEX 200

typedef struct EdgeNode {
    char train_id[MAX_TRAIN_ID];
    int target;
    int weight;
    struct EdgeNode* next;
} EdgeNode;

typedef struct {
    char station_name[MAX_STATION_NAME];
    EdgeNode* first_edge;
} Vertex;

typedef struct {
    Vertex vertices[MAX_VERTEX];
    int vertex_count;
} Graph;

typedef struct {
    int path[MAX_VERTEX];
    int path_len;
    int total_weight;
    char train_on_seg[MAX_VERTEX][MAX_TRAIN_ID];
    bool found;
} RouteResult;

int build_graph(Graph* g, Train trains[], int train_count);
RouteResult dijkstra(Graph* g, int start, int end, int opt_type);
int find_station_index(Graph* g, const char* station_name);
#endif
