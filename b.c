#include <stdio.h>
#include <stdlib.h>
#include "b.h"

NoB *criaNoB(ArvoreB *arvore) {
    int max = arvore->ordem * 2;
    NoB *noB = malloc(sizeof(NoB));
    noB->pai = NULL;
    noB->chaves = malloc(sizeof(int) * (max + 1));
    noB->filhos = malloc(sizeof(NoB*) * (max + 2));
    noB->total = 0;
    for (int i = 0; i < max + 2; i++) noB->filhos[i] = NULL;
    return noB;
}

ArvoreB *criaArvoreB(int ordem) {
    ArvoreB *a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNoB(a);
    return a;
}

void imprimeEstruturaB(NoB *noB, int nivel) {
    if (noB == NULL) return;
    for (int i = 0; i < nivel; i++) printf("    ");
    printf("Nivel %d: [", nivel);
    for (int i = 0; i < noB->total; i++) printf("%d%s", noB->chaves[i], i < noB->total - 1 ? ", " : "");
    printf("]\n");
    for (int i = 0; i <= noB->total; i++) imprimeEstruturaB(noB->filhos[i], nivel + 1);
}

int pesquisaBinariaB(NoB *noB, int chave, long long *esforco) {
    int inicio = 0, fim = noB->total - 1, meio;
    while (inicio <= fim) {
        (*esforco)++;
        meio = (inicio + fim) / 2;
        (*esforco)++;
        if (noB->chaves[meio] == chave) return meio;
        (*esforco)++;
        if (noB->chaves[meio] > chave) fim = meio - 1;
        else inicio = meio + 1;
        (*esforco)++;
    }
    return inicio;
}

int localizaChaveB(ArvoreB *arvore, int chave, long long *esforco) {
    NoB *noB = arvore->raiz; (*esforco)++;
    (*esforco)++;
    while (noB != NULL) {
        int i = pesquisaBinariaB(noB, chave, esforco);
        (*esforco)++;
        if (i < noB->total && noB->chaves[i] == chave) return 1;
        else noB = noB->filhos[i]; (*esforco)++;
        (*esforco)++;
    }
    return 0;
}

NoB *localizaNoB(ArvoreB *arvore, int chave, long long *esforco) {
    NoB *noB = arvore->raiz; (*esforco)++;
    while (noB != NULL) {
        int i = pesquisaBinariaB(noB, chave, esforco);
        (*esforco)++;
        if (noB->filhos[i] == NULL) return noB;
        noB = noB->filhos[i]; (*esforco)++;
        (*esforco)++;
    }
    return NULL;
}

void adicionaChaveNoB(NoB *noB, NoB *direita, int chave, long long *esforco) {
    int i = pesquisaBinariaB(noB, chave, esforco);
    // Loop de deslocamento sem esforço (justica com B)
    for (int j = noB->total - 1; j >= i; j--) {
        noB->chaves[j + 1] = noB->chaves[j];
        noB->filhos[j + 2] = noB->filhos[j + 1];
    }
    noB->chaves[i] = chave; (*esforco)++;
    noB->filhos[i + 1] = direita; (*esforco)++;
    noB->total++; (*esforco)++;
}

int overflowB(ArvoreB *arvore, NoB *noB, long long *esforco) {
    (*esforco)++;
    return noB->total > arvore->ordem * 2;
}

NoB *splitB(ArvoreB *arvore, NoB *noB, long long *esforco) {
    int meio = noB->total / 2;
    NoB *noBvo = criaNoB(arvore);
    noBvo->pai = noB->pai; (*esforco)++;

    // Loop de copia sem esforco
    for (int i = meio + 1; i < noB->total; i++) {
        noBvo->filhos[noBvo->total] = noB->filhos[i];
        noBvo->chaves[noBvo->total] = noB->chaves[i];
        if (noBvo->filhos[noBvo->total] != NULL)
            noBvo->filhos[noBvo->total]->pai = noBvo;
        noBvo->total++;
    }
    noBvo->filhos[noBvo->total] = noB->filhos[noB->total];
    if (noBvo->filhos[noBvo->total] != NULL)
        noBvo->filhos[noBvo->total]->pai = noBvo;
    noB->total = meio; (*esforco)++;
    return noBvo;
}

void adicionaChaveRecursivoB(ArvoreB *arvore, NoB *noB, NoB *noBvo, int chave, long long *esforco) {
    adicionaChaveNoB(noB, noBvo, chave, esforco);
    (*esforco)++;
    if (overflowB(arvore, noB, esforco)) {
        int promovido = noB->chaves[arvore->ordem];
        NoB *noBvo_dividido = splitB(arvore, noB, esforco); (*esforco)++;
        (*esforco)++;
        if (noB->pai == NULL) {
            NoB *raiz = criaNoB(arvore); (*esforco)++;
            raiz->filhos[0] = noB; (*esforco)++;
            adicionaChaveNoB(raiz, noBvo_dividido, promovido, esforco);
            noB->pai = raiz; (*esforco)++;
            noBvo_dividido->pai = raiz; (*esforco)++;
            arvore->raiz = raiz; (*esforco)++;
        } else {
            adicionaChaveRecursivoB(arvore, noB->pai, noBvo_dividido, promovido, esforco);
        }
    }
}

void adicionaChaveB(ArvoreB *arvore, int chave, long long *esforco) {
    NoB *noB = localizaNoB(arvore, chave, esforco); (*esforco)++;
    adicionaChaveRecursivoB(arvore, noB, NULL, chave, esforco);
}

void removeChaveB(ArvoreB *arvore, NoB *noB, int chave, long long *esforco);

