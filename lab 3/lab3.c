/* LAB 3 - Encontrar o maior e o menor elemento de um vetor */
/* Aluno: Lucas Favilla Ferreira Alves da Silva */
/* DRE: 119156518*/
/* Para rodar o programa faça: "./<nome dado na compilação> <tamanho do vetor> <quantidade de threads(1,2,4)>" */
/* OBS: as opções de threads são: 1,2,4*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long long int dim;
int nthreads;
float *vetor;

typedef struct {
    int id;
    float maior;
    float menor;
}SAIDA;


SAIDA *saida;

void *check(void *sai){
    SAIDA* said = (SAIDA*) sai;
    if(nthreads == 2){
        if(said->id==0){
            for(long long int i=0;i<dim;i+=2){
                if(i==0){
                    said->maior = vetor[i];
                    said->menor = vetor[i];
                    continue;
                }
                if(vetor[i] > said->maior){
                    said->maior = vetor[i];
                    continue;
                }
                if(vetor[i] < said->menor){
                    said->menor = vetor[i];
                    continue;
                }
            }
        }
        if(said->id==1){
            for(long long int j=1;j<dim;j+=2){
                if(j==1){
                    said->maior = vetor[j];
                    said->menor = vetor[j];
                    continue;
                }
                if(vetor[j] > said->maior){
                    said->maior = vetor[j];
                    continue;
                }
                if(vetor[j] < said->menor){
                    said->menor = vetor[j];
                    continue;
                }
            }
        }
        pthread_exit(NULL);
        }
    if(nthreads == 4){
        if(said->id==0){
            for(long long int i=0;i<dim;i+=4){
                if(i==0){
                    said->maior = vetor[i];
                    said->menor = vetor[i];
                    continue;
                }
                if(vetor[i] > said->maior){
                    said->maior = vetor[i];
                    continue;
                }
                if(vetor[i] < said->menor){
                    said->menor = vetor[i];
                    continue;
                }
            }
        }
        if(said->id==1){
            for(long long int i=1;i<dim;i+=4){
                if(i==1){
                    said->maior = vetor[i];
                    said->menor = vetor[i];
                    continue;
                }
                if(vetor[i] > said->maior){
                    said->maior = vetor[i];
                    continue;
                }
                if(vetor[i] < said->menor){
                    said->menor = vetor[i];
                    continue;
                }
            }
       }
        if(said->id==2){
            for(long long int i=2;i<dim;i+=4){
                if(i==2){
                    said->maior = vetor[i];
                    said->menor = vetor[i];
                    continue;
                }
                if(vetor[i] > said->maior){
                    said->maior = vetor[i];
                    continue;
                }
                if(vetor[i] < said->menor){
                    said->menor = vetor[i];
                    continue;
                }
            }
        }
        if(said->id==3){
            for(long long int i=3;i<dim;i+=4){
                if(i==3){
                    said->maior = vetor[i];
                    said->menor = vetor[i];
                    continue;
                }
                if(vetor[i] > said->maior){
                    said->maior = vetor[i];
                    continue;
                }
                if(vetor[i] < said->menor){
                    said->menor = vetor[i];
                    continue;
                }
            }
        }
        pthread_exit(NULL);
        }
}

int main(int argc, char* argv[]){
    //inicialização e verificação dos parametros.
    if(argc < 3){
        printf("Para rodar o programa faça:\n./<nome dado na compilação> <tamanho do vetor> <quantidade de threads a ser usada>\n");
        return 1;
    }
    double thread1, fthread1, thread2, fthread2, fthread4, thread4;
    pthread_t *tid;
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    vetor = (float*) malloc(dim*sizeof(float));
    if(!vetor){
        fprintf(stderr, "ERRO DE ALOCAÇÃO: MALLOC vetor\n");
        return 2;
    }
    if(nthreads == 1) saida = (SAIDA*) malloc((nthreads)*sizeof(SAIDA));
    else saida = (SAIDA*) malloc((nthreads+1)*sizeof(SAIDA));
    if(!saida){
        printf("ERRO DE ALOCAÇÃO: MALLOC saida\n");
        return 3;
    }
    //preenche o vetor.
    srand((unsigned int)time(NULL));
    float a = 1000000.0;
    for(long long int i=0; i<dim; i++){
        //printf("Entrei1\n");
        float x = ((float)rand()/(float)(RAND_MAX))*a;
        vetor[i] = x;
        //printf("%f, %f\n", vetor[i], x);
    }

    //localizar o maior e menor dependendo das threads
    if(nthreads == 1){
        GET_TIME(thread1);
        for(long long int j=0; j<dim; j++){
            //printf("Entrei\n");
            if(j==0){
                saida->maior = vetor[j];
                saida->menor = vetor[j];
                continue;
            }
            if(vetor[j] > saida->maior){
                saida->maior = vetor[j];
                continue;
            }
            if(vetor[j] < saida->menor){
                saida->menor = vetor[j];
                continue;
            }
        }
    }
        GET_TIME(fthread1);
        if(nthreads > 1){
            for(long long int j=0; j<dim; j++){
                //printf("Entrei\n");
                if(j==0){
                    saida[nthreads].maior = vetor[j];
                    saida[nthreads].menor = vetor[j];
                    continue;
                }
                if(vetor[j] > saida[nthreads].maior){
                    saida[nthreads].maior = vetor[j];
                    continue;
                }
                if(vetor[j] < saida[nthreads].menor){
                    saida[nthreads].menor = vetor[j];
                    continue;
                }
            }
            tid=(pthread_t*) malloc(sizeof(pthread_t)*nthreads);
            if(tid==NULL){printf("ERRO-Criação de threads\n");return 2;}
            if(nthreads == 2) GET_TIME(thread2);
            if(nthreads == 4) GET_TIME(thread4);
            for(int i=0;i<nthreads;i++){
                //printf("aaaaaaaaaaaaa\n");
                (saida+i)->id = i;
                if(pthread_create(tid+i,NULL,check,(void*)(saida+i))){
                    puts("ERRO-pthread_create\n");
                    return 4;
                }
            }
            for(int i=0;i<nthreads;i++){
                pthread_join(*(tid+i),NULL);
            }
            if(nthreads == 2) GET_TIME(fthread2);
            if(nthreads == 4) GET_TIME(fthread4);
            if(nthreads == 2){
                if(saida[0].menor > saida[1].menor) saida[0].menor = saida[1].menor;
                else saida[1].menor = saida[0].menor;
                if(saida[0].maior < saida[1].maior) saida[0].maior = saida[1].maior;
                else saida[1].maior = saida[0].maior;
                if(saida[0].menor != saida[nthreads].menor) printf("erro 1");
                if(saida[0].maior != saida[nthreads].maior) printf("erro 2");
            }
            if(nthreads == 4){
                if(saida[0].menor > saida[1].menor) saida[0].menor = saida[1].menor;
                else saida[1].menor = saida[0].menor;
                if(saida[2].menor > saida[3].menor) saida[2].menor = saida[3].menor;
                else saida[3].menor = saida[2].menor;
                if(saida[0].menor > saida[3].menor) saida[0].menor = saida[3].menor;
                else saida[3].menor = saida[0].menor;
                if(saida[0].maior < saida[1].maior) saida[0].maior = saida[1].maior;
                else saida[1].maior = saida[0].maior;
                if(saida[2].maior < saida[3].maior) saida[2].maior = saida[3].maior;
                else saida[3].maior = saida[2].maior;
                if(saida[0].maior < saida[3].maior) saida[0].maior = saida[3].maior;
                else saida[3].maior = saida[0].maior;
                if(saida[0].menor != saida[nthreads].menor) printf("erro 1");
                if(saida[0].maior != saida[nthreads].maior) printf("erro 2");
    
            }
        }
    //finalização e liberação do espaço;
    if(nthreads == 1)printf("Maior: %f, Menor: %f\nTempo: %f\n", saida->maior, saida->menor, fthread1-thread1);
    if(nthreads == 2)printf("Maior: %f, Menor: %f\nTempo: %f\n", saida[0].maior, saida[0].menor, fthread2-thread2);
    if(nthreads == 4)printf("Maior: %f, Menor: %f\nTempo: %f\n", saida[0].maior, saida[0].menor, fthread4-thread4);
    free(vetor);
    free(saida);
    return 0;

}
