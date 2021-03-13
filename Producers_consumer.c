#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <syscall.h>
 
#define TAMBUF 5
 
int buffer[TAMBUF]; //Produced elements
int indp = 0;
 
sem_t full, emp, mutex; 
 
//Producer
void *producer(void *arg)
{
   //Variables
   int j;
   int prod;
   while(1)
   {
       //produce element
       prod = (rand() % 26) + 65;
      
       sem_wait(&emp);
       sem_wait(&mutex);
 
       //add element to buffer
       buffer[indp] = prod;
 
       indp = (indp + 1) % TAMBUF;
 
       //Element produced
       printf("Producer %li: %c\n", syscall(SYS_gettid), producido);
 
       //Show elements
       for(j = 0; j < TAMBUF; j++)
           printf("%c  ", buffer[j]);
 
       printf("\n\n");
 
       sem_post(&mutex);
       sem_post(&full);
       int random = (rand() % 3) + 5;
       sleep(random);
   }
 
   pthread_exit(0);
}
 
//Consumer
void *consumer(void *arg)
{
   //Variables
   int indc = 0;
   int cons;
   int as = 42;
   while(1)
   {
       //sleep(3);
       sem_wait(&full);
       sem_wait(&mutex);
 
       cons = buffer[indc];

       buffer[indc] = as;
 
       indicec = (indc + 1) % TAMBUF;

       printf("Consumer %li: %c\n", syscall(SYS_gettid), cons);
 
       sem_post(&mutex);
       sem_post(&emp);
   }
   pthread_exit(0);
}

int main()
{
   pthread_t idProd1, idProd2;
   pthread_t idCons;

   sem_init(&full, 0, 0);
   sem_init(&emp, 0, TAMBUF);
   sem_init(&mutex, 0, 1);
 
   printf("\nBuffer size %d\n\n", TAMBUF);
  
   //threads
   //Producer:
   pthread_create(&idProd1, NULL, producer, NULL);
   pthread_create(&idProd2, NULL, producer, NULL);
 
   //consumer
   pthread_create(&idCons, NULL, consumer, NULL);
 
   pthread_join(idProd1, NULL);
   pthread_join(idProd2, NULL);
   pthread_join(idCons, NULL);
 
   return 0;  
}
