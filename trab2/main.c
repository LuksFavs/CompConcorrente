/**
 * Segundo Trabalho de Implementação
 * Aluno: Lucas Favilla Ferreira Alves da Silva DRE:119156518
 * Aluno: Bruno Llacer Trotti                   DRE:119169008
 *
 * Main do programa.
 * */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>

int* buffer;
char* entrada;
char* saida;
int N;
int total;
int nthreads;
int* ids;
int cabo = 0;
int terminados =0;
//semáforos
sem_t sescr;
sem_t scom;
sem_t sprod;
sem_t x;

void *produtor(){

    //file
    FILE *file;
    size_t buf = 0;
    char* linha=NULL;
    
    if(!(file=fopen(entrada, "r"))){
        printf("erro abrindo o arquivo\n");
        pthread_exit(NULL);
    }
    //logica
    getline(&linha, &buf, file);
    total = atoi(linha);
    
    if(!(buffer=malloc(10*N*sizeof(int)))){
        printf("erro allocando o buffer");
        pthread_exit(NULL);
    }
    int aux2;
    if((total%N)!=0) aux2=total/N+1;
    else aux2=total/N;
    
    for(int i=0; i<aux2;i++){ 
        for(int j=0;j<N;j++){
            if(j+i*N>=total){ 
                break;
            }
            if(j<N-1)getdelim(&linha,&buf, 32, file);
            else getdelim(&linha,&buf, 10, file);
            buffer[j+i*N] = atoi(linha);
        }
        //tira o depois do ou qndo for mandar
        //so fiz pra dar o caso de 20 num com 5 caras por bloco.
        if((i>0 && i%10==0) || (i == (total/N-1))){
            for(int i=0;i<nthreads;i++) {
                sem_post(&scom);
                printf("mandei\n");
            }
            printf("esperando produtor\n");
            sem_wait(&sprod);
            printf("saindo produtor\n");
        }
    }
    cabo=1;
    for(int i=0;i<nthreads;i++){
        sem_post(&scom);
        sem_post(&sescr);
    }
    pthread_exit(NULL);
}

void *consumidor(void* t){
    int* id = (int*) t;
    while(!cabo){
        sem_wait(&scom);
        printf("dentro %d\n", *id);
        if(cabo==1) pthread_exit(NULL);
        int aux2;
        if(total%N!=0)aux2=total/N+1;
        else aux2=total/N;
        if((aux2%nthreads)!=0) aux2=aux2/nthreads+1;
        else aux2=aux2/nthreads;
        printf("%d\n\n\n", aux2);
        int inicio;
        for(int i=0;i<aux2;i++){
            for (int k = 1; k < N; k++) {
                for (int j = 0; j < N - 1; j++) {
                    inicio = j+N*(*id)+i*nthreads*N;
                    if(inicio+1==total) {
                        printf("%d\n\n", inicio);
                    break;}
                    else if (buffer[inicio] > buffer[inicio + 1]) {
                        int aux = buffer[inicio];
                        buffer[inicio] = buffer[inicio + 1];
                        buffer[inicio + 1] = aux;
                    }
                }
            }
        }
        sem_wait(&x);
        terminados++;
        if(terminados==nthreads){
            terminados=0;
            for(int i=0; i<nthreads;i++) sem_post(&sescr);
            printf("mandei escr\n");
        }
        sem_post(&x);
        printf("saindo consumidor %d", *id);
    }pthread_exit(NULL);
}

void *escritor(void* T){
    int* id = (int*)T;
    while(!cabo){
        printf("esperadno escritor\n");
        sem_wait(&sescr);
        if(cabo==1) pthread_exit(NULL);
        sem_wait(&x);
        FILE* pasta;
        if(!(pasta=fopen(saida, "a"))){
            printf("erro abrindo o arquivo\n");
            pthread_exit(NULL);
        } 
        int inicio;
        int aux2=total;
        if((aux2%nthreads)!=0) aux2=aux2/nthreads+1;
        else aux2=aux2/nthreads;
        for(int i=0;i<aux2;i++){
            for (int j = 0; j < N; j++) {
                inicio = j+N*(*id)+i*nthreads*N;
                if(inicio>=total) break;
                fprintf(pasta,"%d", buffer[inicio]);
                if((j==N-1)||(inicio+1==total)) fprintf(pasta,"\n");
                else fprintf(pasta, " ");
            }
        }
        fclose(pasta);
        terminados++;
        if(terminados == nthreads){
            terminados = 0;
            sem_post(&sprod);
        }
        sem_post(&x);
    }pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    //parametros do problema
    nthreads = atoi(argv[1]);
    N = atoi(argv[2]);
    entrada = argv[3];
    saida = argv[4];
    ids=malloc(sizeof(int)*nthreads);
    //inicialização das threads
    pthread_t prod;
    pthread_t consum[nthreads];
    pthread_t escri[nthreads];
    //inicialização dos sinais
    sem_init(&sprod, 0, 0);
    sem_init(&scom, 0, 0);
    sem_init(&sescr, 0, 0);
    sem_init(&x, 0, 1);
    
    pthread_create(&prod, NULL, produtor, NULL);
    for(int i=0; i<nthreads;i++) {
        ids[i]=i;
        pthread_create(&consum[i], NULL, consumidor, (void*)(ids+i));
    }
    for(int i=0; i<nthreads;i++) {
        ids[i]=i;
        pthread_create(&escri[i], NULL, escritor, (void*)(ids+i));
    }
        
    
    pthread_join(prod, NULL);
    printf("passei prod\n");
    for(int i=0; i<nthreads;i++) pthread_join(consum[i], NULL);
    printf("passei consumidores\n");
    for(int i=0; i<nthreads;i++) pthread_join(escri[i], NULL);
    printf("passei escritores\n");
    for(int i=0; i<total; i++) {
        if(i%(N)==0 && i>0) printf("\n");
        printf("[%d]: %d",i, buffer[i]);
    }
    printf("\n");
    return 0;
}























