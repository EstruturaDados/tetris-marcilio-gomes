#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

typedef struct {
    Peca itens[TAMANHO_PILHA];
    int topo;
} Pilha;

// Estrutura para salvar estado (undo)
typedef struct {
    Fila fila;
    Pilha pilha;
    int valido;
} EstadoAnterior;

// ================= FUNÇÕES =================

Peca gerarPeca() {
    char tipos[] = {'I','O','T','L'};
    Peca p;
    p.nome = tipos[rand()%4];
    p.id = rand()%1000;
    return p;
}

void salvarEstado(Fila *f, Pilha *p, EstadoAnterior *e) {
    e->fila = *f;
    e->pilha = *p;
    e->valido = 1;
}

void restaurarEstado(Fila *f, Pilha *p, EstadoAnterior *e) {
    if (e->valido) {
        *f = e->fila;
        *p = e->pilha;
        e->valido = 0;
        printf("\nUltima jogada desfeita!\n");
    } else {
        printf("\nNenhuma jogada para desfazer!\n");
    }
}

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;

    for(int i=0;i<TAMANHO_FILA;i++){
        f->itens[f->fim] = gerarPeca();
        f->fim = (f->fim+1)%TAMANHO_FILA;
        f->quantidade++;
    }
}

void inicializarPilha(Pilha *p){
    p->topo = -1;
}

Peca removerFila(Fila *f){
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio+1)%TAMANHO_FILA;
    f->quantidade--;
    return p;
}

void inserirFila(Fila *f){
    if(f->quantidade < TAMANHO_FILA){
        f->itens[f->fim] = gerarPeca();
        f->fim = (f->fim+1)%TAMANHO_FILA;
        f->quantidade++;
    }
}

int push(Pilha *p, Peca nova){
    if(p->topo == TAMANHO_PILHA-1) return 0;
    p->itens[++p->topo] = nova;
    return 1;
}

int pop(Pilha *p, Peca *saida){
    if(p->topo == -1) return 0;
    *saida = p->itens[p->topo--];
    return 1;
}

void visualizar(Fila *f, Pilha *p){
    printf("\n===== FILA =====\n");
    int i = f->inicio;
    for(int c=0;c<f->quantidade;c++){
        printf("Tipo:%c ID:%d\n",f->itens[i].nome,f->itens[i].id);
        i = (i+1)%TAMANHO_FILA;
    }

    printf("\n===== PILHA =====\n");
    if(p->topo==-1) printf("Vazia\n");
    else{
        for(int j=p->topo;j>=0;j--){
            printf("Tipo:%c ID:%d\n",p->itens[j].nome,p->itens[j].id);
        }
    }
}

// Inverter fila com pilha
void inverter(Fila *f, Pilha *p){
    Peca temp[TAMANHO_FILA];
    int count=0;

    while(p->topo!=-1 && count<TAMANHO_FILA){
        pop(p,&temp[count++]);
    }

    for(int i=0;i<count;i++){
        f->itens[(f->inicio+i)%TAMANHO_FILA] = temp[i];
    }

    printf("\nFila substituida pelas pecas da pilha!\n");
}

// ================= MAIN =================

int main(){
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    EstadoAnterior estado = {0};

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int op;

    do{
        printf("\n===== MENU MESTRE =====\n");
        printf("1-Jogar\n2-Reservar\n3-Usar reserva\n4-Trocar topo/fila\n5-Desfazer\n6-Inverter\n0-Sair\nOpcao:");
        scanf("%d",&op);

        switch(op){

            case 1:{
                salvarEstado(&fila,&pilha,&estado);
                Peca p = removerFila(&fila);
                printf("\nJogada: %c %d\n",p.nome,p.id);
                inserirFila(&fila);
                break;
            }

            case 2:{
                if(pilha.topo==TAMANHO_PILHA-1){ printf("Pilha cheia!\n"); break;}
                salvarEstado(&fila,&pilha,&estado);
                Peca p = removerFila(&fila);
                push(&pilha,p);
                inserirFila(&fila);
                break;
            }

            case 3:{
                salvarEstado(&fila,&pilha,&estado);
                Peca usada;
                if(pop(&pilha,&usada))
                    printf("Usou reserva: %c %d\n",usada.nome,usada.id);
                else
                    printf("Pilha vazia!\n");
                break;
            }

            case 4:{
                if(pilha.topo==-1){ printf("Pilha vazia!\n"); break;}
                salvarEstado(&fila,&pilha,&estado);
                Peca topo = pilha.itens[pilha.topo];
                pilha.itens[pilha.topo] = fila.itens[fila.inicio];
                fila.itens[fila.inicio] = topo;
                printf("Troca realizada!\n");
                break;
            }

            case 5:
                restaurarEstado(&fila,&pilha,&estado);
                break;

            case 6:
                salvarEstado(&fila,&pilha,&estado);
                inverter(&fila,&pilha);
                break;

            case 0:
                printf("\nEncerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

        visualizar(&fila,&pilha);

    }while(op!=0);

    return 0;
}