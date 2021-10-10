/**
 * Segundo Trabalho de Implementação
 * Aluno: Lucas Favilla Ferreira Alves da Silva DRE:119156518
 * Aluno: Bruno Llacer Trotti                   DRE:...
 *
 * Implementação da resolução sequencial.
 * */

#include<stdio.h>
#include<stdlib.h>
#include"arrumar.h"

int* buffer;

void *produtor(void* T){
    
ENT *A = (ENT*) T;

    //parametros
    int nt=atoi(A->nthreads);
    int n=atoi(A->N);

    //file
    FILE *file;
    size_t buf = 0;
    ssize_t tamanho;
    char* linha=NULL;
    
    if(!(file=fopen(A->entrada, "r"))){
        printf("erro abrindo o arquivo\n");
        return;
    }
    //logica
    int aux;  
    tamanho=getline(&linha, &buf, file);
    aux=atoi(linha);
    
    if(!(buffer=malloc(10*aux*sizeof(int)))){
        printf("erro allocando o buffer");
        return;
    }
    
    for(int i=0; i<aux/n;i++){
        for(int j=0;j<n;j++){
            getdelim(&linha,&buf, 32, file);
            buffer[j+i*n] = atoi(linha);
        }
    }
    printf("%d, %d, %d, %d, %d", buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
}
