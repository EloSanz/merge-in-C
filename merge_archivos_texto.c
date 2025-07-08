#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 256

typedef struct {
    int dni;
    unsigned long long fechaDeInscripcion;
    char nombreYApellido[30];
    float promedio;
} tEstudiantes;

// Lee una línea del archivo y la parsea en la estructura tEstudiantes
int leerEstudianteTxt(FILE* arch, void* buffer, unsigned tam) {
    tEstudiantes est;
    char linea[MAX_LINEA];
    if (!fgets(linea, sizeof(linea), arch))
        return 0;
    int cantBytes = sscanf(linea, "%d %llu %s %f",
         &est.dni, &est.fechaDeInscripcion, est.nombreYApellido, &est.promedio) == 4;

    memcpy(buffer, &est, tam);
    return cantBytes;
}

// Escribe un estudiante en formato texto
void escribirEstudianteTxt(FILE* arch, const tEstudiantes* est) {
    fprintf(arch, "%d %llu %s %.2f\n", est->dni, est->fechaDeInscripcion, est->nombreYApellido, est->promedio);
}

// Muestra un estudiante en pantalla
void mostrarEstudianteTxt(const tEstudiantes* est) {
    const char* color;
    if(est->promedio < 4)
        color = "\033[31m"; // rojo
    else if(est->promedio < 7)
        color = "\033[33m"; // amarillo
    else
        color = "\033[32m"; // verde
    printf("%d %llu %s %s%.2f\033[0m\n", est->dni, est->fechaDeInscripcion, est->nombreYApellido, color, est->promedio);
}

// Merge de dos archivos de texto ordenados por fecha de inscripción
void mergeTxt(FILE* arch1, FILE* arch2, FILE* arch3, unsigned tam, int (*cmp)(const void*, const void*)) {
    rewind(arch1);
    rewind(arch2);

    void* buffer1 = malloc(tam);
    void* buffer2 = malloc(tam);

    size_t leido1 = leerEstudianteTxt(arch1, buffer1, tam);
    size_t leido2 = leerEstudianteTxt(arch2, buffer2, tam);

    while (leido1 && leido2) {
        if (cmp(buffer1, buffer2) < 0) {
            escribirEstudianteTxt(arch3, buffer1);
            leido1 = leerEstudianteTxt(arch1, buffer1, tam);
        } else {
            escribirEstudianteTxt(arch3, buffer2);
            leido2 = leerEstudianteTxt(arch2, buffer2, tam);
        }
    }
    while (leido1) {
        escribirEstudianteTxt(arch3, buffer1);
        leido1 = leerEstudianteTxt(arch1, buffer1, tam);
    }
    while (leido2) {
        escribirEstudianteTxt(arch3, buffer2);
        leido2 = leerEstudianteTxt(arch2, buffer2, tam);
    }
    free(buffer1);
    free(buffer2);
}

// Crea archivos de texto de ejemplo
void crearArchivosTxt(const char* n1, const char* n2) {
    FILE* f1 = fopen(n1, "w");
    FILE* f2 = fopen(n2, "w");
    if (!f1 || !f2) {
        printf("Error al crear archivos de texto\n");
        if(f1) fclose(f1);
        if(f2) fclose(f2);
        return;
    }
    fprintf(f1, "12345678 20210101 Juan_Sanchez 6.5\n");
    fprintf(f1, "23456789 20210102 Maria_Lopez 7.5\n");
    fprintf(f1, "34567890 20210103 Carlos_Gomez 8.5\n");
    fprintf(f1, "45678901 20210104 Ana_Martinez 9.5\n");
    fprintf(f1, "56789012 20210105 Pedro_Rodriguez 9.8\n");

    fprintf(f2, "11111111 20210106 Luisa_Fernandez 1.5\n");
    fprintf(f2, "78901234 20210107 Jorge_Perez 2.5\n");
    fprintf(f2, "89012345 20210108 Marta_Lopez 3.5\n");
    fprintf(f2, "90123456 20210109 Alberto_Gomez 5.5\n");
    fprintf(f2, "12345678 20210110 Ana_Martinez 9.5\n");
    fprintf(f2, "12345678 20210110 Ana_Martinez 9.5\n");
    fprintf(f2, "12345678 20210110 Ana_Martinez 9.5\n");
    fclose(f1);
    fclose(f2);
}

// Muestra el contenido de un archivo de texto
void mostrarArchivoTxt(const char* nombre) {
    FILE* f = fopen(nombre, "r");
    if (!f) {
        printf("No se pudo abrir %s\n", nombre);
        return;
    }
    int tam = sizeof(tEstudiantes);
    tEstudiantes est;
    printf("Archivo %s:\n", nombre);
    while (leerEstudianteTxt(f, &est, tam)) {
        mostrarEstudianteTxt(&est);
    }
    fclose(f);
}

int compararPromedio(const void* a, const void* b){
    tEstudiantes* aux1 = (tEstudiantes*) a;
    tEstudiantes* aux2 = (tEstudiantes*) b;
    return aux1->promedio - aux2->promedio;
}

int main() {
