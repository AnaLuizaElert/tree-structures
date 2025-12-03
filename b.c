#include <stdio.h>
#include <stdlib.h>
//https://www.youtube.com/watch?v=AH7zF9PuIAw
//https://www.youtube.com/watch?v=5mC6TmviBPE
//https://www.youtube.com/watch?v=zE1FMszj_1c&list=PLG3ume5wUwQrEbhmxvTR52XRinLjwOmt3
//https://www.youtube.com/watch?v=yt7PbXBaoYs
//https://www.youtube.com/watch?v=u7ATFmZ2cdc
typedef struct noB
{
    int total;
    int *chaves;
    struct noB **filhos;
    struct noB *pai;
} NoB;

typedef struct arvoreB
{
    NoB *raiz;
    int ordem;
} ArvoreB;

NoB *criaNoB(ArvoreB *arvore)
{
    int max = arvore->ordem * 2;
    NoB *noB = malloc(sizeof(NoB));
    noB->pai = NULL;

    noB->chaves = malloc(sizeof(int) * (max + 1));
    noB->filhos = malloc(sizeof(NoB*) * (max + 2));
    noB->total = 0;
    for (int i = 0; i < max + 2; i++)
        noB->filhos[i] = NULL;
    return noB;
}

ArvoreB *criaArvoreB(int ordem)
{
    ArvoreB *a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNoB(a);
    return a;
}

void imprimeEstruturaB(NoB *noB, int nivel)
{
    if (noB == NULL)
    {
        return;
    }

    for (int i = 0; i < nivel; i++)
    {
        printf("    "); // identação por nível para melhor visualização
    }

    printf("Nivel %d: [", nivel);
    for (int i = 0; i < noB->total; i++)
    {
        printf("%d", noB->chaves[i]);
        if (i < noB->total - 1)
        {
            printf(", ");
        }
    }
    printf("] ");

    if (noB->pai == NULL)
    {
        printf("(RAIZ)\n");
    }
    else
    {
        printf("(Pai: [");
        for (int i = 0; i < noB->pai->total; i++)
        {
            printf("%d", noB->pai->chaves[i]);
            if (i < noB->pai->total - 1)
            {
                printf(", ");
            }
        }
        printf("])\n");
    }

    for (int i = 0; i <= noB->total; i++)
    {
        imprimeEstruturaB(noB->filhos[i], nivel + 1);
    }
}

int pesquisaBinariaB(NoB *noB, int chave, long long *esforco)
{
    int inicio = 0, fim = noB->total - 1, meio;
    while (inicio <= fim)
    {
        meio = (inicio + fim) / 2;
        (*esforco)++;
        if (noB->chaves[meio] == chave)
        {
            return meio;
        }
        (*esforco)++;
        if (noB->chaves[meio] > chave)
        {
            fim = meio - 1;
        }
        else
        {
            inicio = meio + 1;
        }
        (*esforco)++;
    }
    return inicio;
}

int localizaChaveB(ArvoreB *arvore, int chave, long long *esforco)
{
    NoB *noB = arvore->raiz; (*esforco)++;
    (*esforco)++;
    while (noB != NULL)
    {
        int i = pesquisaBinariaB(noB, chave, esforco);
        (*esforco)++;
        if (i < noB->total && noB->chaves[i] == chave)
        {
            return 1;
        }
        else
        {
            noB = noB->filhos[i]; (*esforco)++;
        }
        (*esforco)++;
    }
    return 0;
}

NoB *localizaNoB(ArvoreB *arvore, int chave, long long *esforco)
{
    NoB *noB = arvore->raiz; (*esforco)++;

    while (noB != NULL) {
        int i = pesquisaBinariaB(noB, chave, esforco);
        (*esforco)++;
        if (noB->filhos[i] == NULL){
            return noB;
        }
        noB = noB->filhos[i]; (*esforco)++;
        (*esforco)++;
    }
    return NULL;
}

void adicionaChaveNoB(NoB *noB, NoB *direita, int chave, long long *esforco)
{
    int i = pesquisaBinariaB(noB, chave, esforco);
    for (int j = noB->total - 1; j >= i; j--)
    {
        noB->chaves[j + 1] = noB->chaves[j]; (*esforco)++;
        noB->filhos[j + 2] = noB->filhos[j + 1]; (*esforco)++;
        (*esforco)++;
    }

    noB->chaves[i] = chave; (*esforco)++;
    noB->filhos[i + 1] = direita; (*esforco)++;
    noB->total++; (*esforco)++;
}

