//Cajero_ Hannah Montana Os

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int saldo = 1000;  // Variable que representa el saldo
pthread_mutex_t candado;  // Candado para operaciones atomicas :0

// Funcion para retiros
void* retirar(void* cantidad) {
    int monto = *(int*)cantidad;  
    
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&candado);  // Bloquear acceso al saldo
        if (saldo >= monto) {
            saldo -= monto;
            printf("Se retiraron %d. Saldo actual: %d\n", monto, saldo);
        } else {
            printf("No hay suficiente saldo para retirar %d\n", monto);
        }
        pthread_mutex_unlock(&candado);  // Desbloquear acceso al saldo
        sleep(1);  // Simula el tiempo de espera entre operaciones
    }

    return NULL;
}

// Funcion para realizar depósitos
void* depositar(void* cantidad) {
    int monto = *(int*)cantidad;  
    
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&candado);  // Bloquear acceso al saldo
        saldo += monto;
        printf("Se depositaron %d. Saldo actual: %d\n", monto, saldo);
        pthread_mutex_unlock(&candado);  // Desbloquear acceso al saldo
        sleep(1);  // Simula el tiempo de espera entre operaciones
    }

    return NULL;
}

int main() {
    pthread_t hilo_retiro, hilo_deposito;
    int monto_retiro = 200;
    int monto_deposito = 300;

    // Inicializar el candado
    pthread_mutex_init(&candado, NULL);

    // Crear los hilos
    pthread_create(&hilo_retiro, NULL, retirar, &monto_retiro);
    pthread_create(&hilo_deposito, NULL, depositar, &monto_deposito);

    // Esperar a que los hilos terminen
    pthread_join(hilo_retiro, NULL);
    pthread_join(hilo_deposito, NULL);

    // Destruir el candado
    pthread_mutex_destroy(&candado);

    printf("Operaciones finalizadas. Saldo final: %d\n", saldo);

    return 0;
}

