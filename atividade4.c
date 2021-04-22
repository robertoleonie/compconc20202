/* Disciplina: Computacao Concorrente */
/* Autor: Roberto Leonie Ferreira Moreira */
/* Laboratório: 4 */
/* Atividade 4: A thread 4 deve sempre imprimir sua mensagem antes das threads 2 e 3 
(ou 3 e 2) e estas imprimem antes da thread 1*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4

/* Variaveis globais */
pthread_mutex_t mutex;
pthread_cond_t cond, cond2;

void* tarefa1 (void *t){
	int id = 1;
	pthread_cond_wait(&cond2, &mutex);
	printf("%d: Volte sempre!\n", id);

	pthread_exit(NULL);
	
}

void* tarefa2 (void *t){
	int id = 2;
	pthread_cond_wait(&cond, &mutex);
	printf("%d: Fique a vontade.\n", id);
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond2);
	
	pthread_exit(NULL);
	
}

void* tarefa3 (void *t){
	int id = 3;
	pthread_cond_wait(&cond, &mutex);
	printf("%d: Sente-se por favor.\n", id);
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond2);
	
	pthread_exit(NULL);
	
}

void* tarefa4 (void *t){
	int id = 4;
	pthread_mutex_lock(&mutex);
	printf("%d: Seja bem-vindo!\n", id);
	pthread_mutex_unlock(&mutex);
	pthread_cond_broadcast(&cond);
	
	pthread_exit(NULL);
	
}

int main(void){
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init (&cond, NULL);
  pthread_cond_init (&cond2, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, tarefa1, NULL);
  pthread_create(&threads[1], NULL, tarefa2, NULL);
  pthread_create(&threads[2], NULL, tarefa3, NULL);
  pthread_create(&threads[3], NULL, tarefa4, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  pthread_cond_destroy(&cond2);
}
