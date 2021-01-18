Implementation of Max Heapify

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 20

    void
    swap(int *first, int *second)
{
    int temp = *first;
    *first = *second;
    *second = temp;
}

int LeftIsGreater(int leftIndex, int heapSize, int array[], int max)
{

    if (leftIndex < (heapSize) && array[leftIndex] > array[max])
        return leftIndex;
    return max;
}

int RightIsGreater(int rightIndex, int heapSize, int array[], int max)
{
    if (rightIndex < (heapSize) && array[rightIndex] > array[max])
        return rightIndex;
    return max;
}

void max_heapify(int array[], int index, int heapSize)
{
    int max = 0, leftIndex, rightIndex;
    leftIndex = (2 * index + 1);
    rightIndex = (2 * index + 2);

    if (leftIndex >= heapSize)
        return;

    max = LeftIsGreater(leftIndex, heapSize, array, index);

    max = RightIsGreater(rightIndex, heapSize, array, max);

    if (max != index)
    {
        swap(&array[index], &array[max]);
        max_heapify(array, max, heapSize);
    }
}

int *build_max_heap(int array[], int arraySize)
{
    int j;
    for (j = arraySize / 2; j >= 0; j--)
    {
        max_heapify(array, j, arraySize);
    }
    return array;
}

void display(int *max_heap, int arraySize)
{
    int counter;
    for (counter = 0; counter < arraySize; counter++)
    {
        printf("%d ", max_heap[counter]);
    }
}

int main()
{
    int index, arraySize;
    int *array = calloc(MAX, sizeof(int));
    int *max_heap = calloc(MAX, sizeof(int));

    scanf("%d", &arraySize);

    for (index = 0; index < arraySize; index++)
    {
        scanf("%d", &array[index]);
    }
    max_heap = build_max_heap(array, arraySize);
    display(max_heap, arraySize);
}