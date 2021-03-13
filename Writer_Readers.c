#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <syscall.h>
 
#define nReaders 5
#define nWriters 5
 
sem_t wrt, rdr;
int n_rdrs = 0;
int writer_id = -1;

void *writer(void *arg)
{
   int thread_id = syscall(SYS_gettid);
   while(1)
   {
       printf("%d trying to write\n", thread_id);

       sem_wait(&wrt);

       printf("%d is writing\n", thread_id);
       writer_id = thread_id;
       sleep(1);
 
       sem_post(&rdr);
 
       sleep(1);
   }
   pthread_exit(0);
}

void *reader(void *arg)
{
   int thread_id = syscall(SYS_gettid);
   while(1)
   {
       sem_wait(&rdr);

       n_rdrs++;

       if(n_rdrs == 1)
           sem_wait(&wrt);
 
       sem_post(&rdr);

       sleep(2);

       sem_wait(&rdr);
 
       n_rdrs--;

       if(n_rdrs== 0)
           sem_post(&wrt);
 
       sem_post(&rdr);
 
       sleep(2);
   }
   pthread_exit(0);
}

int main()
{
   pthread_t writers[nroWriters], readers[nReaders];

   sem_init(&wrt, 0, 1);
   sem_init(&rdr, 0, 1);

   for (int i = 0; i < nWriters; i++)
   {
       pthread_create(&writers[i], NULL, writer, NULL);
       pthread_create(&writers[i], NULL, reader, NULL);
   }

   for (int i = 0; i < nWriters; i++)
   {
       pthread_join(writers[i], NULL);
       pthread_join(readers[i], NULL);
   }

   return 0;  
}
