#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "train.h"
#include "sort.h"

int compare(Train* a, Train* b, int sort_key){
      switch(sort_key){
          case SORT_BY_DEPART_TIME:
              return strcmp(a->depart_time, b->depart_time);
          case SORT_BY_DURATION:
              return a->duration - b->duration;
          case SORT_BY_PRICE:
              if(a->seats[0].price > b->seats[0].price){
                return 1;
              }else if(a->seats[0].price == b->seats[0].price){
                return 0;
              }else{
                return -1;
              }
          case SORT_BY_ARRIVE_TIME:
              return strcmp(a->arrive_time, b->arrive_time);
          default:
          return 0;
      }
    }




int partition(Train arr[], int left, int right, int sort_key){
    int i = left - 1;
    for(int j = left; j<right; j++){
        if(compare(&arr[j],&arr[right],sort_key) <= 0){
            i++;
            Train temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    Train temp = arr[i+1];
            arr[i+1] = arr[right];
            arr[right] = temp;
    return i + 1;
}
    
void quicksort(Train arr[], int left, int right, int sort_key){
    if(left >= right) return;
    int pos = partition(arr, left, right, sort_key);
    quicksort(arr, left, pos - 1, sort_key);
    quicksort(arr, pos + 1, right, sort_key);
}




void merge(Train arr[], int left, int mid, int right, int sort_key){
    Train temp[right - left +1];
    int i = left;
    int j = mid + 1;
    int k = 0;
    while(i <= mid && j <= right){
        if(compare(&arr[i],&arr[j],sort_key) <= 0){
            temp[k] = arr[i];
            i++;
            k++;
        }else{
            temp[k] = arr[j];
            j++;
            k++;
        }
    }
    while(i <= mid) temp[k++] = arr[i++];
    while(j <= right) temp[k++] = arr[j++];
    for(int m = 0; m < right - left + 1; m++){
        arr[left + m] = temp[m];
    }
}

void mergesort(Train arr[], int left, int right, int sort_key){
    if(left >= right) return;
    int mid = (left + right)/2;
    mergesort(arr, left, mid, sort_key);
    mergesort(arr, mid + 1, right, sort_key);
    merge(arr, left, mid, right, sort_key);
}