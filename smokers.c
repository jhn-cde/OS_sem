//Problem:    Cigarette Smokers Problem
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

sem_t s_agent;
sem_t s_tobacco, s_paper, s_matches;

void *agent(void *arg)
{
    int fum;
    while(1)
    {
        sleep(1);

        sem_wait(&s_agent);    // wait till smoker ends
        fum = rand() % 3;   // choose smoker randomly
        
        if(fum == 0)        // tobacco_smoker
        {
            printf("\nAgent places paper and matches on table");
            sem_post(&s_tobacco);
        }
        else if (fum == 1)  // paper_smoker
        {
            printf("\nAgent places tobacco and matches on table");
            sem_post(&s_paper);
        }
        else                // matches_smoker
        {
            printf("\nAgent places tobacco and paper on table");
            sem_post(&s_matches);
        }
    }
}

// fumadores
void *smoker_tobacco(void *item)
{
    while(1)
    {
        sem_wait(&s_tobacco);    // wait paper and matches
        printf("\nsmoker tobacco making a cigarette\n");
        sleep(1);
        
        printf("smoker tobacco - ends\n");
        sem_post(&s_agent);    // notify agent
    }
}
void *smoker_paper(void *item)
{
    while(1)
    {
        sem_wait(&s_paper);    // wait tobacco and matches
        printf("\nsmoker paper making a cigarette\n");
        sleep(1);
        
        printf("smoker paper - ends\n");
        sem_post(&s_agent);    // notify agent
    }
}
void *smoker_matches(void *item)
{
    while(1)
    {
        sem_wait(&s_matches);    // wait tobacco and paper
        printf("\nsmoker matches making a cigarette\n");
        sleep(1);

        printf("smoker matches - ends\n");
        sem_post(&s_agent);    // notify agent
    }
}


//Programa principal
int main()
{
    srand((unsigned) time(&t));

    sem_init(&s_agent, 0, 1);
    sem_init(&s_tobacco, 0, 0);
    sem_init(&s_paper, 0, 0);
    sem_init(&s_matches, 0, 0);

    // threads
    pthread_t t_agent, t_smoker_tobacco, t_smoker_paper, t_smoker_matches;
    //agent
    pthread_create(&t_agent, NULL, agent, NULL);
    // smokers
    pthread_create(&t_smoker_tobacco, NULL, smoker_tobacco, NULL);
    pthread_create(&t_smoker_paper, NULL, smoker_paper, NULL);
    pthread_create(&t_smoker_matches, NULL, smoker_matches, NULL);

    pthread_join(t_agent, NULL);
    pthread_join(t_smoker_tobacco, NULL);
    pthread_join(t_smoker_paper, NULL);
    pthread_join(t_smoker_matches, NULL);

    return 0;  
}
