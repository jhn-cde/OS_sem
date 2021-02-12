//Problem:    Barber Problem solution using Semaphores
//Autor:      Johan Huamam
//FCreacion:  01/02/2021

#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <syscall.h>

time_t t;

sem_t customer_ready, barber_ready;
sem_t seats; // mutex

int free_seats = 5; // Number of free seats
const int n_seats = 5; // Number of seats

void *Barber (void *arg){
    while(1) {
        // Waiting for customer (sleep)
        printf("Barber is waiting\n");
        sem_wait(&customer_ready);

        // begin critical region
        sem_wait(&seats);
        
        free_seats++;
        
        printf("Barber is ready\n");
        sem_post(&barber_ready);
        
        // end critical region
        sem_post(&seats);
        
        // Barber is working
        sleep(1);
    }
    pthread_exit(0);
}

void *Customer (void *arg){
    while(1) {
        sleep((rand() % 7) + 2);
        
        // begin critical region
        sem_wait(&seats);
        if(free_seats > 0) { 
            // Sentandose
            free_seats--;
            
            printf("Customer %li ready, seat: %d\n", syscall(SYS_gettid), n_seats-free_seats);
            sem_post(&customer_ready);
            
            // end critical region
            sem_post(&seats); 
            
            // wait for barber
            sem_wait(&barber_ready);
            // customer is being served

        } else {
            // end critical region
            sem_post(&seats);
            
            sleep((rand() % 2) + 3);
        } 
    } 
}


int main()
{
    srand((unsigned) time(&t));

    // semaphores
    sem_init(&customer_ready, 0, 0);
    sem_init(&barber_ready, 0, 0);
    sem_init(&seats, 0, 1);

    // threads
    pthread_t barber;
    pthread_t customers[10];

    pthread_create(&barber, NULL, Barber, NULL);
    for (int i = 0; i < 10; i++)
        pthread_create(&customers[i], NULL, Customer, NULL);
        
    pthread_join(barber, NULL);
    for (int i = 0; i < 10; i++)
        pthread_join(customers[i], NULL);
    
    return 0;  
}
