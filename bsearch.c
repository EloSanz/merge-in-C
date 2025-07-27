#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int (*compare_t)(void *, void *);

int binarySearch(int arr[], int n, int key) {
    int left = 0;
    int right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (arr[mid] == key) {
            return mid;
        }
        if (arr[mid] < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int binarySearchRecursive(int arr[], int left, int right, int key) {
    if (left > right) {
        return -1;
    }
    int mid = (left + right) / 2;
    if (arr[mid] == key) {
        return mid;
    }
    if (arr[mid] < key) {
        return binarySearchRecursive(arr, mid + 1, right, key);
    } else {
        return binarySearchRecursive(arr, left, mid - 1, key);
    }
}

void* my_bsearch(void *arr, int n, unsigned tam, compare_t compare, void *key, int *pasos) {
    if (n <= 0) {
        return NULL;
    }
    int mid = n / 2;
    void *mid_elem = arr + (mid * tam);
    int cmp = compare(mid_elem, key);
    (*pasos)++;
    if (cmp == 0) return mid_elem;
    if (cmp < 0) return my_bsearch(arr + ((mid + 1) * tam), n - mid - 1, tam, compare, key, pasos);
    return my_bsearch(arr, mid, tam, compare, key, pasos);
}

int compare(void *a, void *b) {
    return *(int *)a - *(int *)b;
}

void init_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

int main() {
    int n = 10000000;
    int *arr = malloc(n * sizeof(int));
    if (!arr) {
        printf("Error: No se pudo asignar memoria\n");
        return 1;
    }
    
    int k = -1;
    init_array(arr, n);
    unsigned tam = sizeof(int);
    int pasos = 0;
    int *result = my_bsearch(arr, n, tam, compare, &k, &pasos);
    
    if (result) {
        printf("Element found: %d\n", *(int *) result);
        printf("Pasos: %d\n", pasos);
    } else {
        printf("Element not found\n");
    }
    
    free(arr);
    return 0;
}
