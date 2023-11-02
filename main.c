#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char pergunta[1000];
    struct node *sim;
    struct node *nao;
    char resultado[1000];
}node;

node* createnode(char *pergunta) {
    node *newnode = (node*) malloc(sizeof(node));
    strcpy(newnode->pergunta, pergunta);
    newnode->sim = NULL;
    newnode->nao = NULL;
    strcpy(newnode->resultado, "");
    return newnode;
}

void jogo(node *atual, char *nome) {
    if (atual->sim == NULL && atual->nao == NULL) {
        printf(atual->resultado, nome);
        return;
    }

    printf("%s\n", atual->pergunta);
    char resposta;
    scanf(" %c", &resposta);

    if (resposta == 's' || resposta == 'S') {
        jogo(atual->sim, nome);
    } else {
        jogo(atual->nao, nome);
    }
}

int main() {
    char nome[50];
    printf("Digite seu nome: ");
    scanf("%s", nome);

    printf("Quem você quer conquistar? (Caio = C, Ana = A): ");
    char escolha;
    scanf(" %c", &escolha);

    node *raiz;

    if (escolha == 'C' || escolha == 'c') {
        // Construindo árvore para Caio
        raiz = createnode("Pergunta 1:(S para sim, N para não)");
        raiz->sim = createnode("Pergunta 2: (S para sim, N para não)");
        raiz->nao = createnode("Pergunta 2: (S para sim, N para não)");
        
        raiz->sim->sim = createnode("Pergunta 3: (S para sim, N para não)");
        raiz->sim->nao = createnode("");
        strcpy(raiz->sim->nao->resultado, "Infelizmente, %s, voce não conquistou o Caio.");
        
        raiz->sim->sim->sim = createnode("Pergunta 4: (S para sim, N para não)");
        raiz->sim->sim->nao = createnode("");
        strcpy(raiz->sim->sim->nao->resultado, "Infelizmente, %s, voce não conquistou Caio.");
        
        raiz->sim->sim->sim->sim = createnode("");
        strcpy(raiz->sim->sim->sim->sim->resultado, "Parabéns, %s! voce conquistou Caio e ganhou direito a um date com ele!!!");
        raiz->sim->sim->sim->nao = createnode("");
        strcpy(raiz->sim->sim->sim->nao->resultado, "Infelizmente, %s, voce não conquistou Caio.");

    } else {
        // Construindo árvore para Ana
        raiz = createnode("Pergunta 1: (S para sim, N para não)");
        raiz->sim = createnode("Pergunta 2: (S para sim, N para não)");
        raiz->nao = createnode("Pergunta 2: (S para sim, N para não)");
        
        raiz->sim->sim = createnode("Pergunta 3: (S para sim, N para não)");
        raiz->sim->nao = createnode("");
        strcpy(raiz->sim->nao->resultado, "Infelizmente, %s, voce não conquistou Ana.");
        
        raiz->sim->sim->sim = createnode("");
        strcpy(raiz->sim->sim->sim->resultado, "Parabéns, %s! voce conquistou Ana e ganhou direito a um date com ela!!!");
        raiz->sim->sim->nao = createnode("");
        strcpy(raiz->sim->sim->nao->resultado, "Infelizmente, %s, voce não conquistou Ana.");
    }

    jogo(raiz, nome);
    if (escolha == 'C' || escolha == 'c') {
        free(raiz->sim->sim->sim->sim);
        free(raiz->sim->sim->sim->nao);
        free(raiz->sim->sim->sim);
        free(raiz->sim->sim->nao);
        free(raiz->sim->sim);
        free(raiz->sim->nao);
        free(raiz->sim);
        free(raiz->nao);
    } else {
        free(raiz->sim->sim->sim);
        free(raiz->sim->sim->nao);
        free(raiz->sim->sim);
        free(raiz->sim->nao);
        free(raiz->sim);
        free(raiz->nao);
    }
    free(raiz);
}