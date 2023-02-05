#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static char vazio[2] = " ";

#define LIVRE(h) (strcmp(tabela[h].chave, vazio) == 0)
#define INCR(h) (h = h == tamanhoTS - 1 ? 0 : h + 1) 

typedef struct celulaTS CelulaTS;
struct celulaTS {
    char chave[25];
    int conteudo;
};

static CelulaTS* tabela = NULL;
static int numChaves = 0;
static int tamanhoTS;

int primo(int tamanhoTS) {
    for (int i = 2; i <= tamanhoTS / 2; i++) {
        if (tamanhoTS % i == 0) {
            return 0;
        }
    }
    return 1;
}

int hash(char chave[25], int M) {
    int i, h = 0;
    int primoMult = ceil(M * 2.5);
    while (!primo(primoMult)) {
        primoMult++;
    }

    for (i = 0; chave[i] != '\0'; i++)
        h = (h * primoMult + chave[i]) % M;
    return h;
}

void iniciaTS(int tamanhoMax) {
    int h; tamanhoTS = tamanhoMax;
    tabela = malloc(tamanhoTS * sizeof(CelulaTS));
    for (h = 0; h < tamanhoTS; h++) strcpy(tabela[h].chave, vazio);
}

CelulaTS* buscaTS(char chave[25]) {
    int h = hash(chave, tamanhoTS);
    while (!LIVRE(h) && strcmp(tabela[h].chave, chave) != 0)
        INCR(h);
    if (!LIVRE(h)) {
        CelulaTS* p;
        p = &tabela[h];
        return p;
    }
    return NULL;
}

void insereTS(char chave[], int conteudo) {
    int h = hash(chave, tamanhoTS);
    while (!LIVRE(h) && strcmp(tabela[h].chave, chave) != 0)
        INCR(h);
    if (LIVRE(h)) {
        if (numChaves == tamanhoTS - 1) {
            printf("Tabela cheia\n"); return;
        }
        strcpy(tabela[h].chave, chave); numChaves++;
    }
    tabela[h].conteudo = conteudo;
}

void removeTS(char chave[25]) {
    int h = hash(chave, tamanhoTS);
    while (!LIVRE(h) && strcmp(tabela[h].chave, chave) != 0)
        INCR(h);
    if (LIVRE(h))
        return;

    free(tabela[h].chave);
    strcpy(tabela[h].chave, vazio);
    numChaves--;

    for (INCR(h); !LIVRE(h); INCR(h)) {
        char chave[25];
        strcpy(chave, tabela[h].chave);
        int conteudo = tabela[h].conteudo;
        strcpy(tabela[h].chave, vazio);
        insereTS(chave, conteudo);
    }
}

void limpaTS() {
    int h;
    for (h = 0; h < tamanhoTS; h++)
        if (!LIVRE(h))
            free(tabela[h].chave);
    free(tabela);
    tabela = NULL;
    numChaves = 0;
}

char** split(char texto[]) {
    char* temp = 0;
    char** result = 0;
    unsigned int tamanho = 0;

    temp = strtok(texto, " ");

    if (temp) {
        result = malloc((tamanho + 1) * sizeof(char**));
        result[tamanho++] = temp;
    }

    while ((temp = strtok(0, " ")) != 0) {
        result = realloc(result, (tamanho + 1) * sizeof(char**));
        result[tamanho++] = temp;
    }

    for (int i = 0; i < tamanho; i++)
        printf("%d - %s\n", i, result[i]);

    return result;
}

int main() {

    int N /*Número de palavras escolhidas*/, M /*Número de participantes*/;

    char palavra_escolhida[25];
    int pontuacao_palavra;
    int* pontuacoes_participantes;

    char palavra_texto[25];

    char terminador_texto[2] = ".";

    scanf("%d %d", &N, &M);

    pontuacoes_participantes = malloc(M * sizeof(int));

    for (int i = 0; i < M; i++) {
        pontuacoes_participantes[i] = 0;
    }

    // Busca pelo primeiro primo maior que 1.3 vezes o núemro de items da tabela
    tamanhoTS = ceil(1.3 * N);
    while (!primo(tamanhoTS)) {
        tamanhoTS++;
    }

    iniciaTS(tamanhoTS);

    for (int i = 0; i < N; i++) {
        scanf("%s %d", palavra_escolhida, &pontuacao_palavra);
        insereTS(palavra_escolhida, pontuacao_palavra);
    }

    int n = 0;
    for (int j = 0; j < M; j++) {

        scanf("%s", palavra_texto);

        while (strcmp(palavra_texto, terminador_texto) != 0) {

            CelulaTS* p = buscaTS(palavra_texto);
            if ((p != NULL) && (strcmp(p->chave, palavra_texto) == 0)) {
                pontuacoes_participantes[n] = pontuacoes_participantes[n] + p->conteudo;
            }
            else {
                pontuacoes_participantes[n] = pontuacoes_participantes[n] - 10;
            }

            scanf("%s", palavra_texto);
        }
        n++;
    }

    for (int m = 0; m < n; m++) {
        printf("%d\n", pontuacoes_participantes[m]);
    }

    return 0;
}