#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char pergunta[1000];
    struct node *sim;
    struct node *nao;
    char resultado[1000];
}node;

typedef struct Player {
    char nome[50];
    int pontuacao;
} Player;

node* createnode(char *pergunta) {
    node *newnode = (node*) malloc(sizeof(node));
    strcpy(newnode->pergunta, pergunta);
    newnode->sim = NULL;
    newnode->nao = NULL;
    strcpy(newnode->resultado, "");
    return newnode;
}

void atualizaRanking(Player *ranking, char *nome, int pontuacao, int *tamanho) {
    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(ranking[i].nome, nome) == 0) {
            ranking[i].pontuacao += pontuacao;
            return;
        }
    }
    strcpy(ranking[*tamanho].nome, nome);
    ranking[*tamanho].pontuacao = pontuacao;
    (*tamanho)++;
}

void insertionSort(Player *ranking, int tamanho) {
    int i, j;
    Player key;
    for (i = 1; i < tamanho; i++) {
        key = ranking[i];
        j = i - 1;
        while (j >= 0 && ranking[j].pontuacao < key.pontuacao) {
            ranking[j + 1] = ranking[j];
            j = j - 1;
        }
        ranking[j + 1] = key;
    }
}

void exibeRanking(Player ranking[], int tamanho) {
    printf("\nRanking:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s - %d pontos\n", i + 1, ranking[i].nome, ranking[i].pontuacao);
    }
}

void salvaRankingEmArquivo(Player ranking[], int tamanho, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w+");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivo, "%s %d\n", ranking[i].nome, ranking[i].pontuacao);
    }

    fclose(arquivo);
}

void carregaRankingDeArquivo(Player ranking[], int *tamanho, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    *tamanho = 0;
    while (fscanf(arquivo, "%49s %d", ranking[*tamanho].nome, &ranking[*tamanho].pontuacao) == 2) {
        (*tamanho)++;
    }

    fclose(arquivo);
}

void jogo(node *atual, char *nome, int *pontuacao) {
    if (atual->sim == NULL && atual->nao == NULL) {
        printf(atual->resultado, nome);
        return;
    }

    printf("%s\n", atual->pergunta);
    char resposta;
    scanf(" %c", &resposta);

    if (resposta == 's' || resposta == 'S') {
        (*pontuacao)++;
        jogo(atual->sim, nome, pontuacao);
    } else {
        jogo(atual->nao, nome, pontuacao);
    }
}

void liberaArvore(node *raiz) {
    if (raiz == NULL) return;
    
    liberaArvore(raiz->sim);
    liberaArvore(raiz->nao);
    
    free(raiz);
}

int main() {
    char nome[50];
    printf("Digite seu nome: ");
    scanf("%s", nome);

    printf("Quem você quer conquistar? (Caio = C, Ana = A): ");
    char escolha;
    scanf(" %c", &escolha);

    node *raiz;
    int pontuacao = 0;

    if (escolha == 'C' || escolha == 'c') {
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
        raiz = createnode("Pergunta 1: (S para sim, N para não)");
        raiz->sim = createnode("Pergunta 2: (S para sim, N para não)");
        raiz->nao = createnode("Pergunta 2: (S para sim, N para não)");
        
        raiz->sim->sim = createnode("Pergunta 3: (S para sim, N para não)");
        raiz->sim->nao = createnode("");
        strcpy(raiz->sim->nao->resultado, "Infelizmente, %s, voce não conquistou Ana.");
        
        raiz->sim->sim->sim = createnode("Pergunta 4: (S para sim, N para não)");
        raiz->sim->sim->nao = createnode("");
        strcpy(raiz->sim->sim->nao->resultado, "Infelizmente, %s, voce não conquistou Ana.");

        raiz->sim->sim->sim = createnode("");
        strcpy(raiz->sim->sim->sim->resultado, "Parabéns, %s! voce conquistou Ana e ganhou direito a um date com ela!!!");
        raiz->sim->sim->nao = createnode("");
        strcpy(raiz->sim->sim->nao->resultado, "Infelizmente, %s, voce não conquistou Ana.");
    }

    jogo(raiz, nome, &pontuacao);

    Player rankingAna[100];
    Player rankingCaio[100];
    int tamanhoAna = 0, tamanhoCaio = 0;
    carregaRankingDeArquivo(rankingAna, &tamanhoAna, "ranking_ana.txt");
    carregaRankingDeArquivo(rankingCaio, &tamanhoCaio, "ranking_caio.txt");

    if (escolha == 'C' || escolha == 'c') {
        atualizaRanking(rankingCaio, nome, pontuacao, &tamanhoCaio);
        insertionSort(rankingCaio, tamanhoCaio);
        salvaRankingEmArquivo(rankingCaio, tamanhoCaio, "ranking_caio.txt");
        exibeRanking(rankingCaio, tamanhoCaio);
    } else {
        atualizaRanking(rankingAna, nome, pontuacao, &tamanhoAna);
        insertionSort(rankingAna, tamanhoAna);
        salvaRankingEmArquivo(rankingAna, tamanhoAna, "ranking_ana.txt");
        exibeRanking(rankingAna, tamanhoAna);
    }
    liberaArvore(raiz);
}
