/**
 * Segundo Trabalho de Implementação
 * Aluno: Lucas Favilla Ferreira Alves da Silva DRE:119156518
 * Aluno: Bruno Llacer Trotti                   DRE:...
 *
 * Main do programa.
 * */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"arrumar.h"

int main(int argc, char* argv[]){
    char a, lixo;
    ENT *an; 
    if(!(an = malloc(sizeof(ENT)))){
        printf("erro de alocação de an\n");
        return 1;
    }
    
    an->nthreads = argv[1];
    an->N = argv[2];
    an->entrada = argv[3];
    an->saida = argv[4];

    while(a>'3' || a<'1'){
        printf("--Bem vindo ao Segundo Trabalho de Implementação--\n");
        printf("Escolha o método de resolução:\n\n");
        printf("1 - sequêncial\n");
        printf("2 - concorrente\n");
        printf("3 - sair\n");
        scanf("%c", &a);
        scanf("%c", &lixo);
        if(a == '3') return 0;
        if(a == '1') *produtor((void*) an);
        if(a == '2') *produtor((void*) an);
    }
    return 0;
}























