/* Computacao Concorrente - DCC/UFRJ - 2020/2 (REMOTO)
Prof. Silvana Rossetto
Aluno: Roberto Leonie Ferreira Moreira
Modulo 2 - Semana 3 - Laboratorio 6 */

class SomaVetor {
    private static int tamVetor;

    // recurso a ser compartilhado
    public Integer[] vetor;

    SomaVetor(int tamVetor){
        this.tamVetor = tamVetor;
        this.vetor = new Integer[tamVetor];
    }

    public int getTamVetor(){
        return this.tamVetor;
    }

    public synchronized void incrementa(int start, int stop, int step){
        for(int i = start; i < stop; i+=step){
            this.vetor[i] += 1;
            System.out.println(String.format("Pos-incremento (fim) vetor[%d] = %d", i, vetor[i]));
        }
    }

    public synchronized Integer[] getVetor(){
        return this.vetor;
    }
}

class ExecutaThread extends Thread {
    private int tid;
    SomaVetor somaVetor;
    int numThreads;

    ExecutaThread(int tid, SomaVetor somaVetor, int numThreads){
        this.tid = tid;
        this.somaVetor = somaVetor;
        this.numThreads = numThreads;
    }

    public void run(){
        //int periodo = somaVetor.getTamVetor()/this.numThreads;
        System.out.println(String.format("Thread %d iniciando", this.tid));
        somaVetor.incrementa(this.tid, somaVetor.getTamVetor(), this.numThreads);
        System.out.println(String.format("Thread %d encerrada", this.tid));
    }
}

public class Atividade {

    public static void main (String[] args){
        int numThreads = 4; // o numero de threads
        SomaVetor somaVetor = new SomaVetor(10);    // array de 10 posicoes
        Thread[] threads = new Thread[numThreads];

        if(numThreads > somaVetor.getTamVetor()) return;

        for(int i = 0; i < somaVetor.getTamVetor(); i++){
            somaVetor.vetor[i] = i;
            System.out.println((String.format("Antes do incremento (inicio): vetor[%d] = %d", i, somaVetor.vetor[i])));
        }

        for(int i = 0; i < threads.length; i++){
            threads[i] = new ExecutaThread(i, somaVetor, numThreads);
        }

        for(int i = 0; i < threads.length; i++){
            threads[i].start();
        }

        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        System.out.println("\nImprimindo elementos do array sequencialmente...");
        for(int i = 0; i < somaVetor.getTamVetor(); i++){
            System.out.println((String.format("vetor[%d] = %d", i, somaVetor.vetor[i])));
        }

        System.out.println("Encerrando thread principal.");
    }
}
