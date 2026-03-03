#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

// ================= ESTRUTURAS =================

typedef struct {
    char nome;  // I, O, T, L
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

// ================= FUNÇÕES =================

// Gera peça automaticamente
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = rand() % 1000;
    return nova;
}

// Inicializa fila com 5 peças
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;

    for (int i = 0; i < TAMANHO_FILA; i++) {
        f->itens[f->fim] = gerarPeca();
        f->fim = (f->fim + 1) % TAMANHO_FILA;
        f->quantidade++;
    }
}

// Inicializa pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Inserção automática na fila
void inserirFila(Fila *f) {
    if (f->quantidade == TAMANHO_FILA)
        return;

    f->itens[f->fim] = gerarPeca();
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->quantidade++;
}

// Remove da fila
Peca removerFila(Fila *f) {
    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;
    return removida;
}

// Push na pilha
int push(Pilha *p, Peca nova) {
    if (p->topo == TAMANHO_PILHA - 1)
        return 0;

    p->itens[++p->topo] = nova;
    return 1;
}

// Pop da pilha
int pop(Pilha *p, Peca *removida) {
    if (p->topo == -1)
        return 0;

    *removida = p->itens[p->topo--];
    return 1;
}

// Visualização
void visualizarEstado(Fila *f, Pilha *p) {
    printf("\n===== FILA =====\n");
    int i = f->inicio;

    for (int count = 0; count < f->quantidade; count++) {
        printf("Tipo: %c | ID: %d\n",
               f->itens[i].nome,
               f->itens[i].id);
        i = (i + 1) % TAMANHO_FILA;
    }

    printf("\n===== PILHA (Reserva) =====\n");

    if (p->topo == -1) {
        printf("Pilha vazia\n");
    } else {
        for (int j = p->topo; j >= 0; j--) {
            printf("Tipo: %c | ID: %d\n",
                   p->itens[j].nome,
                   p->itens[j].id);
        }
    }
}

// ================= MAIN =================

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        printf("\n===== MENU AVENTUREIRO =====\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: {
                Peca jogada = removerFila(&fila);
                printf("\nPeca jogada -> Tipo: %c | ID: %d\n",
                       jogada.nome, jogada.id);

                inserirFila(&fila); // mantém sempre 5
                break;
            }

            case 2: {
                if (pilha.topo == TAMANHO_PILHA - 1) {
                    printf("\nPilha cheia! Nao e possivel reservar.\n");
                    break;
                }

                Peca reservada = removerFila(&fila);
                push(&pilha, reservada);

                printf("\nPeca reservada -> Tipo: %c | ID: %d\n",
                       reservada.nome, reservada.id);

                inserirFila(&fila); // mantém sempre 5
                break;
            }

            case 3: {
                Peca usada;

                if (!pop(&pilha, &usada)) {
                    printf("\nNenhuma peca reservada!\n");
                } else {
                    printf("\nPeca usada da reserva -> Tipo: %c | ID: %d\n",
                           usada.nome, usada.id);
                }
                break;
            }

            case 0:
                printf("\nEncerrando...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

        visualizarEstado(&fila, &pilha);

    } while (opcao != 0);

    return 0;
}