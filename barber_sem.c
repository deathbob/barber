/**
	Author: Andrew Anderson <aanderso> and Robert Larrick <rlarrick>
	Class: CS 471
	Project Assignment: Scheduling
**/
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_THREADS 100
pthread_t threads[NUM_THREADS];

int seats = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t customers;
sem_t barber;


void *barberBob(void *threadid){
  int haircuts = NUM_THREADS-1;
  while(haircuts > 0){
    sem_wait(&customers);
    pthread_mutex_lock(&mutex);
    seats++;
    sem_post(&barber);
    haircuts--;
    printf("cutting some hair, waiting on next customer\n");
    pthread_mutex_unlock(&mutex);
    printf("\n Barber Thread: haircuts = %d, chairs left = %d \n",haircuts,seats);
  }
  printf("Closing shop, bye!\n");
  pthread_exit(NULL);
}

void *customer(void *threadid)
{

  int haircuts = 1;
  while(haircuts > 0){
    pthread_mutex_lock(&mutex);
    if(seats > 0){
      seats--;
      printf("taking a seat, waiting on barber\n");
      sem_post(&customers);
      haircuts--;
  sleep(1);
      pthread_mutex_unlock(&mutex);
      sem_wait(&barber);
      printf("getting a haircut %d \n", (int)threadid);
    }
    else{
      pthread_mutex_unlock(&mutex);
      printf("\n Customer Thread:%d couldn't get a haircut, seats = %d\n",(int)threadid, seats);
      sleep(1);
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if(argc < 2 || argc > 3){
    printf("Wrong number of arguments: Usage > barber.exe x \n");
  }
  seats = atoi(argv[1]);
  printf("number of chairs %d \n", seats);
  sem_init(&customers,0,0);
  sem_init(&barber,0,0);
  int t;
  for(t=0;t<NUM_THREADS;t++){
    printf("Creating thread %d\n", t);
    if(t == 0){
      pthread_create(&threads[t], NULL, barberBob, (void *)t);
    }
    else{
      pthread_create(&threads[t], NULL, customer, (void *)t);
    }
  }


  for(t = 0; t < NUM_THREADS; t++)
    pthread_join(threads[t],NULL);
  pthread_exit(NULL);
}


