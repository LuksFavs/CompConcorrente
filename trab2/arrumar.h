#ifndef _H_ARRUMAR
#define _H_ARRUMAR
extern int* buffer;
typedef struct{
    char* nthreads;
    char* N;
    char* entrada;
    char* saida;
}ENT;
void *produtor(void* T);
#endif

