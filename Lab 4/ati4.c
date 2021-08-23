/*
 * Atividade 4 do Laboratório 4
 * Aluno: Lucas Favilla Ferreira Alves da Silva
 * DRE: 119156518
 * 
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

//globais 
int estado = 0, est = 0;
pthread_mutex_t x_mutex;
pthread_mutex_t y_mutex;
pthread_cond_t x_cond;
pthread_cond_t y_cond;
pthread_cond_t z_cond;
//2 antes de 1 ou 4
//1 e 4 antes da 3
//2 / 1 ou 4 / 3 
void *A (void *t){
    pthread_mutex_lock(&x_mutex);
        if(estado < 1){
            //printf("1 esperando\n");
            pthread_cond_wait(&y_cond, &x_mutex);
            printf("Fique a vontade\n");
        }
        if(est==0){
            pthread_cond_broadcast(&x_cond);
            est = 1;
        }
        else pthread_cond_broadcast(&z_cond);

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}
void *B (void *t){
    printf("Seja bem-vindo\n");
    pthread_mutex_lock(&x_mutex);
        estado++;
        pthread_cond_broadcast(&y_cond);
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}
void *C (void *t){
    pthread_mutex_lock(&y_mutex);
        pthread_cond_wait(&x_cond, &y_mutex);
        pthread_cond_wait(&z_cond, &y_mutex);
    pthread_mutex_unlock(&y_mutex);
    printf("Volte Sempre\n");
    pthread_exit(NULL);
}
void *D (void *t){
    pthread_mutex_lock(&x_mutex);
        if(estado < 1){
            //printf("4 esperando\n");
            pthread_cond_wait(&y_cond, &x_mutex);
        }
        printf("Sente-se por favor\n");
        if(est==1)pthread_cond_broadcast(&z_cond);
        else {
            pthread_cond_broadcast(&x_cond);
            est=1;
        }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]){
    int i;
    pthread_t threads[NTHREADS];

    //inicializa os mutex e a condicao
    pthread_mutex_init(&x_mutex, NULL);
    pthread_mutex_init(&y_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);
    pthread_cond_init(&y_cond, NULL);
    pthread_cond_init(&z_cond, NULL);

    //cria as threads
    pthread_create(&threads[0], NULL, A, NULL);
    pthread_create(&threads[1], NULL, B, NULL);
    pthread_create(&threads[2], NULL, C, NULL);
    pthread_create(&threads[3], NULL, D, NULL);

    //join para a main não acabar sua execução antes das threads
    for(i=0;i<NTHREADS;i++){
        pthread_join(threads[i],NULL);
    }
    pthread_mutex_destroy(&x_mutex);
    pthread_mutex_destroy(&y_mutex);
    pthread_cond_destroy(&x_cond);
    pthread_cond_destroy(&y_cond);
    pthread_cond_destroy(&z_cond);
}
