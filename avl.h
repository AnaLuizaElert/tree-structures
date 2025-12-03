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

int alturaAVL(NoAVL *noAVL, int *esforco);
int fbAVL(NoAVL *noAVL, int *esforco);
NoAVL* buscaNoAVL(NoAVL *noAVL, int valor, int *esforco);
NoAVL* noTrocaAVL(NoAVL *raiz, int *esforco);

NoAVL* adicionarAVL(ArvoreAVL *arv, int valor, int *esforco);
void removerAVL(ArvoreAVL *arv, int valor, int *esforco);

void balancearAVL(ArvoreAVL *arv, NoAVL *noAVL, int *esforco);
NoAVL* rseAVL(NoAVL* noAVL, int *esforco);
NoAVL* rsdAVL(NoAVL* noAVL, int *esforco);
NoAVL* rdeAVL(NoAVL* noAVL, int *esforco);
NoAVL* rddAVL(NoAVL* noAVL, int *esforco);

#endif