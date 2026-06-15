#ifndef SORT_H
#define SORT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"
#define SORT_BY_DEPART_TIME  1
#define SORT_BY_DURATION     2
#define SORT_BY_PRICE        3
#define SORT_BY_ARRIVE_TIME  4

int compare(Train* a, Train*b, int sort_key);
void quicksort(Train arr[], int left, int right, int sort_key);
void mergesort(Train arr[], int left, int right, int sort_key);
void merge(Train arr[], int left, int mid, int right, int sort_key);
#endif