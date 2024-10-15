#include <stdio.h>
#include <string.h>
#include <windows.h>k

struct registro {
    int puntaje;
    char name[50];
};
int main() {
	system("mode con cols=80 lines=25");
    FILE *archivo;
    char name[50];
    struct registro registros[100];
    int i;
    for (i = 0; i < 10; i++) {
        printf("Ingrese su nombre %d: ", i + 1);
        fgets(name, sizeof(name), stdin);
        strcpy(registros[i].name, name);
        registros[i].name[strcspn(registros[i].name, "\n")] = '\0'; 
        printf("Ingrese su puntaje total: ");
        scanf("%d", &registros[i].puntaje);
        getchar(); 
    }
    archivo = fopen("registros.txt", "w");
    if (archivo != NULL) {
        for (i = 0; i < 10; i++) {
            fprintf(archivo, "%s %d\n", registros[i].name, registros[i].puntaje);
        }
        fclose(archivo);
        printf("Se han guardado los registros en el archivo 'registros.txt'\n");
    } else {
        printf("No se pudo abrir el archivo\n");
    }
    system("pause");

    return 0;
}

