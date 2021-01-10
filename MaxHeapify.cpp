 Implementation of Max Heapify

#include <stdio.h>
#include <stdlib.h>
#define MAX 20

void max_heapify(int array[], int index, int heapSize){
    int max, l_node, r_node;
    l_node = (2 * index + 1);
    r_node = (2 * index + 2);
    if (l_node >= heapSize)
        return;
    else {
        if (l_node < (heapSize) && array[l_node] > array[index])
            max = l_node;
        else
            max = index;
        if (r_node < (heapSize) && array[r_node] > array[max])
            max = r_node;
        if (max != index) {
            int temp = array[index];
            array[index] = array[max];
            array[max] = temp;
            max_heapify(array, max, heapSize);
        }
    }
}

int* build_max_heap(int array[], int arraySize){
    int j;
    for (j = arraySize/2; j >= 0; j--) {
        max_heapify(array, j, arraySize);
    }
    return array;
}

void display(int *max_heap, int arraySize){
    int counter;
    for (counter = 0; counter < arraySize; counter++) {
        printf("%d ", max_heap[counter]);
    }
}

int main(){
    int index, arraySize;
    int *array = calloc(MAX, sizeof(int));
    int *max_heap = calloc(MAX, sizeof(int));

    scanf("%d", &arraySize);

    for (index = 0; index < arraySize; index++) {
        scanf("%d", &array[index]);
    }
    max_heap = build_max_heap(array, arraySize);
    display(max_heap, arraySize);
}