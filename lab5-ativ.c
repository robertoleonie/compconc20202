/* Disciplina: Computacao Concorrente */

/* Prof.: Silvana Rossetto */

/* Aluno: Roberto Leonie Ferreira Moreira */

/* Laboratorio 5 - Atividade1.c */



#include <pthread.h>

#include <stdio.h>

#include <stdlib.h>

#include <math.h>



double log2(double x){

	return log(x) / log(2);

}



int nThreads;

int *vetor;

int aux;		// variavel auxiliar global



/* Variaveis globais */

int bloqueadas = 0;

pthread_mutex_t x_mutex;

pthread_cond_t x_cond;



//funcao barreira

void barreira(){
	pthread_mutex_lock(&x_mutex);
	bloqueadas++;
	if(bloqueadas < nThreads) pthread_cond_wait(&x_cond, &x_mutex);
	else {
		bloqueadas = 0;
		pthread_cond_broadcast(&x_cond);
	}
	pthread_mutex_unlock(&x_mutex);
}



//funcao das threads

void *tarefa(void *arg){
	int id = (int) arg;
	int aux;

	for(int i = 1; i < nThreads; i *= 2){
		int j = id - i;
		if(j >= 0) aux = vetor[j];

		barreira();

		if(j >= 0) vetor[id] += aux;

		barreira();
	}



  	free(arg);

  	pthread_exit(NULL);

}



/* Funcao principal */

int main(int argc, char *argv[]) {

  pthread_t threads[nThreads];	// criacao de threads

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */

  pthread_mutex_init(&x_mutex, NULL);

  pthread_cond_init(&x_cond, NULL);



  vetor = (int *) malloc(nThreads * sizeof(int));

//  vetor = {1, 4, -1, 7};	// termos esperados



	if(argc != 2) {

		printf("%s <nThreads>\n", argv[0]);

		exit(-1);

	} else {

		nThreads = atoi(argv[1]);

	}



  printf("Entre com os elementos do vetor de %d posicoes: ", nThreads);

  for(int i = 0; i < nThreads; i++){

  	scanf("%d", &vetor[i]);

  }



  /* Cria as threads */

  for(int i=0;i<nThreads;i++) {

     if(pthread_create(&threads[i], NULL, tarefa, (void *) i+1)){

     	printf("--ERRO: pthread_create\n");

     	exit(-1);

     }

  }



  /* Espera todas as threads completarem */

  for (int i = 0; i < nThreads; i++) {

    if(pthread_join(threads[i], NULL)){

     	printf("--ERRO: pthread_join\n");

     	exit(-1);

    }

  }



	//printf("Imprimindo o array final.\n");

	//printf("[");

	//for(int i = 0; i < nThreads; i++){

	//	printf("%d", vetor[i]);

	//	if(i != nThreads-1) printf(", ");

	//}

	//printf("]\n");



  /* Desaloca variaveis e termina */

  pthread_mutex_destroy(&x_mutex);

  pthread_cond_destroy(&x_cond);

  return 0;

}
