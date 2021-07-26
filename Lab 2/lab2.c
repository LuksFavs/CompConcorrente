/*Multiplicação matriz-matriz*/
/*Aluno: Lucas Favilla Ferreira Alves da Silva*/
/*DRE:119156518*/
/*para rodar passe: ./... <número de threads> <tamanho da matriz>*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

float *mat1;
float *mat2;
float *saida;
int dim;
int nthreads;

typedef struct{
    int id;
} TD;//ident da thread;

void * multi(void * iden){
    TD* identifica = (TD*) iden;
    if(nthreads==1){
        printf("entrei 1\n");
        for(int i=0;i<dim;i++){
            for(int j=0;j<dim;j++){
                for(int k=0;k<dim;k++){
                    saida[i*dim+j]+=mat1[i*dim+k]*mat2[k*dim+j];
                }
            }
        }
    }
    if(nthreads==2){
        printf("entrei 2\n");
        if(identifica->id==0){
            for(int i=0;i<dim;i+=2){
                for(int j=0;j<dim;j++){
                    for(int k=0;k<dim;k++){
                        saida[i*dim+j]+=mat1[i*dim+k]*mat2[k*dim+j];
                    }
                }
            }
        }
        if(identifica->id==1){
            for(int i=1;i<dim;i+=2){
                for(int j=0;j<dim;j++){
                    for(int k=0;k<dim;k++){
                        saida[i*dim+j]+=mat1[i*dim+k]*mat2[k*dim+j];
                    }
                }
            }
        }
    }
    if(nthreads==4){
        printf("entrei 4\n");
        if(identifica->id==0){
            for(int i=0;i<dim;i+=4){
                for(int j=0;j<dim;j++){
                    for(int k=0;k<dim;k++){
                        saida[i*dim+j]+=mat1[i*dim+k]*mat2[k*dim+j];
                    }
                }
            }
        }
        if(identifica->id==1){
            for(int i=1;i<dim;i+=4){
                for(int j=0;j<dim;j++){
                    for(int k=0;k<dim;k++){
                        saida[i*dim+j]+=mat1[i*dim+k]*mat2[k*dim+j];
                    }
                }
            }
        }
        if(identifica->id==2){
            for(int i=2;i<dim;i+=4){
                for(int j=0;j<dim;j++){
                    for(int k=0;k<dim;k++){
                        saida[i*dim+j]+=mat1[i*dim+k]*mat2[k*dim+j];
                    }
                }
            }
        }
        if(identifica->id==3){
            for(int i=3;i<dim;i+=4){
                for(int j=0;j<dim;j++){
                    for(int k=0;k<dim;k++){
                        saida[i*dim+j]+=mat1[i*dim+k]*mat2[k*dim+j];
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]){
    double start1,start2,start3,finish1,finish2,finish3, elapsed1,elapsed2,elapsed3;
    GET_TIME(start1);
    /*leitura dos parametros de entrada e testes dos mesmos*/
    if(argc !=3){
        printf("ERRO-PASSAGEM DE PARAMETROS INCORRETOS\nUse o modo\"./... <número de threads> <tamanho da matriz>\"\n");
        return 1;
    }
    dim = atoi(argv[2]);
    nthreads = atoi(argv[1]);
    pthread_t *tid;
    TD *ident;

    /*alocação de memoria para as matrizes e checks de erro*/
    if(!(mat1=malloc((dim*dim)*sizeof(float)))){
        printf("ERRO-FALHA NA ALOCAÇÃO DA PRIMERIA MATRIZ\n");
        return 2;
    }
    if(!(mat2=malloc((dim*dim)*sizeof(float)))){
        printf("ERRO-FALHA NA ALOCAÇÃO DA SEGUNDA MATRIZ\n");
        return 2;
    }
    if(!(saida=malloc((dim*dim)*sizeof(int)))){
        printf("ERRO-FALHA NA ALOCAÇÃO DA SAIDA\n");
        return 2;
    }
    
    /*inicialização das estruturas*/
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            mat1[(i*dim)+j]= 1;
            mat2[(i*dim)+j]= 1;
            saida[(i*dim)+j]= 0;
        }
    }
    GET_TIME(finish1);
    GET_TIME(start2);
    tid=(pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL){printf("ERRO-Criação de threads\n");return 2;}
    ident=(TD*) malloc(sizeof(TD)*nthreads);
    if(ident==NULL){printf("ERRO-Criação de threads\n");return 2;}
    /*criação das threads e multiplicação*/
    for(int i=0;i<nthreads;i++){
        (ident+i)->id = i;
        if(pthread_create(tid+i,NULL,multi,(void*)(ident+i))){
            puts("ERRO-pthread_create\n");return 4;
        }
    }
    
    /*verificação*/
    for(int i=0;i<nthreads;i++){
        pthread_join(*(tid+i),NULL);
    }
    GET_TIME(finish2);
    GET_TIME(start3);
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            if(!(saida[(i*dim)+j] == dim)){
                printf("erro na multi\n");
                return 3;
            }
        }
    }
    
    /*retorno*/
    free(mat1);
    free(mat2);
    free(saida);
    free(tid);
    free(ident);
    GET_TIME(finish3);
    elapsed1=finish1-start1;
    elapsed2=finish2-start2;
    elapsed3=finish3-start3;
    printf("tempo a:%f\n", elapsed1);
    printf("tempo b:%f\n", elapsed2);
    printf("tempo c:%f\n", elapsed3);
    return 0;
}
