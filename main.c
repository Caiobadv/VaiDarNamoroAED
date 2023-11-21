#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct respostaNode {
    char resposta[1000];
    struct respostaNode *proximo;
} RespostaNode;

RespostaNode *respostasGlobais = NULL;

typedef struct {
    char nome[50];
    int pontuacao;
} Jogador;

Jogador ranking[100];
int numJogadores = 0;

void atualizaRanking(Jogador ranking[], char *nome, int pontuacao, int *numJogadores) {
    int i;
    for (i = 0; i < *numJogadores; ++i) {
        if (strcmp(ranking[i].nome, nome) == 0) {
            ranking[i].pontuacao = pontuacao;
            return;
        }
    }
    strcpy(ranking[i].nome, nome);
    ranking[i].pontuacao = pontuacao;
    (*numJogadores)++;
}

void insertionSort(Jogador ranking[], int numJogadores) {
    int i, j;
    Jogador chave;
    for (i = 1; i < numJogadores; i++) {
        chave = ranking[i];
        j = i - 1;

        while (j >= 0 && ranking[j].pontuacao < chave.pontuacao) {
            ranking[j + 1] = ranking[j];
            j = j - 1;
        }
        ranking[j + 1] = chave;
    }
}

void salvaRankingEmArquivo(Jogador ranking[], int numJogadores, const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "w+");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de ranking");
        return;
    }

    for (int i = 0; i < numJogadores; i++) {
        fprintf(file, "%s %d\n", ranking[i].nome, ranking[i].pontuacao);
    }
    
    fclose(file);
}

void carregaRankingDeArquivo(Jogador ranking[], int *numJogadores, const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de ranking");
        *numJogadores = 0;
        return;
    }

    *numJogadores = 0;
    while (fscanf(file, "%49s %d", ranking[*numJogadores].nome, &ranking[*numJogadores].pontuacao) == 2) {
        (*numJogadores)++;
    }

    fclose(file);
}

void lerPerguntasRespostas(char *perguntas[], char *respostas[], const char *perguntasJogo, const char *respostaJogo, int *tamanho) {
    FILE *fpPerguntas = fopen(perguntasJogo, "r");
    FILE *fpRespostas = fopen(respostaJogo, "r");
    char linha[1000];
    *tamanho = 0;

    if (fpPerguntas == NULL || fpRespostas == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), fpPerguntas)) {
        perguntas[*tamanho] = strdup(linha);
        fgets(linha, sizeof(linha), fpRespostas);
        linha[strcspn(linha, "\n")] = 0;
        respostas[*tamanho] = strdup(linha);
        (*tamanho)++;
    }

    fclose(fpPerguntas);
    fclose(fpRespostas);
}

RespostaNode* addRedposta(RespostaNode *head, char *resposta) {
    RespostaNode *novoNode = (RespostaNode*)malloc(sizeof(RespostaNode));
    strcpy(novoNode->resposta, resposta);
    novoNode->proximo = NULL;

    if (head == NULL) {
        return novoNode;
    }

    RespostaNode *atual = head;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    atual->proximo = novoNode;

    return head;
}

void liberaLista(RespostaNode *head) {
    RespostaNode *atual = head;
    while (atual != NULL) {
        RespostaNode *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

void jogo(char *perguntas[], RespostaNode *respostaHead, char *respostas[], int numPerguntas, int *pontuacao) {
    RespostaNode *currentResponse = respostaHead;

    for (int i = 0; i < numPerguntas; i++) {
        if (currentResponse && strcasecmp(currentResponse->resposta, respostas[i]) == 0) {
            (*pontuacao)++;
        }

        if (currentResponse) currentResponse = currentResponse->proximo;
    }
}

int main() {
    char nome[50];
    char escolha;
    int pontuacao = 0;
    int numPerguntas = 0;
    char *perguntas[100];
    char *respostas[100];
    Jogador rankingAna[100];
    Jogador rankingCaio[100];
    int numJogadoresAna = 0;
    int numJogadoresCaio = 0;
    char respostaUsuario[1000];

    printf("Digite seu nome: ");
    scanf("%49s", nome);
    getchar();
    printf("Quem você quer conquistar? (Caio = C, Ana = A): ");
    scanf(" %c", &escolha);
    getchar();
    
    if (escolha == 'C' || escolha == 'c') {
        carregaRankingDeArquivo(rankingCaio, &numJogadoresCaio, "rankingCaio.txt");
        lerPerguntasRespostas(perguntas, respostas, "perguntasCaio.txt", "respostaCaio.txt", &numPerguntas);
    } else {
        carregaRankingDeArquivo(rankingAna, &numJogadoresAna, "rankingAna.txt");
        lerPerguntasRespostas(perguntas, respostas, "perguntasAna.txt", "respostaAna.txt", &numPerguntas);
    }

    for (int i = 0; i < numPerguntas; i++) {
        printf("Pergunta %d: %s\n", i + 1, perguntas[i]);
        printf("Resposta: ");
        fgets(respostaUsuario, 1000, stdin);
        respostaUsuario[strcspn(respostaUsuario, "\n")] = 0;

        respostasGlobais = addRedposta(respostasGlobais, respostaUsuario);
    }    
    jogo(perguntas, respostasGlobais, respostas, numPerguntas, &pontuacao);

    if (escolha == 'C' || escolha == 'c') {
        atualizaRanking(rankingCaio, nome, pontuacao, &numJogadoresCaio);
        insertionSort(rankingCaio, numJogadoresCaio);
        salvaRankingEmArquivo(rankingCaio, numJogadoresCaio, "rankingCaio.txt");
    } else {
        atualizaRanking(rankingAna, nome, pontuacao, &numJogadoresAna);
        insertionSort(rankingAna, numJogadoresAna);
        salvaRankingEmArquivo(rankingAna, numJogadoresAna, "rankingAna.txt");
    }

    if (escolha == 'C' || escolha == 'c') {
        printf("\nRanking Caio:\n");
    for (int i = 0; i < numJogadoresCaio; i++) {
        printf("%d. %s - %d pontos\n", i + 1, rankingCaio[i].nome, rankingCaio[i].pontuacao);
    }
    } else {
        printf("\nRanking Ana:\n");
        for (int i = 0; i < numJogadoresAna; i++) {
            printf("%d. %s - %d pontos\n", i + 1, rankingAna[i].nome, rankingAna[i].pontuacao);
        }
    }
    for (int i = 0; i < numPerguntas; i++) {
        free(perguntas[i]);
        free(respostas[i]);
    }

    return 0;
}