int overflowB(ArvoreB *arvore, NoB *noB, long long *esforco)
{
    (*esforco)++;
    return noB->total > arvore->ordem * 2;
}

NoB *splitB(ArvoreB *arvore, NoB *noB, long long *esforco)
{
    int meio = noB->total / 2;
    NoB *noBvo = criaNoB(arvore);
    noBvo->pai = noB->pai; (*esforco)++;

    (*esforco)++;
    for (int i = meio + 1; i < noB->total; i++)
    {
        noBvo->filhos[noBvo->total] = noB->filhos[i]; (*esforco)++;
        noBvo->chaves[noBvo->total] = noB->chaves[i]; (*esforco)++;
        (*esforco)++;
        if (noBvo->filhos[noBvo->total] != NULL)
            noBvo->filhos[noBvo->total]->pai = noBvo; (*esforco)++;

        noBvo->total++;
    }
    noBvo->filhos[noBvo->total] = noB->filhos[noB->total]; (*esforco)++;
    (*esforco)++;
    if (noBvo->filhos[noBvo->total] != NULL)
        noBvo->filhos[noBvo->total]->pai = noBvo; (*esforco)++;
    noB->total = meio; (*esforco)++;
    return noBvo;
}

void adicionaChaveRecursivoB(ArvoreB *arvore, NoB *noB, NoB *noBvo, int chave, long long *esforco)
{
    adicionaChaveNoB(noB, noBvo, chave, esforco);
    (*esforco)++;
    if (overflowB(arvore, noB, esforco))
    {
        int promovido = noB->chaves[arvore->ordem];
        NoB *noBvo_dividido = splitB(arvore, noB, esforco); (*esforco)++;
        (*esforco)++;
        if (noB->pai == NULL)
        {
            NoB *raiz = criaNoB(arvore); (*esforco)++;
            raiz->filhos[0] = noB; (*esforco)++;
            adicionaChaveNoB(raiz, noBvo_dividido, promovido ,esforco);

            noB->pai = raiz; (*esforco)++;
            noBvo_dividido->pai = raiz; (*esforco)++;
            arvore->raiz = raiz; (*esforco)++;
        }
        else {
            adicionaChaveRecursivoB(arvore, noB->pai, noBvo_dividido, promovido, esforco);
        }
    }
}

void adicionaChaveB(ArvoreB *arvore, int chave, long long *esforco)
{
    NoB *noB = localizaNoB(arvore, chave, esforco); (*esforco)++;
    adicionaChaveRecursivoB(arvore, noB, NULL, chave, esforco);
}

void imprimeChaveB(int chave)
{
    printf("%d ", chave);
}

void removeChaveB(ArvoreB *arvore, NoB *noB, int chave, long long *esforco);

//maior chave da subárvore esquerda
int pegaPredecessorB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    NoB *atual = noB->filhos[indice]; (*esforco)++;
    while (atual->filhos[atual->total] != NULL) {
        atual = atual->filhos[atual->total]; (*esforco)++;
        (*esforco)++;
    }
    return atual->chaves[atual->total - 1];
}

//menoBr chave da subárvore direita
int pegaSucessorB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    NoB *atual = noB->filhos[indice + 1]; (*esforco)++;
    (*esforco)++;
    while (atual->filhos[0] != NULL) {
        atual = atual->filhos[0]; (*esforco)++;
        (*esforco)++;
    }
    return atual->chaves[0];
}

void mergeB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    NoB *filho = noB->filhos[indice]; (*esforco)++;
    NoB *irmao = noB->filhos[indice + 1]; (*esforco)++;

    filho->chaves[filho->total] = noB->chaves[indice]; (*esforco)++;
    filho->total++; (*esforco)++;

    for (int i = 0; i < irmao->total; ++i) {
        filho->chaves[filho->total + i] = irmao->chaves[i]; (*esforco)++;
        (*esforco)++;
    }
    (*esforco)++;
    if (filho->filhos[0] != NULL) { //não é folha
        for (int i = 0; i <= irmao->total; ++i) {
            filho->filhos[filho->total + i] = irmao->filhos[i]; (*esforco)++;
            (*esforco)++;
            if (filho->filhos[filho->total + i])
                filho->filhos[filho->total + i]->pai = filho; (*esforco)++;
            (*esforco)++;
        }
    }

    filho->total += irmao->total; (*esforco)++;

    (*esforco)++;
    for (int i = indice; i < noB->total - 1; ++i) {
        noB->chaves[i] = noB->chaves[i + 1]; (*esforco)++;
        noB->filhos[i + 1] = noB->filhos[i + 2]; (*esforco)++;
    }
    noB->total--; (*esforco)++;

    free(irmao->chaves);
    free(irmao->filhos);
    free(irmao);
}

