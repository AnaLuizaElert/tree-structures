#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//https://www.youtube.com/watch?v=AH7zF9PuIAw
//https://www.youtube.com/watch?v=5mC6TmviBPE
//https://www.youtube.com/watch?v=zE1FMszj_1c&list=PLG3ume5wUwQrEbhmxvTR52XRinLjwOmt3
//https://www.youtube.com/watch?v=yt7PbXBaoYs
//https://www.youtube.com/watch?v=u7ATFmZ2cdc
typedef struct no
{
    int total;
    int *chaves;
    struct no **filhos;
    struct no *pai;
} No;

typedef struct arvoreB
{
    No *raiz;
    int ordem;
} ArvoreB;

No *criaNo(ArvoreB *arvore)
{
    int max = arvore->ordem * 2;
    No *no = malloc(sizeof(No));
    no->pai = NULL;

    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(No*) * (max + 2));
    no->total = 0;
    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;
    return no;
}

ArvoreB *criaArvore(int ordem)
{
    ArvoreB *a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);
    return a;
}

void imprimeEstrutura(No *no, int nivel)
{
    if (no == NULL)
    {
        return;
    }

    for (int i = 0; i < nivel; i++)
    {
        printf("    "); // identação por nível para melhor visualização
    }

    printf("Nivel %d: [", nivel);
    for (int i = 0; i < no->total; i++)
    {
        printf("%d", no->chaves[i]);
        if (i < no->total - 1)
        {
            printf(", ");
        }
    }
    printf("] ");

    if (no->pai == NULL)
    {
        printf("(RAIZ)\n");
    }
    else
    {
        printf("(Pai: [");
        for (int i = 0; i < no->pai->total; i++)
        {
            printf("%d", no->pai->chaves[i]);
            if (i < no->pai->total - 1)
            {
                printf(", ");
            }
        }
        printf("])\n");
    }

    for (int i = 0; i <= no->total; i++)
    {
        imprimeEstrutura(no->filhos[i], nivel + 1);
    }
}
int pesquisaBinaria(No *no, int chave, int *esforco)
{
    (*esforco)++;
    int inicio = 0, fim = no->total - 1, meio;
    while (inicio <= fim)
    {
        meio = (inicio + fim) / 2;
        (*esforco)++;
        if (no->chaves[meio] == chave)
        {
            return meio;
        }
        else if (no->chaves[meio] > chave)
        {
            fim = meio - 1;
        }
        else
        {
            inicio = meio + 1;
        }
        (*esforco) += 2;
    }
    return inicio;
}

int localizaChave(ArvoreB *arvore, int chave, int *esforco)
{
    No *no = arvore->raiz;
    (*esforco)++;
    while (no != NULL)
    {
        int i = pesquisaBinaria(no, chave, esforco);
        (*esforco)++;
        if (i < no->total && no->chaves[i] == chave)
        {
            return 1;
        }
        else
        {
            (*esforco)++;
            no = no->filhos[i];
        }
        (*esforco)++;
    }
    return 0;
}

No *localizaNo(ArvoreB *arvore, int chave, int *esforco)
{
    No *no = arvore->raiz;

    (*esforco)++;
    while (no != NULL)
    {
        int i = pesquisaBinaria(no, chave, esforco);
        (*esforco)++;
        if (no->filhos[i] == NULL)
            return no;
        else {
            (*esforco)++;
            no = no->filhos[i];
        }
        (*esforco)++;
    }
    return NULL;
}

void adicionaChaveNo(No *no, No *direita, int chave, int *esforco)
{
    int i = pesquisaBinaria(no, chave, esforco);
    (*esforco)++;
    for (int j = no->total - 1; j >= i; j--)
    {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
        (*esforco)++;
    }

    (*esforco)++;
    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    no->total++;
}

int overflow(ArvoreB *arvore, No *no)
{
    return no->total > arvore->ordem * 2;
}

