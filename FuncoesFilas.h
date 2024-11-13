#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PROCESSOS 5


typedef struct Processo{
    int pid;
    int tempo_chegada;
    int tempo_servico;
    int tempo_restante;
    char tipo_io;
    int tempo_espera_io;
} Processo;

typedef struct No_Fila{
    Processo* p;
    struct No_Fila *proximo;
}No_Fila;

typedef struct Fila {
    No_Fila *frente;
    No_Fila *tras;
} Fila;

/*le o proximo inteiro da linha separada por ";"*/

int prox_inteiro(char *linha, int *inicio){

}

/*função manipulação fila*/
Fila* inicializa_fila(){
    Fila *f = (Fila *) malloc(sizeof(Fila));
    f->frente = NULL;
    f->tras = NULL;
    return f;
}

int fila_vazia(Fila* f){
    if(f->frente == NULL)
        return 1;
    return 0;
}

void enfileirar_processos(Fila* f, Processo* p, int tempo){
    if(fila_vazia(f)){
        f -> frente = (No_Fila*) malloc(sizeof(No_Fila));
        f -> frente -> p = p; 
        f -> frente -> p -> tempo_chegada = time;
        f -> frente -> proximo = NULL;
        f -> tras = f -> frente;
    }
    else{
        f -> tras -> proximo = (No_Fila*) malloc(sizeof(No_Fila));
        f -> tras -> proximo -> p = p;
        f -> tras -> proximo -> p -> tempo_chegada = time;
        f -> tras -> proximo -> proximo = NULL;
        f -> tras = f -> tras -> proximo;
    }
}

/*retorna o processo que está na frente da fila*/
Processo* frente(Fila* f){
    if(fila_vazia(f))
        return NULL;
    return f -> frente -> p;
}

void desinfileirar_processos(Fila* f){
    if(fila_vazia(f))
        return;
    /*um único elemento na fila*/
    if(f -> frente == f -> tras){
        free(f -> frente);
        f -> frente = f -> tras = NULL;
    }

    else{
        No_Fila *f_aux = f -> frente -> proximo;
        free(f -> frente);
        f -> frente = f_aux;
    }
}





Processo* cria_processo(int pid){
    Processo* p;
    int tipo_io;

    p = (Processo*)malloc(sizeof(Processo));
    p->pid = pid;
    p->tempo_servico = rand() % 10 + 1;
    p->tempo_restante = p->tempo_servico;

    tipo_io = rand() % 3;

        switch (tipo_io){

        case 0:
            p->tipo_io = 'd'; /*disco*/
            p->tempo_espera_io = 3;
            break;
        case 1:
            p->tipo_io = 'f'; /*fita magnética*/
            p->tempo_espera_io = 5;
            break;
        case 2:
            p->tipo_io = 'i'; /*impressora*/
            p->tempo_espera_io = 4;
            break;
        }
    return p;
}








