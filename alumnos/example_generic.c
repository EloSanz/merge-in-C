#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
    int dni;
    unsigned long long fechaDeInscripcion;
    char nombreYApellido[30];
    float promedio;
} tEstudiantes;

/* Comparadores */
int compararNota(const void *elemento1, const void *elemento2){
    tEstudiantes *estudiante1 = (tEstudiantes *)elemento1;
    tEstudiantes *estudiante2 = (tEstudiantes *)elemento2;
        
    return estudiante1->promedio - estudiante2->promedio;
    }
int compararFechaDeInscripcion(const void *elemento1, const void *elemento2){
    tEstudiantes *estudiante1 = (tEstudiantes *)elemento1;
    tEstudiantes *estudiante2 = (tEstudiantes *)elemento2;
    return estudiante1->fechaDeInscripcion - estudiante2->fechaDeInscripcion;
}
/**/

/* Prints*/
int mostrarArchivoBinario(FILE *archivo, size_t tamanoDato, void (*mostrar)(const void *)){
    rewind(archivo);
    void *elemento = malloc(tamanoDato);
    if (!elemento)
        return 0;
    fread(elemento, tamanoDato, 1, archivo);
    while (!feof(archivo))
    {
        mostrar(elemento);
        fread(elemento, tamanoDato, 1, archivo);
    }
    free(elemento);
    rewind(archivo);
    return 1;
}
void mostrar(const void *elemento){
    tEstudiantes *estudiante = (tEstudiantes *)elemento;
    printf("%d\t%llu\t%20s\t%.2f\n", estudiante->dni, estudiante->fechaDeInscripcion, estudiante->nombreYApellido, estudiante->promedio);
}

/* Archivos */
int crearArchivos(char *nombreArchivo_1, char *nombreArchivo_2){
    tEstudiantes estudiantes_1 [] = {
        {12345678, 20210101, "Juan Sanchez", 6.5},
        {23456789, 20210102, "Maria Lopez", 7.5},
        {34567890, 20210103, "Carlos Gomez", 8.5},
        {45678901, 20210104, "Ana Martinez", 9.5},
        {56789012, 20210105, "Pedro Rodriguez", 9.8}
    } ;
    tEstudiantes estudiantes_2 [] = {
        {11111111, 20210106, "Luisa Fernandez", 1.5},
        {78901234, 20210107, "Jorge Perez", 2.5},
        {89012345, 20210108, "Marta Lopez", 3.5},
        {90123456, 20210109, "Alberto Gomez", 5.5},
        {12345678, 20210110, "Ana Martinez", 9.5}
    };
 
    int cantidad = sizeof(estudiantes_1) / sizeof(tEstudiantes);
    int cantidad2 = sizeof(estudiantes_2) / sizeof(tEstudiantes);

    FILE* archivo  = fopen(nombreArchivo_1, "wb");
    FILE* archivo2 = fopen(nombreArchivo_2, "wb");

    if (!archivo || !archivo2){
        if (archivo) fclose(archivo);
        if (archivo2) fclose(archivo2);
        return 0;
    }
        
    fwrite(estudiantes_1, sizeof(tEstudiantes), cantidad, archivo);
    fwrite(estudiantes_2, sizeof(tEstudiantes), cantidad2, archivo2);


    fclose(archivo);
    fclose(archivo2);
    
    return 1;
}
int abrirArchivo(FILE **archivo, char *nombreArchivo, char *modoApertura){
    *archivo = fopen(nombreArchivo, modoApertura);
    return archivo ? 1 : 0;
}

/********************************************************************************/
void mergeArchivos(FILE *file1, FILE *file2, FILE *fileOut, unsigned tamEstructura, int (*comparar)(const void *, const void *)){
    rewind(file2);
    rewind(file1);

    void *buffer1 = malloc(tamEstructura);
    void *buffer2 = malloc(tamEstructura);

    size_t leido1 = fread(buffer1, tamEstructura, 1, file1);
    size_t leido2 = fread(buffer2, tamEstructura, 1, file2);

    while (leido1 && leido2){
        if (comparar(buffer1, buffer2) <= 0){
            fwrite(buffer1, tamEstructura, 1, fileOut);
            leido1 = fread(buffer1, tamEstructura, 1, file1);
        }
        else{
            fwrite(buffer2, tamEstructura, 1, fileOut);
            leido2 = fread(buffer2, tamEstructura, 1, file2);
        }
    }

    while (leido1){
        fwrite(buffer1, tamEstructura, 1, fileOut);
        leido1 = fread(buffer1, tamEstructura, 1, file1);
    }

    while (leido2){
        fwrite(buffer2, tamEstructura, 1, fileOut);
        leido2 = fread(buffer2, tamEstructura, 1, file2);
    }

    free(buffer1);
    free(buffer2);
}

int main(){
    unsigned tam = sizeof(tEstudiantes);
    crearArchivos("estudiantes1.dat", "estudiantes2.dat");

    FILE* archivo_1 = fopen("estudiantes1.dat", "rb");
    FILE* archivo_2 = fopen("estudiantes2.dat", "rb");
    FILE* archivo_3 = fopen("estudiantesMerge.dat", "w+b");

    mostrarArchivoBinario(archivo_1, tam, mostrar);
    printf("\n**********************************\n");
    mostrarArchivoBinario(archivo_2, tam, mostrar);

    printf("\n****************** MERGE ****************\n");
    mergeArchivos(archivo_1, archivo_2, archivo_3, tam, compararNota);
    rewind(archivo_3);
    mostrarArchivoBinario(archivo_3, tam, mostrar);
    return 0;
}