No *split(ArvoreB *arvore, No *no, int *esforco)
{
    int meio = no->total / 2;
    No *novo = criaNo(arvore);
    novo->pai = no->pai;

    (*esforco)++;
    for (int i = meio + 1; i < no->total; i++)
    {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        (*esforco)++;
        if (novo->filhos[novo->total] != NULL)
            novo->filhos[novo->total]->pai = novo;

        novo->total++;
        (*esforco)++;
    }
    novo->filhos[novo->total] = no->filhos[no->total];
    (*esforco)++;
    if (novo->filhos[novo->total] != NULL)
        novo->filhos[novo->total]->pai = novo;
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivo(ArvoreB *arvore, No *no, No *novo, int chave, int *esforco)
{
    adicionaChaveNo(no, novo, chave, esforco);
    (*esforco)++;
    if (overflow(arvore, no))
    {
        int promovido = no->chaves[arvore->ordem];
        No *novo_dividido = split(arvore, no, esforco);
        (*esforco)++;
        if (no->pai == NULL)
        {
            No *raiz = criaNo(arvore);
            raiz->filhos[0] = no;
            adicionaChaveNo(raiz, novo_dividido, promovido ,esforco);

            no->pai = raiz;
            novo_dividido->pai = raiz;
            arvore->raiz = raiz;
        }
        else {
            adicionaChaveRecursivo(arvore, no->pai, novo_dividido, promovido, esforco);
        }
    }
}

void adicionaChave(ArvoreB *arvore, int chave, int *esforco)
{
    No *no = localizaNo(arvore, chave, esforco);
    adicionaChaveRecursivo(arvore, no, NULL, chave, esforco);
}

void imprimeChave(int chave)
{
    printf("%d ", chave);
}

void removeChave(ArvoreB *arvore, No *no, int chave, int *esforco);

//maior chave da subárvore esquerda
int pegaPredecessor(ArvoreB *arvore, No *no, int indice, int *esforco) {
    No *atual = no->filhos[indice];
    (*esforco)++;
    while (atual->filhos[atual->total] != NULL) {
        atual = atual->filhos[atual->total];
        (*esforco)++;
    }
    return atual->chaves[atual->total - 1];
}

//menor chave da subárvore direita
int pegaSucessor(ArvoreB *arvore, No *no, int indice, int *esforco) {
    No *atual = no->filhos[indice + 1];
    (*esforco)++;
    while (atual->filhos[0] != NULL) {
        atual = atual->filhos[0];
        (*esforco)++;
    }
    return atual->chaves[0];
}

void merge(ArvoreB *arvore, No *no, int indice, int *esforco) {
    No *filho = no->filhos[indice];
    No *irmao = no->filhos[indice + 1];

    filho->chaves[filho->total] = no->chaves[indice];
    filho->total++;

    (*esforco)++;
    for (int i = 0; i < irmao->total; ++i) {
        filho->chaves[filho->total + i] = irmao->chaves[i];
        (*esforco)++;
    }
    if (filho->filhos[0] != NULL) { //não é folha
        for (int i = 0; i <= irmao->total; ++i) {
            filho->filhos[filho->total + i] = irmao->filhos[i];
            if (filho->filhos[filho->total + i])
                filho->filhos[filho->total + i]->pai = filho;
        }
    }

    filho->total += irmao->total;

    for (int i = indice; i < no->total - 1; ++i) {
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }
    no->total--;

    free(irmao->chaves);
    free(irmao->filhos);
    free(irmao);
}

//(Esq -> Dir)
void rotacaoDireita(ArvoreB *arvore, No *no, int indice) {
    No *filho = no->filhos[indice];
    No *irmao = no->filhos[indice - 1];

    for (int i = filho->total - 1; i >= 0; --i) {
        filho->chaves[i + 1] = filho->chaves[i];
    }
    if (filho->filhos[0] != NULL) {
        for (int i = filho->total; i >= 0; --i) {
            filho->filhos[i + 1] = filho->filhos[i];
        }
    }

    filho->chaves[0] = no->chaves[indice - 1];
    if (filho->filhos[0] != NULL) {
        filho->filhos[0] = irmao->filhos[irmao->total];
        if(filho->filhos[0]) filho->filhos[0]->pai = filho;
    }

    no->chaves[indice - 1] = irmao->chaves[irmao->total - 1];

    filho->total++;
    irmao->total--;
}

//(Dir -> Esq)
void rotacaoEsquerda(ArvoreB *arvore, No *no, int indice) {
    No *filho = no->filhos[indice];
    No *irmao = no->filhos[indice + 1];

    filho->chaves[filho->total] = no->chaves[indice];

    if (filho->filhos[0] != NULL) {
        filho->filhos[filho->total + 1] = irmao->filhos[0];
        if(filho->filhos[filho->total + 1])
            filho->filhos[filho->total + 1]->pai = filho;
    }

    // Irmão sobe para o pai
    no->chaves[indice] = irmao->chaves[0];

    // Ajusta o irmão (remove o primeiro)
    for (int i = 1; i < irmao->total; ++i)
        irmao->chaves[i - 1] = irmao->chaves[i];

    if (irmao->filhos[0] != NULL) {
        for (int i = 1; i <= irmao->total; ++i)
            irmao->filhos[i - 1] = irmao->filhos[i];
    }

    filho->total++;
    irmao->total--;
}

// garante a ordem antes de descer
void preenche(ArvoreB *arvore, No *no, int indice, int *esforco) {
    if (indice != 0 && no->filhos[indice - 1]->total >= arvore->ordem)
        rotacaoDireita(arvore, no, indice);
    else if (indice != no->total && no->filhos[indice + 1]->total >= arvore->ordem)
        rotacaoEsquerda(arvore, no, indice);
    else {
        if (indice != no->total)
            merge(arvore, no, indice, esforco);
        else
            merge(arvore, no, indice - 1, esforco);
    }
}

// Caso 1: Remove de uma folha
void removeDeFolha(No *no, int indice, int *esforco) {
    (*esforco)++;
    for (int i = indice + 1; i < no->total; ++i) {
        no->chaves[i - 1] = no->chaves[i];
        (*esforco)++;
    }
    no->total--;
}

// Caso 2: Remove de um nó interno, 3 variantes
void removeDeNaoFolha(ArvoreB *arvore, No *no, int indice, int *esforco) {
    int k = no->chaves[indice];

    (*esforco)++;
    // Caso 2a: Se o filho à esquerda tem chaves suficientes
    if (no->filhos[indice]->total >= arvore->ordem) {
        int pred = pegaPredecessor(arvore, no, indice, esforco);
        no->chaves[indice] = pred;
        removeChave(arvore, no->filhos[indice], pred, esforco);
    }
    // Caso 2b: Se o filho à direita tem chaves suficientes
    else if (no->filhos[indice + 1]->total >= arvore->ordem) {
        (*esforco)++;
        int suc = pegaSucessor(arvore, no, indice, esforco);
        no->chaves[indice] = suc;
        removeChave(arvore, no->filhos[indice + 1], suc, esforco);
    }
    // Caso 2c: Ambos tem poucas chaves -> Merge
    else {
        merge(arvore, no, indice, esforco);
        removeChave(arvore, no->filhos[indice], k, esforco);
    }
}

void removeChave(ArvoreB *arvore, No *no, int chave, int *esforco) {
    int indice = pesquisaBinaria(no, chave, esforco);

    int encontrou = (indice < no->total && no->chaves[indice] == chave);

    (*esforco)++;
    if (encontrou) {
        (*esforco)++;
        if (no->filhos[0] == NULL)
            removeDeFolha(no, indice, esforco);
        else
            removeDeNaoFolha(arvore, no, indice, esforco);
    }
    else {
        if (no->filhos[0] == NULL) {
            printf("A chave %d nao esta na arvore.\n", chave);
            return;
        }

        //a flag indica se a chave está na subárvore que vai sumir após um merge
        int flag = ((indice == no->total) ? 1 : 0);

        //garante que o filho que vai descer tenha chaves suficientes
        (*esforco)++;
        if (no->filhos[indice]->total < arvore->ordem)
            preenche(arvore, no, indice, esforco);

        (*esforco)++;
        if (flag && indice > no->total)
            removeChave(arvore, no->filhos[indice - 1], chave, esforco);
        else
            removeChave(arvore, no->filhos[indice], chave, esforco);
    }
}

void remover(ArvoreB *arvore, int chave, int *esforco) {
    if (arvore->raiz == NULL) return;

    removeChave(arvore, arvore->raiz, chave, esforco);

    (*esforco)++;
    if (arvore->raiz->total == 0) {
        No *aux = arvore->raiz;
        (*esforco)++;
        if (arvore->raiz->filhos[0] != NULL) {
            arvore->raiz = arvore->raiz->filhos[0];
            arvore->raiz->pai = NULL;
        }
        else {
             arvore->raiz = criaNo(arvore);
        }
        free(aux->chaves);
        free(aux->filhos);
        free(aux);
    }
}

int main()
{
    srand(time(0));

    int esforco = 0;
    int ordem = 10;
    ArvoreB *arvore = criaArvore(ordem);

    int n = 10000;            //Quantidade de valores
    const int max = 10000;   //intervalo de valores

    int numeros[max];
    for (int i = 0; i < max; i++)
    {
        numeros[i] = i;
    }

    //gerar valores aleatórios e não repetidos
    for (int i = max - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }

    for (int i = 0; i < n; i++)
    {
        adicionaChave(arvore, numeros[i], &esforco);
        //printf("Inserido: %d\n\n", numeros[i]);
    }

    imprimeEstrutura(arvore->raiz, 0);

    int x = 5; // remover x unidades

    for (int i = 0; i < x; i++)
    {
        int chave = numeros[i];
        //printf("\nRemovendo: %d\n", chave);

        remover(arvore, chave, &esforco);

        //imprimeEstrutura(arvore->raiz, 0);
    }
    /*
    if (n < max) {
        int chave_inexistente = numeros[max-1];
        remover(arvore, chave_inexistente);
    }
    */
    printf("\n--- Arvore Final ---\n");
    imprimeEstrutura(arvore->raiz, 0);

    return 0;
}