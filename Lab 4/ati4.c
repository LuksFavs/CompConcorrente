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
int estado = 0, est = 0, rodei = 0;
//est regula qual broadcast e chamado por qm, foi a saida q eu encontrei para fazer que o 3 sempre sair no final.
//"rodei" era a tentativa de parar o travamento exemplificado no formulario.
//por alguma razão qndo eu descomento os prints "rodei" o programa funciona perfeitamente(???),
//isso me leva a crer q seja um problema na concorrencia da função printf, entretanto não soube concerta-la. 
pthread_mutex_t x_mutex;
pthread_mutex_t y_mutex;
pthread_cond_t rod;
pthread_cond_t x_cond;
pthread_cond_t y_cond;
pthread_cond_t z_cond;

void *A (void *t){
    pthread_mutex_lock(&x_mutex);
        if(estado < 1){
            //printf("1 esperando\n");
            pthread_cond_wait(&y_cond, &x_mutex);
        }
        printf("Fique a vontade\n");
        if(rodei == 0) pthread_cond_wait(&rod, &x_mutex);
        if(est==0){
            pthread_cond_broadcast(&x_cond);
            //printf("rodei a");
            est = 1;
        }
        else {
            pthread_cond_broadcast(&z_cond);
            //printf("rodei a2");
        }
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
        rodei = 1; // tentativa de concerto do travamento
        pthread_cond_broadcast(&rod);
        pthread_cond_wait(&x_cond, &y_mutex);
        //printf("esperando z\n");
        pthread_cond_wait(&z_cond, &y_mutex);
        //printf("passei\n");
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
        if(rodei == 0) pthread_cond_wait(&rod, &x_mutex);
        if(est==1) {
            pthread_cond_broadcast(&z_cond);
            //printf("rodei d");
        }
        else {
            pthread_cond_broadcast(&x_cond);
            //printf("rodei d2");
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
    pthread_cond_init(&rod, NULL);
    pthread_cond_init(&x_cond, NULL);
    pthread_cond_init(&y_cond, NULL);
    pthread_cond_init(&z_cond, NULL);

    //cria as threads
    pthread_create(&threads[2], NULL, C, NULL); //tentativa de resolver a problematica
    pthread_create(&threads[3], NULL, D, NULL);
    pthread_create(&threads[0], NULL, A, NULL);
    pthread_create(&threads[1], NULL, B, NULL);

    //join para a main não acabar sua execução antes das threads
    for(i=0;i<NTHREADS;i++){
        pthread_join(threads[i],NULL);
    }
    pthread_mutex_destroy(&x_mutex);
    pthread_mutex_destroy(&y_mutex);
    pthread_cond_destroy(&rod);
    pthread_cond_destroy(&x_cond);
    pthread_cond_destroy(&y_cond);
    pthread_cond_destroy(&z_cond);
}
