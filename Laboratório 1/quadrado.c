/*Laboratório 1 - Atividade 5*/
/*Aluno: Lucas Favilla Ferreira Alves da Silva DRE:119156518*/
/*Código: Cria um vetor de 10000 números e em cada casa coloca o quadrado de sua posição,
 * ex: numeros[90] = 8100*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NTHREADS 2
#define TOTAL 10000

//struct para poder diferenciar cada thread, o vetor e passado por referencia para nao ter neces
//sidade de criar mais de um.
typedef struct{
    int* numer;
    int id;
}ID;

//faz o quadrado em duas partes, uma faz os primerios 5000 e a outra os ultimos;
void * quadrado(void * vetor){
  ID * vector = (ID *) vetor;
  if(vector->id == 0){
    for(int i=0;i<=4999;i++){
      vector->numer[i]=i*i;
    }
    printf("terminei 1\n");
    free(vector);
    pthread_exit(NULL);
  }
  if(vector->id == 1){
    for(int j=5000;j<10000;j++){
      vector->numer[j]=j*j;
    }
    printf("terminei 2\n");
    free(vector);
    pthread_exit(NULL);
  }
}

int main(void){
  pthread_t tid[NTHREADS]; 
  int numeros[TOTAL];
  int erros =0;
  ID *a;
  //inicialização das threads
  for(int k =0;k<NTHREADS;k++){
    a = malloc(sizeof(ID));
    a->numer = numeros;
    a->id = k;
    if(pthread_create(&tid[k],NULL,quadrado,(void *) a))
        printf("erro\n");
  }
  //espera as threads terminarem
  for(int k=0;k<2;k++){
    if(pthread_join(tid[k],NULL)){
      printf("erro no join\n");
      exit(-1);
    }
  }
  //teste
  for(int k=0;k<TOTAL;k++){
    if(!(numeros[k] == k*k)){
      printf("numero %d errado, valor deu %d, deveria ter dado %d\n", k, numeros[k], k*k);
      erros++;
    }
    //descomente essa aqui em baixo para printar todas as respostas. ps:são 10000 numeros.
    //printf("%d\n",numeros[k]);
  }
    if(erros==0){
      printf("nenhum erro encontrado\n");
    }
  return 0;
}
