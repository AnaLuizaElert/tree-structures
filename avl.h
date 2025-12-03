// arvoreAVL.h

#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct noAVL {
    int valor;
    struct noAVL* pai;
    struct noAVL* direita;
    struct noAVL* esquerda;
    int fb;
} NoAVL;

typedef struct arvoreAVL {
    struct noAVL* root;
} ArvoreAVL;

ArvoreAVL* criarAVL();
int vaziaAVL(ArvoreAVL *arv);

int alturaAVL(NoAVL *noAVL, long long *esforco);
int fbAVL(NoAVL *noAVL, long long *esforco);
NoAVL* buscaNoAVL(NoAVL *noAVL, int valor, long long *esforco);
NoAVL* noTrocaAVL(NoAVL *raiz, long long *esforco);

NoAVL* adicionarAVL(ArvoreAVL *arv, int valor, long long *esforco);
void removerAVL(ArvoreAVL *arv, int valor, long long *esforco);

void balancearAVL(ArvoreAVL *arv, NoAVL *noAVL, long long *esforco);
NoAVL* rseAVL(NoAVL* noAVL, long long *esforco);
NoAVL* rsdAVL(NoAVL* noAVL, long long *esforco);
NoAVL* rdeAVL(NoAVL* noAVL, long long *esforco);
NoAVL* rddAVL(NoAVL* noAVL, long long *esforco);

#endif