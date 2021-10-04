/*
 * Atividade 1 do Laboratório 9
 * Aluno: Lucas Favilla Ferreira Alves da Silva
 * DRE: 119156518
 * Problema produtores e consumidores com semáforos.
 * para mudar a quantidade no buffer, o numero de produtores e/ou o numero de consumidores,
 * basta mudar os defines abaixo.
 * devido a logica do problema apenas um consumidor consegue consumir por vez,
 * já que ele consome a produção toda.
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NBUFFER 10
#define NPRODUTORES 7 
#define NCONSUMIDORES 6

//globais 
int Buffer[NBUFFER];
int idP[NPRODUTORES];
int idC[NCONSUMIDORES];
int buf = 0;
int total = 0;
int certo = NBUFFER*(1+NBUFFER)/2;
sem_t prod, cons, aux, falha;
void *produt (void *t){
    int* id = (int*) t;
    while(1){
        sem_wait(&prod);//exclusao mutua para garantir que so um produtor muda uma informação compartilhada por vez
        if(buf == NBUFFER){//check de termino da produção
            buf = 0;
            sem_post(&cons);//libera o consumidor
            sem_wait(&aux);//espera o consumidor
        }
        else{
            //produção
            Buffer[buf] = buf+1;
            buf++;
            printf("P[%d] produzi o elemento %d\n", *id, buf);
        }
        sem_post(&prod);
    }
}
void *consum (void *t){
    int* id = (int*) t;
    while(1){
        sem_wait(&cons);//espera o sinal do produtor
        for(int i = 0; i<NBUFFER ; i++){
            total+=Buffer[i];//para check de corretude
            Buffer[i]=0;//consumo
        }
        if(total!=certo) {//check de corretude
            printf("C[%d] ERRO NA CONTA total:%d != certo:%d\n",*id, total , certo);
            sem_post(&falha);//como varia quem pode falhar, usar um sinal para termino cai bem.
            pthread_exit(NULL);
        }
        printf("C[%d] Recolhi, TOTAL = %d valor correto: %d\n\n",*id, total, certo);
        total = 0;
        sem_post(&aux);//libera o produtor pausado para consumo.
    }
}

int main (int argc, char *argv[]){
    pthread_t produtores[NPRODUTORES];
    pthread_t consumidores[NCONSUMIDORES];
    //cria os sinais
    sem_init(&prod,0,1);
    sem_init(&cons,0,0);
    sem_init(&aux,0,0);
    sem_init(&falha,0,0);
    //inicia os arrays;
    for(int i = 0; i<NBUFFER;i++){
        Buffer[i]=0;
    }
    for(int i = 0; i<NPRODUTORES;i++){
        idP[i]= i+1;
    }
    for(int i = 0; i<NCONSUMIDORES;i++){
        idC[i]= i+1;
    }
    
    //cria as threads
    for(int i = 0;i<NPRODUTORES;i++){
        pthread_create(&produtores[i], NULL, produt, (void*) (idP+i));
    }
    for(int i = 0; i<NCONSUMIDORES;i++){
        pthread_create(&consumidores[i], NULL, consum, (void*) (idC+i));
    }
    sem_wait(&falha);
    return 0;
}
