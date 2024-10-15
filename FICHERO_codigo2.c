//FICHERO_ Hannah Montana OS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE_LENGTH 256

void tarea1(const char *nombre_archivo, int tuberia1[2], int tuberia2[2]) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        perror("Error abriendo el archivo");
        exit(1);
    }

    char linea[MAX_LINE_LENGTH];
    int numero_linea = 0;

    // Cerrar las partes de lectura de las tuberias
    close(tuberia1[0]);
    close(tuberia2[0]);

    while (fgets(linea, sizeof(linea), archivo)) {
        if (numero_linea % 2 == 0) {
            write(tuberia1[1], linea, strlen(linea));
        } else {
            write(tuberia2[1], linea, strlen(linea));
        }
        numero_linea++;
    }

    // Cerrar las partes de escritura de las tuberias
    close(tuberia1[1]);
    close(tuberia2[1]);

    fclose(archivo);
}

void tarea2(int tuberia_par[2], int tuberia3[2], const char *palabra) {
    // Redireccionar entrada y salida para grep
    dup2(tuberia_par[0], STDIN_FILENO);  
    dup2(tuberia3[1], STDOUT_FILENO);    

    
    close(tuberia_par[1]);
    close(tuberia3[0]);

    
    execlp("grep", "grep", palabra, NULL);

    // Si execlp falla
    perror("Error ejecutando grep en tarea 2");
    exit(1);
}

void tarea3(int tuberia_imp[2], int tuberia3[2], const char *palabra) {
    // Redireccionar entrada y salida para grep
    dup2(tuberia_imp[0], STDIN_FILENO);  
    dup2(tuberia3[1], STDOUT_FILENO);    

    // Cerrar las partes no utilizadas
    close(tuberia_imp[1]);
    close(tuberia3[0]);

    // Ejecutar grep
    execlp("grep", "grep", palabra, NULL);

    // Si execlp falla
    perror("Error ejecutando grep en tarea 3");
    exit(1);
}

void tarea4(int tuberia3[2]) {
    
    dup2(tuberia3[0], STDIN_FILENO);  // Leer desde la tuberia 3

    // Cerrar la parte de escritura de la tuberia
    close(tuberia3[1]);

    // Ejecutar sort
    execlp("sort", "sort", NULL);

    // Si execlp falla
    perror("Error ejecutando sort en tarea 4");
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <archivo> <palabra1> <palabra2>\n", argv[0]);
        exit(1);
    }

    const char *nombre_archivo = argv[1];
    const char *palabra1 = argv[2];
    const char *palabra2 = argv[3];

    int tuberia1[2], tuberia2[2], tuberia3[2];

    // Crear las tuberias
    pipe(tuberia1);
    pipe(tuberia2);
    pipe(tuberia3);

    // Crear los procesos
    if (fork() == 0) {
        // Hijo 1: ejecuta la tarea 1 (leer el archivo y distribuir en tuberias)
        tarea1(nombre_archivo, tuberia1, tuberia2);
        exit(0);
    }

    if (fork() == 0) {
        
        close(tuberia2[0]);  
        close(tuberia2[1]);  
        tarea2(tuberia1, tuberia3, palabra1);
    }

    if (fork() == 0) {
        // Hijo 3: ejecuta la tarea 3 (grep sobre lineas impares)
        close(tuberia1[0]);  // No necesitamos la tuberia de lineas pares
        close(tuberia1[1]);  // Cerrar ambos extremos de la tuberia de pares
        tarea3(tuberia2, tuberia3, palabra2);
    }

    if (fork() == 0) {
        // Hijo 4: ejecuta la tarea 4 (sort sobre la tuberia 3)
        tarea4(tuberia3);
    }

    // Cerrar las tuberias en el proceso padre
    close(tuberia1[0]);
    close(tuberia1[1]);
    close(tuberia2[0]);
    close(tuberia2[1]);
    close(tuberia3[0]);
    close(tuberia3[1]);

    // Esperar a que los hijos terminen
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    return 0;
}

