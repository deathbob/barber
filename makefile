all:
	gcc barber.c -o barber.exe -lm -g -Wall -lpthread
sem:
	gcc barber_sem.c -o barber_sem.exe -lm -g -Wall -lpthread