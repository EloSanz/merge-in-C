#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{   
    int dni;
    char nombreYApellido[30];
    float promedio;
}tEstudiantes;

/* Comparadores */
int compararPromedio(const void* a, const void* b){
    return ((tEstudiantes*)a)->promedio - ((tEstudiantes*)b)->promedio;
}

/* Creación y Lectura */
void crearArchivos(const char* nombre_1, const char* nombre_2){

    tEstudiantes estudiantes_1[] = {
        {12345678, "Juan Sanchez", 6.5},
        {23456789, "Maria Lopez", 7.5},
        {34567890, "Carlos Gomez", 8.5},
        {45678901, "Ana Martinez", 9.5},
        {56789012, "Pedro Rodriguez", 9.8}
    };
    tEstudiantes estudiantes_2[] = {
        {11111111, "Luisa Fernandez", 1.5},
        {78901234, "Jorge Perez", 2.5},
        {89012345, "Marta Lopez", 3.5},
        {90123456, "Alberto Gomez", 5.5},
        {12345678, "Juan Sanchez", 6.5},
        {23456789, "Maria Lopez", 7.5},
        {34567890, "Carlos Gomez", 8.5},
        {45678901, "Ana Martinez", 9.5},
        {56789012, "Pedro Rodriguez", 9.8}
    };
    
    int ce_1 = sizeof(estudiantes_1) / sizeof(tEstudiantes);
    int ce_2 = sizeof(estudiantes_2) / sizeof(tEstudiantes);
    FILE* archivo1 = fopen(nombre_1, "w");
    FILE* archivo2 = fopen(nombre_2, "w");

    for(int i = 0; i < ce_1; i++){
        fprintf(archivo1, "%d\t%-20s\t%.2f\n", estudiantes_1[i].dni, estudiantes_1[i].nombreYApellido, estudiantes_1[i].promedio);
    }
    for (int i = 0; i < ce_2; i++){
        fprintf(archivo2, "%d\t%-20s\t%.2f\n", estudiantes_2[i].dni, estudiantes_2[i].nombreYApellido, estudiantes_2[i].promedio);
    }

    fclose(archivo1);
    fclose(archivo2);

}
void leerArchivo(const char* nombre){
    FILE* archivo = fopen(nombre, "r");
    tEstudiantes aux;
    if(!archivo){
        return;
    }
    while(fscanf(archivo, "%d\t%20[^\t]\t%f\n", &aux.dni, aux.nombreYApellido, &aux.promedio) != EOF){
        printf("%d\t%s\t%.2f\n", aux.dni, aux.nombreYApellido, aux.promedio);
    }
    fclose(archivo);
}

/* Merge */
void mergeText(const char* nombre_1, const char* nombre_2, const char* nombre_3, int cmp(const void*, const void*)){
    FILE* archivo1 = fopen(nombre_1, "r"); FILE* archivo2 = fopen(nombre_2, "r"); FILE* archivo3 = fopen(nombre_3, "w");
    tEstudiantes aux1, aux2;
    if(!archivo1 || !archivo2 || !archivo3){
        return;
    }
    
    int read1 = fscanf(archivo1, "%d\t%20[^\t]\t%f\n", &aux1.dni, aux1.nombreYApellido, &aux1.promedio);
    int read2 = fscanf(archivo2, "%d\t%20[^\t]\t%f\n", &aux2.dni, aux2.nombreYApellido, &aux2.promedio);

    while (read1 != EOF && read2 != EOF) {
        if (cmp(&aux1, &aux2) <= 0) {
            fprintf(archivo3, "%d\t%-20s\t%.2f\n", aux1.dni, aux1.nombreYApellido, aux1.promedio);
            read1 = fscanf(archivo1, "%d\t%20[^\t]\t%f\n", &aux1.dni, aux1.nombreYApellido, &aux1.promedio);
        } else {
            fprintf(archivo3, "%d\t%-20s\t%.2f\n", aux2.dni, aux2.nombreYApellido, aux2.promedio);
            read2 = fscanf(archivo2, "%d\t%20[^\t]\t%f\n", &aux2.dni, aux2.nombreYApellido, &aux2.promedio);
        }
    }

    while (read1 != EOF) {
        fprintf(archivo3, "%d\t%-20s\t%.2f\n", aux1.dni, aux1.nombreYApellido, aux1.promedio);
        read1 = fscanf(archivo1, "%d\t%20[^\t]\t%f\n", &aux1.dni, aux1.nombreYApellido, &aux1.promedio);
    }

    while (read2 != EOF) {
        fprintf(archivo3, "%d\t%-20s\t%.2f\n", aux2.dni, aux2.nombreYApellido, aux2.promedio);
        read2 = fscanf(archivo2, "%d\t%20[^\t]\t%f\n", &aux2.dni, aux2.nombreYApellido, &aux2.promedio);
    }

    fclose(archivo1); fclose(archivo2); fclose(archivo3);
}

int main(){
    const char* nombreArchivo_1 = "archivo1.txt";
    const char* nombreArchivo_2 = "archivo2.txt";

    crearArchivos(nombreArchivo_1, nombreArchivo_2);
    leerArchivo(nombreArchivo_1);
    puts("");
    leerArchivo(nombreArchivo_2);

    const char* nombreArchivo_3 = "archivo3.txt";
    mergeText(nombreArchivo_1, nombreArchivo_2, nombreArchivo_3, compararPromedio);
    puts("");
    leerArchivo(nombreArchivo_3);
    return 0;
}