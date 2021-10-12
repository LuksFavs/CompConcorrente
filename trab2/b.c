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
#include"timer.h"


int disponivel[10]={0,0,0,0,0,0,0,0,0,0};
int fim=0;
int* buffer;
int nthreads;
int total;
int N;
int nblocos;
char* entrada;
char* saida;

pthread_mutex_t lock;
pthread_cond_t cons;
pthread_cond_t parada;
sem_t scom;
sem_t sesc;
sem_t sprod;
sem_t x;

void *produtor(){

    //file
    FILE *file;
    size_t buf = 0;
    int termino = 0;
    char* linha=NULL;
    
    
    if(!(file=fopen(entrada, "r"))){
        printf("erro abrindo o arquivo\n");
        pthread_exit(NULL);
    }
    //logica
    getline(&linha, &buf, file);
    total = atoi(linha);
    if(!(buffer = (int*) malloc(10*N*sizeof(int)))){
        printf("erro allocando o buffer");
        pthread_exit(NULL);
    }

    if((total%N)!=0) nblocos=(total/N)+1;//se o total de elementos não for multiplo de N, o último bloco possuirá o resto da divisão
    else nblocos=total/N;
    if(nblocos<nthreads) {
        printf("número de threads reduzido para %d para não causar erros\n", nblocos);
        nthreads = nblocos;
    }

    int processados=0;// numero de elementos já lidos
    while(1){
        for (int i=0; i<10;i++){
            if (disponivel[i]==0){//não precisa de exclusão pois nenhuma das trheads muda o valor de 0 para outro, apenas valores que não são 0 para 0, então não corre risco de conflito.
                for(int j=0;j<N;j++){
                    if(processados==total){//se já tiverem sido percorridos todos os elementos, para o for
                        //puts("stop");
                        termino = 1;//variavel q alerta a produtora que acabou
                        break;
                    }
                    if(j<N-1)getdelim(&linha,&buf, 32, file);//lê até o espaço se não for o último
                    else getdelim(&linha,&buf, 10, file);//lê até o \n se for
                    buffer[j+i*N] = atoi(linha);
                    processados++;
                }
                //sincronizar
                pthread_mutex_lock(&lock);
                disponivel[i]=1;//alerta que o blooc está disponivel para consumo
                pthread_mutex_unlock(&lock);
                sem_post(&scom);//libera consumidor
            }
            // else{
            //     // printf("bloco em operação %d \n ", i);
            //     //passa para o próximo
            // }
            while(termino){
                //verificação do controlador do buffer.
                pthread_mutex_lock(&lock);
                for(int k=0;k<10;k++){//procura blocos que ainda não estejam processados ou em processamento.
                    while(disponivel[k]!=0){
                        //printf("a espera da &parada\n");
                        pthread_cond_wait(&parada,&lock);//caso ainda tenha algum bloco em operação fica em espera
                    }
                }
                fim=1;//atualiza a variavel de alerta de fim
                pthread_mutex_unlock(&lock);
                //printf("saindo produtor\n");
                sem_post(&scom);
                pthread_exit(NULL);
            }
        }
    }
}
void *consumidor(void* t){
    int id = (int) t;
    int inicio;
    //Quantos blocos cada thread processa
    
    while(1){   
        sem_wait(&scom);
        //printf("passei %d\n", id);
        pthread_mutex_lock(&lock);
        if(fim){//encerra a execução
            pthread_mutex_unlock(&lock);
            sem_post(&sesc);
            sem_post(&scom);//caso seja o fim libera as outras threads para elas percorrem
            //printf("saindoconsumidor %d\n", id);
            pthread_exit(NULL);
        }
        for (int i=0; i<10;i++){
            if(disponivel[i]==1){
                disponivel[i]=2;//avisa que o bloco está em processamento
                inicio=N*i;//calcula o indice de inicio do bloco no buffer
                break;
            }
        }
        

        pthread_mutex_unlock(&lock);
        int qtdblocos=nblocos/nthreads;
        if((nblocos%nthreads)!=0 && id==nthreads-1) qtdblocos+=1;
        
        for (int k = 0; k < N; k++) {
            for (int j = inicio; j < inicio + N - k - 1; j++) {
                 if(j+1==total) {//faz parar caso já tenha chegado ao último elemento do buffer
                        //printf("Chegou ao fim %d\n\n", j);
                    break;
                }
                else if (buffer[j] > buffer[j + 1]) {//bubble sort organiza o bloco
                    int aux = buffer[j];
                    buffer[j] = buffer[j + 1];
                    buffer[j + 1] = aux;
                }
            }
        }
        sem_post(&sesc);//alerta ao escritor que tem um bloco disponivel para escrita
    }
}
void *escritor(void* T){
    int id= (int) T;
    int inicio;
    while(1){
        sem_wait(&sesc);
        pthread_mutex_lock(&lock);//garante exclusividade de execução, para variaveis fim e outros escritores.

        if(fim){//encerra a execução
            //puts("entrei");
            //printf("saindo escritor %d\n", id);
            pthread_mutex_unlock(&lock);
            sem_post(&sesc);//caso seja o fim libera as outras threads para elas percorrem
            pthread_exit(NULL);
            //AAAAAAAAAAAAAAAAAAAAAAAAA
        }
        for (int i=0; i<10;i++){//procura o bloco que está disponivel para escrita
            if(disponivel[i]==2){
                inicio=N*i;
                //printf("%d, %d", disponivel[i],inicio);
                //printf("destravando parada &parada\n");
                pthread_cond_signal(&parada);
                break;
            }
        }
        
        //escreve o o bloco no arquivo de saída
        FILE* pasta;
        if(!(pasta=fopen(saida, "a"))){
            printf("erro abrindo o arquivo\n");
            pthread_exit(NULL);
        } 
        for(int i = inicio; i<inicio+N; i++){
            if(i>=total) break;
            fprintf(pasta,"%d", buffer[i]);
            if((i==inicio+N-1)||(i+1==total)) fprintf(pasta,"\n");
            else fprintf(pasta, " ");
        }
        disponivel[inicio/N]=0;
        fclose(pasta);
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char* argv[]){
    //parametros para o timer.
    double start1,finish1, elapsed1;
    GET_TIME(start1);
    //parametros do problema
    nthreads = atoi(argv[1]);
    N = atoi(argv[2]);
    entrada = argv[3];
    saida = argv[4];
    
    //inicialização das threads
    pthread_t prod;
    pthread_t consum[nthreads];
    pthread_t escri[nthreads];
    
    //inicialização dos sinais
     sem_init(&sprod, 0, 0);
    sem_init(&scom, 0, 0);
    sem_init(&sesc, 0, 0);
    sem_init(&x, 0, 1);
    
    //se ja existir ele apaga o que tem dentro;
    FILE *pf;
    if(!(pf=fopen(saida, "w"))){
        printf("falha ao criar o arquivo de saida");
        return 0;
    }
    fclose(pf);

    pthread_create(&prod, NULL, produtor, NULL);
     for(int i=0; i<nthreads;i++) {
         pthread_create(&consum[i], NULL, consumidor, (void*)(i));
         pthread_create(&escri[i], NULL, escritor, (void*)(i));
     }
        
    
    pthread_join(prod, NULL);
    // printf("passei prod\n");
     for(int i=0; i<nthreads;i++) pthread_join(consum[i], NULL);
    // printf("passei consumidores\n");
     for(int i=0; i<nthreads;i++) pthread_join(escri[i], NULL);
    // printf("passei escritores\n");
    /*for (int i=0; i<10;i++){
        for(int j=0;j<N;j++){
            printf("[%d] = %d", j, buffer[j+i*N]);
        }
    printf("\n");
    }*/
         
    printf("\n");
    GET_TIME(finish1);
    elapsed1=finish1-start1;
    printf("tempo total: %f\n", elapsed1);
    sem_close(&scom);
    sem_close(&sesc);
    sem_close(&x);
    return 0;
}