int pegaPredecessorB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    NoB *atual = noB->filhos[indice]; (*esforco)++;
    while (atual->filhos[atual->total] != NULL) {
        atual = atual->filhos[atual->total]; (*esforco)++;
        (*esforco)++;
    }
    return atual->chaves[atual->total - 1];
}

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
        filho->chaves[filho->total + i] = irmao->chaves[i];
    }
    (*esforco)++;
    if (filho->filhos[0] != NULL) {
        for (int i = 0; i <= irmao->total; ++i) {
            filho->filhos[filho->total + i] = irmao->filhos[i];
            if (filho->filhos[filho->total + i])
                filho->filhos[filho->total + i]->pai = filho;
        }
    }
    filho->total += irmao->total; (*esforco)++;

    for (int i = indice; i < noB->total - 1; ++i) {
        noB->chaves[i] = noB->chaves[i + 1];
        noB->filhos[i + 1] = noB->filhos[i + 2];
    }
    noB->total--; (*esforco)++;
    free(irmao->chaves);
    free(irmao->filhos);
    free(irmao);
}

void rotacaoDireitaB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    NoB *filho = noB->filhos[indice]; (*esforco)++;
    NoB *irmao = noB->filhos[indice - 1]; (*esforco)++;

    for (int i = filho->total - 1; i >= 0; --i) {
        filho->chaves[i + 1] = filho->chaves[i];
    }
    if (filho->filhos[0] != NULL) {
        for (int i = filho->total; i >= 0; --i) {
            filho->filhos[i + 1] = filho->filhos[i];
        }
    }
    filho->chaves[0] = noB->chaves[indice - 1]; (*esforco)++;
    (*esforco)++;
    if (filho->filhos[0] != NULL) {
        filho->filhos[0] = irmao->filhos[irmao->total]; (*esforco)++;
        if(filho->filhos[0]) filho->filhos[0]->pai = filho;
    }
    noB->chaves[indice - 1] = irmao->chaves[irmao->total - 1]; (*esforco)++;
    filho->total++; (*esforco)++;
    irmao->total--; (*esforco)++;
}

void rotacaoEsquerdaB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    NoB *filho = noB->filhos[indice]; (*esforco)++;
    NoB *irmao = noB->filhos[indice + 1]; (*esforco)++;
    filho->chaves[filho->total] = noB->chaves[indice]; (*esforco)++;
    (*esforco)++;
    if (filho->filhos[0] != NULL) {
        filho->filhos[filho->total + 1] = irmao->filhos[0]; (*esforco)++;
        if(filho->filhos[filho->total + 1])
            filho->filhos[filho->total + 1]->pai = filho;
    }
    noB->chaves[indice] = irmao->chaves[0]; (*esforco)++;
    for (int i = 1; i < irmao->total; ++i) {
        irmao->chaves[i - 1] = irmao->chaves[i];
    }
    (*esforco)++;
    if (irmao->filhos[0] != NULL) {
        for (int i = 1; i <= irmao->total; ++i) {
            irmao->filhos[i - 1] = irmao->filhos[i];
        }
    }
    filho->total++; (*esforco)++;
    irmao->total--; (*esforco)++;
}

void preencheB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    (*esforco)++;
    if (indice != 0 && noB->filhos[indice - 1]->total >= arvore->ordem) {
        rotacaoDireitaB(arvore, noB, indice, esforco);
    } else if (indice != noB->total && noB->filhos[indice + 1]->total >= arvore->ordem) {
        (*esforco)++;
        rotacaoEsquerdaB(arvore, noB, indice, esforco);
    } else {
        (*esforco)+=2;
        if (indice != noB->total) mergeB(arvore, noB, indice, esforco);
        else mergeB(arvore, noB, indice - 1, esforco);
    }
}

void removeDeFolhaB(NoB *noB, int indice, long long *esforco) {
    for (int i = indice + 1; i < noB->total; ++i) {
        noB->chaves[i - 1] = noB->chaves[i];
    }
    noB->total--; (*esforco)++;
}

void removeDeNaoFolhaB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco) {
    int k = noB->chaves[indice];
    (*esforco)++;
    if (noB->filhos[indice]->total >= arvore->ordem) {
        int pred = pegaPredecessorB(arvore, noB, indice, esforco);
        noB->chaves[indice] = pred; (*esforco)++;
        removeChaveB(arvore, noB->filhos[indice], pred, esforco);
    } else if (noB->filhos[indice + 1]->total >= arvore->ordem) {
        (*esforco)++;
        int suc = pegaSucessorB(arvore, noB, indice, esforco);
        noB->chaves[indice] = suc; (*esforco)++;
        removeChaveB(arvore, noB->filhos[indice + 1], suc, esforco);
    } else {
        (*esforco)++;
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
        if (noB->filhos[0] == NULL) removeDeFolhaB(noB, indice, esforco);
        else removeDeNaoFolhaB(arvore, noB, indice, esforco);
    } else {
        if (noB->filhos[0] == NULL) return;
        int flag = ((indice == noB->total) ? 1 : 0);
        (*esforco)++;
        if (noB->filhos[indice]->total < arvore->ordem) preencheB(arvore, noB, indice, esforco);
        (*esforco)++;
        if (flag && indice > noB->total) removeChaveB(arvore, noB->filhos[indice - 1], chave, esforco);
        else removeChaveB(arvore, noB->filhos[indice], chave, esforco);
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
        } else {
             arvore->raiz = criaNoB(arvore); (*esforco)++;
        }
        free(aux->chaves);
        free(aux->filhos);
        free(aux);
    }
}