//(Esq -> Dir)
void rotacaoDireitaB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    NoB *filho = noB->filhos[indice]; (*esforco)++;
    NoB *irmao = noB->filhos[indice - 1]; (*esforco)++;

    for (int i = filho->total - 1; i >= 0; --i) {
        filho->chaves[i + 1] = filho->chaves[i]; (*esforco)++;
        (*esforco)++;
    }

    (*esforco)++;
    if (filho->filhos[0] != NULL) {
        for (int i = filho->total; i >= 0; --i) {
            filho->filhos[i + 1] = filho->filhos[i]; (*esforco)++;
            (*esforco)++;
        }
    }

    filho->chaves[0] = noB->chaves[indice - 1]; (*esforco)++;
    (*esforco)++;
    if (filho->filhos[0] != NULL) {
        filho->filhos[0] = irmao->filhos[irmao->total]; (*esforco)++;
        (*esforco)++;
        if(filho->filhos[0]) filho->filhos[0]->pai = filho;
    }

    noB->chaves[indice - 1] = irmao->chaves[irmao->total - 1];

    filho->total++;
    irmao->total--;
}

//(Dir -> Esq)
void rotacaoEsquerdaB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    NoB *filho = noB->filhos[indice]; (*esforco)++;
    NoB *irmao = noB->filhos[indice + 1]; (*esforco)++;

    filho->chaves[filho->total] = noB->chaves[indice]; (*esforco)++;

    (*esforco)++;
    if (filho->filhos[0] != NULL) {
        filho->filhos[filho->total + 1] = irmao->filhos[0]; (*esforco)++;
        (*esforco)++;
        if(filho->filhos[filho->total + 1])
            filho->filhos[filho->total + 1]->pai = filho; (*esforco)++;
    }

    // Irmão sobe para o pai
    noB->chaves[indice] = irmao->chaves[0]; (*esforco)++;

    // Ajusta o irmão (remove o primeiro)
    for (int i = 1; i < irmao->total; ++i) {
        irmao->chaves[i - 1] = irmao->chaves[i]; (*esforco)++;
        (*esforco)++;
    }

    (*esforco)++;
    if (irmao->filhos[0] != NULL) {
        for (int i = 1; i <= irmao->total; ++i) {
            irmao->filhos[i - 1] = irmao->filhos[i]; (*esforco)++;
            (*esforco)++;
        }
    }

    filho->total++; (*esforco)++;
    irmao->total--; (*esforco)++;
}

// garante a ordem antes de descer
void preencheB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    (*esforco)++;
    if (indice != 0 && noB->filhos[indice - 1]->total >= arvore->ordem) {
        rotacaoDireitaB(arvore, noB, indice, esforco);
    } else if (indice != noB->total && noB->filhos[indice + 1]->total >= arvore->ordem) {
        (*esforco)++;
        rotacaoEsquerdaB(arvore, noB, indice, esforco);
    } else {
        (*esforco)+=2;
        if (indice != noB->total)
            mergeB(arvore, noB, indice, esforco);
        else
            mergeB(arvore, noB, indice - 1, esforco);
    }
}

// Caso 1: Remove de uma folha
void removeDeFolhaB(NoB *noB, int indice, long long *esforco) {
    for (int i = indice + 1; i < noB->total; ++i) {
        noB->chaves[i - 1] = noB->chaves[i]; (*esforco)++;
        (*esforco)++;
    }
    noB->total--; (*esforco)++;
}

