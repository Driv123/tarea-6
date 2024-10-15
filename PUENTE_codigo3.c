//Puente- Hannah Montana Os

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TOTAL_VEHICULOS 100
#define VEHICULOS_SENTIDO 50

// Semaforo que es para controlar el acceso al puente
sem_t puente;

// Variables que son para contar los vehiculos en cada sentido xd
int vehiculos_norte = 0;
int vehiculos_sur = 0;
pthread_mutex_t mutex;

// Funcion para los vehiculos que van en el sentido norte-sur
void* cruzar_puente_norte(void* arg) {
    sleep(rand() % 20);  

    
    pthread_mutex_lock(&mutex);
    vehiculos_norte++;
    if (vehiculos_norte == 1) {  
        sem_wait(&puente);
    }
    pthread_mutex_unlock(&mutex);

    printf("Vehiculo %d (Norte-Sur) esta cruzando el puente\n", (int)arg);
    sleep(1);  // Este simula que esta cruzando el puente 
    printf("Vehiculo %d (Norte-Sur) ha salido del puente\n", (int)arg);

    // Al salir del puente
    pthread_mutex_lock(&mutex);
    vehiculos_norte--;
    if (vehiculos_norte == 0) {  
        sem_post(&puente);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Funcion para los vehiculos que van en el sentido sur-norte
void* cruzar_puente_sur(void* arg) {
    sleep(rand() % 20);  

    // Intentar acceder al puente
    pthread_mutex_lock(&mutex);
    vehiculos_sur++;
    if (vehiculos_sur == 1) {  
        sem_wait(&puente);
    }
    pthread_mutex_unlock(&mutex);

    printf("Vehiculo %d (Sur-Norte) esta cruzando el puente\n", (int)arg);
    sleep(1);  // Simula que esta cruzando el puente
    printf("Vehiculo %d (Sur-Norte) ha salido del puente\n", (int)arg);

    // Al salir del puente
    pthread_mutex_lock(&mutex);
    vehiculos_sur--;
    if (vehiculos_sur == 0) {  
        sem_post(&puente);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t vehiculos[TOTAL_VEHICULOS];
    int ids[TOTAL_VEHICULOS];
    int i;

    
    sem_init(&puente, 0, 1);  // Control del puente
    pthread_mutex_init(&mutex, NULL);

    srand(time(NULL));

    // Crear los hilos alternando la direccion de los vehiculos
    for (i = 0; i < TOTAL_VEHICULOS; i++) {
        ids[i] = i + 1;
        if (i % 2 == 0) {
            pthread_create(&vehiculos[i], NULL, cruzar_puente_norte, &ids[i]);
        } else {
            pthread_create(&vehiculos[i], NULL, cruzar_puente_sur, &ids[i]);
        }
    }

    // Esperar a que todos los hilos terminen
    for (i = 0; i < TOTAL_VEHICULOS; i++) {
        pthread_join(vehiculos[i], NULL);
    }

    // Destruir el semaforo y el mutex
    sem_destroy(&puente);
    pthread_mutex_destroy(&mutex);

    return 0;
}

