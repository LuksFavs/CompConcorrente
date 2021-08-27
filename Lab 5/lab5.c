/*
 * Laboratório 5 - Computação Concorrente
 * Aluno: Lucas Favilla Ferreira Alves da Silva | DRE:119156518
 *
 * Fazer soma dos membros do vetor e a cada interação substituir valores dentro dele por números aleatórios
 * entre 0 e 9.
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "time.h"

typedef struct{
    int id;
    int saida;
} IDENT;

/* Variaveis Globais*/
int n;
int *vetor;
IDENT* ids;
int block = 0;
/*variaveis de controle de sincronização*/
int som = 0;//contador de quantos ja terminaram a soma.
int esc = 0;//sinal para entrada da escrita.
int cont = 0;//contador de quantos ja mudaram as variaveis.
pthread_mutex_t mutex;
pthread_cond_t escrita, esperando;
/* Nao precisei fazer o check de leitura pois as barreiras nao 
 * deixam ter chance de se escrever e ler ao msm tempo*/

void barreiraSoma(int id, int vez){
    pthread_mutex_lock(&mutex);
        if(som == (n-1)){
            pthread_cond_broadcast(&esperando);
            som=0;
            printf("passamos soma %d\n\n", vez);
        } else{
            som++;
            printf("ID:%d vez:%d esperando outros somarem\n", id, vez);
            pthread_cond_wait(&esperando, &mutex);
        }
        
    pthread_mutex_unlock(&mutex);
    return;
}
//essa barreira difere da de cima pois foi uma tentativa de concertar
//um erro que estava acontecendo, consegui consertá-lo de outra maneira.
void barreiraEsc(int id, int vez){
    pthread_mutex_lock(&mutex);
    if(cont < (n-1)){ 
         cont++;
         printf("ID:%d vez:%d esperando outros escreverem\n", id, vez); 
         pthread_cond_wait(&esperando, &mutex);

   } else{
        pthread_cond_broadcast(&esperando);
        printf("passamos escrita %d\n\n", vez);
        cont=0;
    }
        
    pthread_mutex_unlock(&mutex);
    return;
}

void escInit(int id){
    pthread_mutex_lock(&mutex);
    while(esc>0){
        //printf("ID: %d na fila para escrever.\n", id);//descomentar para ver a fila da escrita em ação.
        pthread_cond_wait(&escrita, &mutex);
    }
    esc++;
    pthread_mutex_unlock(&mutex);
}
void escFim(int id){
    pthread_mutex_lock(&mutex);
    esc--;
    pthread_cond_signal(&escrita);
    //printf("ID: %d terminei de escrever\n", id);//descomentar para ver a fila da escrita em ação.
    pthread_mutex_unlock(&mutex);
}

void *soma(void *num){
    IDENT* nova = (IDENT*) num;
    int soma = 0;
    //int ant;
    for(int k = 0;k<n;k++){
        for(int j = 0;j<n;j++){
            soma+=*(vetor+j);
        }
        //printf("ID:%d, soma=%d\n", nova->id, soma);//descomentar para checar a soma de cada interação.
        barreiraSoma(nova->id, k+1);
        escInit(nova->id);
        //ant=nova->id;//descomentar para testar a setagem dos valores aleatorios.
        *(vetor+nova->id) = rand()%10;
        //printf("ant = %d, nova=%d\n", ant, *(vetor+nova->id));
        escFim(nova->id);
        barreiraEsc(nova->id, k+1);
    }
    nova->saida = soma;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    
    srand(time(NULL));//numero aleatorio;
    n = atoi(argv[1]);
    printf("N = %d\n", n); 
    if(n==0){
        free(vetor);
        free(ids);
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&esperando);
        pthread_cond_destroy(&escrita);
        printf("Terminando o programa pois N é nulo.\n");
        return 0;
    }
    vetor = (int*) malloc(n*sizeof(int));
    ids = (IDENT*)malloc(n*sizeof(IDENT));
    pthread_t *tid;
    tid=(pthread_t*) malloc(sizeof(pthread_t)*n);
    
    if(!vetor || !ids || !tid){
        printf("ERRO NA ALOCAÇÃO DO VETOR ou das IDs ou do tid\n");
        return 1;
    }
    for(int i=0;i<n;i++){
        *(vetor+i) = rand()%10;
        //printf("vetor + %d = %d\n", i, *(vetor+i));//para ver o vetor
    }

    for(int i=0;i<n;i++){
        (ids+i)->id = i;
        if(pthread_create(tid+i, NULL, soma, (void*)(ids+i))){
            puts("erro na pthread_create");
            return 2;
        }
    }

    for(int i=0;i<n;i++){
        pthread_join(*(tid+i), NULL);
    }
    for(int i=0;i<n;i++){
        printf("ID:%d soma = %d\n",(ids+i)->id+1,(ids+i)->saida); 
    }
    for(int i=0;i<n-1;i++){
        if(ids->saida != (ids+1)->saida){
            printf("houve erro:\n%d != %d\n", ids->saida, (ids+1)->saida);
            return 3;
        }else if(i==(n-2)){
            printf("DEU CERTO, soma = %d\n",ids->saida); 
        }
    }
    
    free(vetor);
    free(tid);
    free(ids);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&esperando);
    pthread_cond_destroy(&escrita);
    return 0;
}
