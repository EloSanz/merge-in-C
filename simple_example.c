#include <stdio.h>

void showVector(int vector[], int n){
    for(int i = 0; i < n; i++){
        printf("%d ", vector[i]);
    }
    printf("\n");
}
void merge(int vector1[], int vector2[], int vector3[], int n, int m){
    int i = 0, j = 0, k = 0;

    while(i < n && j < m){
        if(vector1[i] < vector2[j]){
            vector3[k] = vector1[i];
            i++;
            k++;
        }else{
            vector3[k] = vector2[j];
            k++;
            j++;
        }
    }
    while(i < n){
        vector3[k] = vector1[i];
        i++;
        k++;
    }
    while(j < m){
        vector3[k] = vector2[j];
        k++;
        j++;
    }
}
int main(){

    int vector1[] = {1, 2, 4, 5, 11};
    int vector2[] = {3, 7, 9, 10, 13};
    int n = sizeof(vector1)/sizeof(vector1[0]);
    int m = sizeof(vector2)/sizeof(vector2[0]);
    int vector3[n + m ];


    merge(vector1, vector2, vector3, n, m);
    showVector(vector3, n + m);
   return 0;
}