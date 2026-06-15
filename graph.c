#include "graph.h"

static int add_or_find_station(Graph* g, const char* name) {
    for(int i = 0; i < g->vertex_count; i++){
        if(strcmp(g->vertices[i].station_name, name) == 0)
            return i;
    }
    if(g->vertex_count >= MAX_VERTEX) return -1;
    strncpy(g->vertices[g->vertex_count].station_name, name, MAX_STATION_NAME - 1);
    g->vertices[g->vertex_count].station_name[MAX_STATION_NAME - 1] = '\0';
    g->vertices[g->vertex_count].first_edge = NULL;
    return g->vertex_count++;
}

static void add_edge(Graph* g, int from, int to, const char* train_id, int weight) {
    EdgeNode* e = (EdgeNode*)malloc(sizeof(EdgeNode));
    if(e == NULL) return;
    strncpy(e->train_id, train_id, MAX_TRAIN_ID - 1);
    e->train_id[MAX_TRAIN_ID - 1] = '\0';
    e->target = to;
    e->weight = weight;
    e->next = g->vertices[from].first_edge;
    g->vertices[from].first_edge = e;
}

int build_graph(Graph* g, Train trains[], int train_count) {
    g->vertex_count = 0;
    for(int i = 0; i < train_count; i++){
        int from = add_or_find_station(g, trains[i].departure);
        int to   = add_or_find_station(g, trains[i].destination);
        if(from < 0 || to < 0) return -1;
        add_edge(g, from, to, trains[i].train_id, trains[i].duration);
    }
    return g->vertex_count;
}

int find_station_index(Graph* g, const char* station_name) {
    for(int i = 0; i < g->vertex_count; i++){
        if(strcmp(g->vertices[i].station_name, station_name) == 0)
            return i;
    }
    return -1;
}

RouteResult dijkstra(Graph* g, int start, int end, int opt_type) {
    (void)opt_type; 
    RouteResult result;
    result.found = false;
    result.path_len = 0;
    result.total_weight = 0;
    memset(result.path, -1, sizeof(result.path));
    memset(result.train_on_seg, 0, sizeof(result.train_on_seg));

    int n = g->vertex_count;
    int dist[MAX_VERTEX];  
    int prev[MAX_VERTEX];    
    char prev_train[MAX_VERTEX][MAX_TRAIN_ID]; 
    bool visited[MAX_VERTEX];

    for(int i = 0; i < n; i++){
        dist[i] = INT_MAX;
        prev[i] = -1;
        prev_train[i][0] = '\0';
        visited[i] = false;
    }
    dist[start] = 0;

    for(int count = 0; count < n; count++){
        int u = -1;
        int min_dist = INT_MAX;
        for(int i = 0; i < n; i++){
            if(!visited[i] && dist[i] < min_dist){
                min_dist = dist[i];
                u = i;
            }
        }
        if(u == -1 || u == end) break;
        visited[u] = true;

        EdgeNode* e = g->vertices[u].first_edge;
        while(e != NULL){
            int v = e->target;
            int w = e->weight;
            if(!visited[v] && dist[u] != INT_MAX && dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                prev[v] = u;
                strncpy(prev_train[v], e->train_id, MAX_TRAIN_ID - 1);
                prev_train[v][MAX_TRAIN_ID - 1] = '\0';
            }
            e = e->next;
        }
    }

    if(dist[end] == INT_MAX) return result;


    result.found = true;
    result.total_weight = dist[end];
    int temp[MAX_VERTEX];
    char temp_train[MAX_VERTEX][MAX_TRAIN_ID];
    int len = 0;
    int cur = end;
    while(cur != -1){
        temp[len] = cur;
        if(cur != start)
            strncpy(temp_train[len], prev_train[cur], MAX_TRAIN_ID - 1);
        else
            temp_train[len][0] = '\0';
        len++;
        cur = prev[cur];
    }

    for(int i = 0; i < len; i++){
        result.path[i] = temp[len - 1 - i];
        if(i < len - 1)
            strncpy(result.train_on_seg[i], temp_train[len - 2 - i], MAX_TRAIN_ID - 1);
    }
    result.path_len = len;
    return result;
}
