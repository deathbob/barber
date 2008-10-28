#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_THREADS 10
pthread_t threads[NUM_THREADS];

int cuts = 0;
int seats = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t customers;
pthread_cond_t barber;


void *barberBob(void *threadid){
  int haircuts = 90;
  while(haircuts > 0){
    pthread_mutex_lock(&mutex);    
    pthread_cond_wait(&customers, &mutex);
    haircuts--;
    printf("\nBarber Thread Cutting Hair!: seats left = %d, haircuts before close = %d\n",seats,haircuts);
    seats++;
    pthread_cond_signal(&barber);
    pthread_mutex_unlock(&mutex);
  }
  printf(" All haircuts for today given, closing shop \n");
  pthread_exit(NULL);
}

void *customer(void *threadid)
{
  int haircuts = 10;
  while(haircuts > 0){
    pthread_mutex_lock(&mutex);
    if(seats > 0){
      seats--;
      haircuts--;
      cuts++;
      pthread_cond_signal(&customers);
      printf("\n I'm getting my hair cut! %d seats, %d threadid %d cuts done total, %d haircuts left for me \n",seats,(int)threadid,cuts,haircuts);
      pthread_cond_wait(&barber, &mutex);
      pthread_mutex_unlock(&mutex);
    }
    else{
      printf("\n NO SEATS I'll come back tomorrow :( thread %d \n",(int)threadid);
      pthread_mutex_unlock(&mutex);
    }
    sleep(1);
  }
  pthread_exit(NULL);
}



int main(int argc, char *argv[])
{
  if(argc < 2 || argc > 3)
    printf("Wrong number of arguments: Usage > barber.exe x \n");
  seats = atoi(argv[1]);
  printf("number of seats %d \n", seats);
  pthread_cond_init(&customers,NULL);
  pthread_cond_init(&barber,NULL);
  int t;
  for(t = 0; t < NUM_THREADS; t++){
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