// Caso 2: Remove de um nó internoB, 3 variantes
void removeDeNaoFolhaB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    int k = noB->chaves[indice];

    (*esforco)++;
    // Caso 2a: Se o filho à esquerda tem chaves suficientes
    if (noB->filhos[indice]->total >= arvore->ordem) {
        int pred = pegaPredecessorB(arvore, noB, indice, esforco);
        noB->chaves[indice] = pred; (*esforco)++;
        removeChaveB(arvore, noB->filhos[indice], pred, esforco);
    }
    // Caso 2b: Se o filho à direita tem chaves suficientes
    else if (noB->filhos[indice + 1]->total >= arvore->ordem) {
        (*esforco)++;
        int suc = pegaSucessorB(arvore, noB, indice, esforco);
        noB->chaves[indice] = suc; (*esforco)++;
        removeChaveB(arvore, noB->filhos[indice + 1], suc, esforco);
    }
    // Caso 2c: Ambos tem poucas chaves -> Merge
    else {
        (*esforco)++; //referente ao else if
        mergeB(arvore, noB, indice, esforco);
        removeChaveB(arvore, noB->filhos[indice], k, esforco);
    }
}

void removeChaveB(ArvoreB *arvore, NoB *noB, int chave, long long *esforco) {
    int indice = pesquisaBinariaB(noB, chave, esforco);
    int encontrou = (indice < noB->total && noB->chaves[indice] == chave);

    (*esforco)++;
    if (encontrou) {
        (*esforco)++;
        if (noB->filhos[0] == NULL)
            removeDeFolhaB(noB, indice, esforco);
        else
            removeDeNaoFolhaB(arvore, noB, indice, esforco);
    }
    else {
        if (noB->filhos[0] == NULL) {
            printf("A chave %d nao esta na arvore.\n", chave);
            return;
        }

        //a flag indica se a chave está na subárvore que vai sumir após um mergeB
        int flag = ((indice == noB->total) ? 1 : 0);

        //garante que o filho que vai descer tenha chaves suficientes
        (*esforco)++;
        if (noB->filhos[indice]->total < arvore->ordem)
            preencheB(arvore, noB, indice, esforco);

        (*esforco)++;
        if (flag && indice > noB->total)
            removeChaveB(arvore, noB->filhos[indice - 1], chave, esforco);
        else
            removeChaveB(arvore, noB->filhos[indice], chave, esforco);
    }
}

void removerB(ArvoreB *arvore, int chave, long long *esforco) {
    if (arvore->raiz == NULL) return;

    removeChaveB(arvore, arvore->raiz, chave, esforco);

    (*esforco)++;
    if (arvore->raiz->total == 0) {
        NoB *aux = arvore->raiz; (*esforco)++;
        (*esforco)++;
        if (arvore->raiz->filhos[0] != NULL) {
            arvore->raiz = arvore->raiz->filhos[0]; (*esforco)++;
            arvore->raiz->pai = NULL; (*esforco)++;
        }
        else {
             arvore->raiz = criaNoB(arvore); (*esforco)++;
        }
        free(aux->chaves);
        free(aux->filhos);
        free(aux);
    }
}

// int main()
// {
//     srand(time(0));
//
//     int esforco = 0;
//     int ordem = 10;
//     ArvoreB *arvore = criaArvoreB(ordem);
//
//     int n = 10000;            //Quantidade de valores
//     const int max = 10000;   //intervalo de valores
//
//     int numeros[max];
//     for (int i = 0; i < max; i++)
//     {
//         numeros[i] = i;
//     }
//
//     //gerar valores aleatórios e não repetidos
//     for (int i = max - 1; i > 0; i--)
//     {
//         int j = rand() % (i + 1);
//         int temp = numeros[i];
//         numeros[i] = numeros[j];
//         numeros[j] = temp;
//     }
//
//     for (int i = 0; i < n; i++)
//     {
//         adicionaChaveB(arvore, numeros[i], &esforco);
//         //printf("Inserido: %d\n\n", numeros[i]);
//     }
//
//     imprimeEstruturaB(arvore->raiz, 0);
//
//     int x = 5; // removerB x unidades
//
//     for (int i = 0; i < x; i++)
//     {
//         int chave = numeros[i];
//         //printf("\nRemovendo: %d\n", chave);
//
//         removerB(arvore, chave, &esforco);
//
//         //imprimeEstruturaB(arvore->raiz, 0);
//     }
//     /*
//     if (n < max) {
//         int chave_inexistente = numeros[max-1];
//         removerB(arvore, chave_inexistente);
//     }
//     */
//     printf("\n--- Arvore Final ---\n");
//     imprimeEstruturaB(arvore->raiz, 0);
//
//     return 0;
// }