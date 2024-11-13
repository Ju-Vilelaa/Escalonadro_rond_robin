#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_PROCESSOS 5
#define QUANTUM 4

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

/*função manipulação fila*/
Fila* inicializa_fila(Fila* f){
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

void enfileirar_processos(Fila* f, Processo* p, int time){
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

/*CÓDIGO ANTIGO N ALTERADO*/

void executa_processo(Processo* p, Fila* fila_destino){
    if(p->tempo_restante > QUANTUM){
        p->tempo_restante -= QUANTUM;
        enfileirar_processos(fila_destino, p);
    }
    else{
        p->tempo_restante = 0;
        printf("Processo %d finalizado", p->pid);
        free(p);
    }
}

 
void gerencia_escalonamento(Fila* alta_prioridade, Fila* baixa_prioridade, Fila fila_io[]){
    int i;

    while(!fila_vazia(alta_prioridade) || !fila_vazia(baixa_prioridade) || !fila_vazia(&fila_io[0]) || !fila_vazia(&fila_io[1]) || !fila_vazia(&fila_io[2])){
        Processo* processo;

        /*Executa processos na fila de alta prioridade*/
        if(!fila_vazia(alta_prioridade)){
            processo = desinfileirar_processos(alta_prioridade);
            executa_processo(processo,baixa_prioridade);
        }

        /*Executa processos na fila de baixa prioridade*/
        else if(!fila_vazia(baixa_prioridade)){
            processo = desinfileirar_processos(baixa_prioridade);
            executa_processo(processo, baixa_prioridade);
        }

        /*Processa filas de I/O*/
        for(i = 0; i < 3; i++){
            if(!fila_vazia(&fila_io[i])){
                processo = desinfileirar_processos(&fila_io[i]);

                if(processo->tipo_io == 'd'){
                    enfileirar_processos(baixa_prioridade, processo);
                }
                else{
                    enfileirar_processos(alta_prioridade, processo);
                }
            }
        }
        printf("\n");
        printf("\n");
        printf("\n");
        printf("processo %d", processo->pid);
        printf("\n");
        printf("tempo_restante: %d", processo->tempo_restante);
        printf("\n");
    }
}

int main(){

    
    Fila alta_prioridade, baixa_prioridade;
    Fila fila_io[3] ; /*0 = disco, 1 = fita magnetica, 2 = impressora*/
    int i, j;
    srand(time(NULL));
    

    inicializa_fila(&alta_prioridade);
    inicializa_fila(&baixa_prioridade);

    for(i = 0; i < 3; i++){
        inicializa_fila(&fila_io[i]);
    }

    /*cria o processo e adiciona na fila de alta prioridade*/
    for(j = 1; j <=MAX_PROCESSOS; j++){
        Processo* novo_processo;
        novo_processo = cria_processo(j);
        printf("%d processo criado!\n\n", novo_processo->pid);
        printf("Processo %d\n", novo_processo->pid);
        printf("tempo servico: %d\n", novo_processo->tempo_servico);
        printf("tempo restante: %d\n", novo_processo->tempo_restante);
        printf("tipo io: %c\n", novo_processo->tipo_io);
        printf("tempo espera io: %d\n\n", novo_processo->tempo_espera_io);
        enfileirar_processos(&alta_prioridade, novo_processo);

    }
    
    /*Executa o escalonador*/
    gerencia_escalonamento(&alta_prioridade, &baixa_prioridade, fila_io);

    return 0;
} 

/*arrumar os printfs pra entender de fato qq esta acontecendo no algoritmo*/