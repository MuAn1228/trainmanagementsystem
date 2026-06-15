#ifndef QUERY_H
#define QUERY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"
int search_by_stations(Train trains[], int count, const char* from, const char* to);
#endif