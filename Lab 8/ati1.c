/*
 * Atividade 1 do Laboratório 8
 * Aluno: Lucas Favilla Ferreira Alves da Silva
 * DRE: 119156518
 * Primeiro rodasse a thread B, dois ou a A ou a D, e por ultimo a C.
 * Na thread B eu chamei duas vezes o sem_post pra dar chance de acontecer as duas possibilidades de saidas,
 * se eu so chamasse uma vez em cada thread a ordem sempre(ou pelo menos todas as vezes que eu testei) sairia
 * a mesma.
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

//globais 
sem_t cond1, cond2, cond3;
void *A (void *t){
    sem_wait(&cond1);
    printf("Fique a vontade\n");
    sem_post(&cond2);
    pthread_exit(NULL);
}
void *B (void *t){
    printf("Seja bem-vindo\n");
    sem_post(&cond1);
    sem_post(&cond1);
    pthread_exit(NULL);
}
void *C (void *t){
    sem_wait(&cond2);
    sem_wait(&cond3);
    printf("Volte Sempre\n");
    pthread_exit(NULL);
}
void *D (void *t){
    sem_wait(&cond1);
    printf("Sente-se por favor\n");
    sem_post(&cond3);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]){
    int i;
    pthread_t threads[NTHREADS];
    //cria os sinais
    sem_init(&cond1,0,0);
    sem_init(&cond2,0,0);
    sem_init(&cond2,0,0);
    //cria as threads
    pthread_create(&threads[3], NULL, D, NULL);
    pthread_create(&threads[0], NULL, A, NULL);
    pthread_create(&threads[2], NULL, C, NULL);
    pthread_create(&threads[1], NULL, B, NULL);

    //join para a main não acabar sua execução antes das threads
    for(i=0;i<NTHREADS;i++){
        pthread_join(threads[i],NULL);
    }
    return 0;
}
