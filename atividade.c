//Soma todos os elementos de um vetor de inteiro
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include "timer.h"

long long int N;	//dimensao do vetor de entrada
int nthreads;	//numero de threads

//fluxo das threads
void *tarefa(void *arg){
	int id = (int) arg;	//identificador da thread
	double *somaLocal;	//variavel local da soma de elementos
	long long int tamBloco = N/nthreads;	//tamanho do bloco de cada thread
	long long int ini = id * tamBloco;	//elemento inicial do bloco da thread
	long long int fim;	//elemento final(nao processado) do bloco da thread
	
	somaLocal = (double *) malloc(sizeof(double));
	if(somaLocal == NULL) {
		fprintf(stderr, "Erro--malloc\n");	exit(1);
	}
	*somaLocal = 0;

	if(id == nthreads-1) fim = N;
	else fim = ini + tamBloco;	//trata o resto se houver
	//soma os elementos do bloco da thread
	for(long long int i=ini; i<fim; i++)
		*somaLocal += pow(-1,i)/(2*i+1);
	*somaLocal *= 4;
	//retorna o resultado da soma local
	pthread_exit((void *) somaLocal);
}

//fluxo principal
int main(int argc, char *argv[]) {
	double somaPiSeq = 0;	//soma sequencial
	double somaPiConc = 0;	//soma concorrente
	double ini, fim;	//tomada de tempo
	pthread_t *tid;		//identificadores das threads no sistema
	double *retorno;		//valor de retorno das threads
	double epsilonSeq;		// quanto que eu errei calculando pi no sequencial
	double epsilonConc;		// quanto que eu errei calculando pi na concorrencia
	
	//recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
	if(argc < 2) {
		fprintf(stderr, "Digite: %s <N> <nThreads>\n", argv[0]);
		return 1;
	}
	N = atoll(argv[1]);
	nthreads = atoi(argv[2]);

	//soma sequencial dos elementos
	GET_TIME(ini);
	for(long long int i=0; i<N; i++)
		somaPiSeq += pow(-1,i)/(2*i+1);
	somaPiSeq *= 4;
	epsilonSeq = fabs(M_PI - somaPiSeq);
	GET_TIME(fim);

	// ATIVIDADE 1 (ENCONTRAR PI COM 1 THREAD - SEQUENCIAL)
	printf("Valor pi encontrado (sequencial): %.15lf\n", somaPiSeq);
	printf("Erro epsilon: %.15lf\n", epsilonSeq);
	printf("Tempo sequencial: %lf\n", fim-ini);
	
	//soma concorrente dos elementos
	GET_TIME(ini);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid==NULL) {
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}
	//criar as threads
	for(int i=0; i<nthreads; i++){
		if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
			fprintf(stderr, "ERRO--pthread_create\n");
			return 3;
		}
	}
	//aguardar o termino das threads
	for(int i=0; i<nthreads; i++){
		if(pthread_join(*(tid+i), (void**) &retorno)){
			fprintf(stderr, "ERRO--pthread_join\n");
			return 3;
		}
		//soma global
		somaPiConc += *retorno;
		free(retorno);
	}
	epsilonConc = fabs(M_PI - somaPiConc);
	GET_TIME(fim);

	// ATIVIDADE 2 (ENCONTRAR PI COM MAIS DE 1 THREAD - CONCORRENCIA)
	printf("\n\nValor pi encontrado (concorrente): %.15lf\n", somaPiConc);
	printf("Erro epsilon: %.15lf\n", epsilonConc);
	printf("Tempo concorrente: %lf\n", fim-ini);

	if(epsilonConc > epsilonSeq) printf("\n\nPROGRAMACAO SEQUENCIAL CHEGOU MAIS PERTO DE PI\n");
	else if(epsilonConc < epsilonSeq) printf("\n\nPROGRAMACAO CONCORRENTE CHEGOU MAIS PERTO DE PI\n");

	//libera as areas de memoria alocadas
	free(tid);
	
	return 0;
}
