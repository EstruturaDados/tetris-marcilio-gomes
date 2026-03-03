#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5

// Estrutura da peça
typedef struct {
    char nome;   // I, O, T, L
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

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

// Visualizar fila
void visualizarFila(Fila *f) {
    if (f->quantidade == 0) {
        printf("\nFila vazia!\n");
        return;
    }

    printf("\n=== FILA DE PECAS ===\n");

    int i = f->inicio;
    for (int count = 0; count < f->quantidade; count++) {
        printf("Posicao %d -> Tipo: %c | ID: %d\n",
               count + 1,
               f->itens[i].nome,
               f->itens[i].id);

        i = (i + 1) % TAMANHO_FILA;
    }
}

// Dequeue (jogar peça)
void jogarPeca(Fila *f) {
    if (f->quantidade == 0) {
        printf("\nNenhuma peca para jogar!\n");
        return;
    }

    Peca removida = f->itens[f->inicio];

    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;

    printf("\nPeca jogada -> Tipo: %c | ID: %d\n",
           removida.nome,
           removida.id);
}

// Enqueue (inserir nova peça)
void inserirPeca(Fila *f) {
    if (f->quantidade == TAMANHO_FILA) {
        printf("\nFila cheia! Nao e possivel inserir.\n");
        return;
    }

    f->itens[f->fim] = gerarPeca();
    printf("\nNova peca inserida -> Tipo: %c | ID: %d\n",
           f->itens[f->fim].nome,
           f->itens[f->fim].id);

    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->quantidade++;
}

// ================= MAIN =================

int main() {
    srand(time(NULL));

    Fila fila;
    int opcao;

    inicializarFila(&fila);

    do {
        printf("\n===== MENU TETRIS STACK =====\n");
        printf("1 - Visualizar fila\n");
        printf("2 - Jogar peca (dequeue)\n");
        printf("3 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                visualizarFila(&fila);
                break;
            case 2:
                jogarPeca(&fila);
                break;
            case 3:
                inserirPeca(&fila);
                break;
            case 0:
                printf("\nEncerrando...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}