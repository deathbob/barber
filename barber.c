#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_THREADS 100
pthread_t threads[NUM_THREADS];

int cuts = 0;// NUM_THREADS;
int seats = 0;
pthread_mutex_t b_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t customers;
pthread_cond_t barber;

int max_seats = 0;

void *barberBob(void *threadid){
  int haircuts = NUM_THREADS - 1;
  while(haircuts > 0 || seats != max_seats){
    pthread_mutex_lock(&b_mutex);
    printf("1)  BBBBB   Barber lock b_mutex, wait for signal on customers\n");
    pthread_cond_wait(&customers, &b_mutex);
    printf("4)  BBBBB  barber received customers signal, should have lock on b_mutex\n");
    pthread_cond_signal(&barber);
    printf("5)  BBBBB   barber just signaled he's ready\n");
    seats++;
    printf("6)  BBBBB   barber just incremented seats\n");
    pthread_mutex_unlock(&b_mutex);
    printf("7)  BBBBB  barber just unlocked b_mutex\n");
    //cutting hair
    haircuts--;
    printf("\nBarber Thread Cutting Hair!: seats left = %d, haircuts before close = %d\n",seats,haircuts);
  }
  printf(" All haircuts for today given, closing shop \n");
  pthread_exit(NULL);
}

void *customer(void *threadid)
{
    int haircuts = 1;
    while(haircuts > 0){
      sleep(1);
    pthread_mutex_lock(&b_mutex);
    if(seats > 1){
      seats--;
      printf("2)  CCCCC   customer just decremented seats\n");
      haircuts--;
      pthread_cond_signal(&customers);
      //      pthread_mutex_unlock(&b_mutex);// might be shady
      printf("3)  CCCCC   custmer signaled barber he's here, about to wait on barber \n");
      pthread_cond_wait(&barber, &b_mutex);
      printf("8)  CCCCC   signaled barber, got b_mutex back\n");
      if(seats < max_seats){pthread_cond_signal(&customers);}
      pthread_mutex_unlock(&c_mutex);
      printf("9)  CCCCC  custmers getting cut, no longer has b_mutex lock\n");
      //getting cut
      cuts++;
      printf("\n I'm getting my hair cut! %d seats, %d threadid %d cuts done total, %d haircuts left for me \n",seats,(int)threadid,cuts,haircuts);
    }
    else{
      printf("\n NO SEATS I'll come back tomorrow :( thread %d \n",(int)threadid);
      pthread_mutex_unlock(&b_mutex);
      printf(" custmer no longer has mutex lock\n");
    }
    //    sleep(1);
  }
  pthread_exit(NULL);
}



int main(int argc, char *argv[])
{
  if(argc < 2 || argc > 3)
    printf("Wrong number of arguments: Usage > barber.exe x \n");
  seats = atoi(argv[1]);
  max_seats = seats;
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

