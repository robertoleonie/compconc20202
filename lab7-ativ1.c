/* Disciplina: Computacao Concorrente
Autor: Roberto Leonie Ferreira Moreira
Prof.: Silvana Rossetto
Laboratório: 7
Atividade 1: Projetar e implementar um programa concorrente onde a ordem de execucao
das threads e controlada no programa.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4



sem_t semaforo, semaforo2;

void* tarefa1 (void *t){
	int id = 1;
	sem_wait(&semaforo2);
	printf("%d: Volte sempre!\n", id);
	pthread_exit(NULL);
}

void* tarefa2 (void *t){
	int id = 2;
	sem_wait(&semaforo);
	printf("%d: Fique a vontade.\n", id);
	sem_post(&semaforo2);
	pthread_exit(NULL);
}

void* tarefa3 (void *t){
	int id = 3;
	sem_wait(&semaforo);
	printf("%d: Sente-se por favor.\n", id);
	sem_post(&semaforo2);
	pthread_exit(NULL);
}

void* tarefa4 (void *t){
	int id = 4;
	printf("%d: Seja bem-vindo!\n", id);
	sem_post(&semaforo);
	sem_post(&semaforo);
	pthread_exit(NULL);
}

int main(void){
  int i; 
  pthread_t threads[NTHREADS];

  sem_init(&semaforo, 0, 0);
  sem_init(&semaforo2, 0, 0);

  /* Cria as threads */
  if(pthread_create(&threads[3], NULL, tarefa1, NULL)) exit(-1);
  if(pthread_create(&threads[2], NULL, tarefa2, NULL)) exit(-1);
  if(pthread_create(&threads[1], NULL, tarefa3, NULL)) exit(-1);
  if(pthread_create(&threads[0], NULL, tarefa4, NULL)) exit(-1);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    if(pthread_join(threads[i], NULL)) exit(-1);
  }

  pthread_exit(NULL);
}
