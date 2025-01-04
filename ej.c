#include <stdio.h>
#include <stdlib.h>
typedef struct{
    int dni;
    unsigned long long fechaDeInscripcion;
    char nombreYApellido[30];
    float promedio;
} tEstudiantes;

int compararPromedio(const void* a, const void* b){
    tEstudiantes* aux1 = (tEstudiantes*) a;
    tEstudiantes* aux2 = (tEstudiantes*) b;
    return aux1->promedio - aux2->promedio;
}
int compararDNI(const void* a, const void* b){
    tEstudiantes* aux1 = (tEstudiantes*) a;
    tEstudiantes* aux2 = (tEstudiantes*) b;
    return aux1->dni - aux2->dni;
}
int compararFechaDeInscripcion(const void* a, const void* b){
    tEstudiantes* aux1 = (tEstudiantes*) a;
    tEstudiantes* aux2 = (tEstudiantes*) b;
    return aux1->fechaDeInscripcion - aux2->fechaDeInscripcion;
}
void mostrarEstudiante(const void* param){
    tEstudiantes* aux = (tEstudiantes* ) param;
    printf("%d %llu %s %.2f\n", aux->dni, (*aux).fechaDeInscripcion, aux->nombreYApellido, aux->promedio);
}

void crearArchivos(const char* n1, const char* n2){
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

    int c1 = sizeof(estudiantes_1) / sizeof(estudiantes_1[0]);
    int c2 = sizeof(estudiantes_2) / sizeof(estudiantes_2[0]);
    
    FILE* archivo_1 = fopen(n1, "wb");
    FILE* archivo_2 = fopen(n2, "wb");

    if(!archivo_1 || !archivo_2){
        printf("Error al crear archivos\n");
        return ;
    }

    fwrite(estudiantes_1, sizeof(tEstudiantes), c1, archivo_1);
    fwrite(estudiantes_2, sizeof(tEstudiantes), c2, archivo_2);
    
    fclose(archivo_1);
    fclose(archivo_2);
}

void mostrarArchivoBinario(FILE* archivo, unsigned tam, void (*mostrar)(const void*)){
    rewind(archivo);
    void* buffer = malloc(tam);
    if(!buffer) return;

    fread(buffer, tam, 1, archivo);
    while (!feof(archivo)){
        mostrar(buffer);
        fread(buffer, tam, 1, archivo);
    }
    free(buffer);
}
void merge(FILE* arch1, FILE* arch2, FILE* arch3, unsigned tam, int (*cmp)(const void*, const void*)){
    rewind(arch1);
    rewind(arch2);

    void* buffer_1 = malloc(tam);
    void* buffer_2 = malloc(tam);

    size_t leido_1 = fread(buffer_1, tam, 1, arch1);
    size_t leido_2 = fread(buffer_2, tam, 1, arch2);

    while (leido_1 && leido_2){
        if(cmp(buffer_1, buffer_2) < 0){
            fwrite(buffer_1, tam, 1, arch3);
            leido_1 = fread(buffer_1, tam, 1, arch1);
        }else{
            fwrite(buffer_2, tam, 1, arch3);
            leido_2 = fread(buffer_2, tam, 1, arch2);
        }
    }
    while (leido_1){
        fwrite(buffer_1, tam, 1, arch3);
        leido_1 = fread(buffer_1, tam, 1, arch1);
    }
    while (leido_2){
        fwrite(buffer_2, tam, 1, arch3);
        leido_2 = fread(buffer_2, tam, 1, arch2);
    }
    free(buffer_1);
    free(buffer_2);
}

int main(){
    unsigned tam = sizeof(tEstudiantes);
    crearArchivos("archivo_1.dat", "archivo_2.dat");
    
    FILE* archivo_1 = fopen("archivo_1.dat", "rb");
    FILE* archivo_2 = fopen("archivo_2.dat", "rb");
    FILE* archivo_3 = fopen("archivo_3.dat", "w+b");
    if(!archivo_1 || !archivo_2 || !archivo_3){
        printf("Fallo al leer\n");
        return 1;
    }


    mostrarArchivoBinario(archivo_1, tam, mostrarEstudiante);
    mostrarArchivoBinario(archivo_2, tam, mostrarEstudiante);

    merge(archivo_1, archivo_2, archivo_3,tam, compararFechaDeInscripcion);
    printf("\n****MERGE****\n");
    mostrarArchivoBinario(archivo_3, tam, mostrarEstudiante);
    return 0;
}
