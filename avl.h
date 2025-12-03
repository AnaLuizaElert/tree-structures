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

int alturaAVL(NoAVL *noAVL);
int fbAVL(NoAVL *noAVL);
NoAVL* buscaNoAVL(NoAVL *noAVL, int valor);
NoAVL* noTrocaAVL(NoAVL *raiz);

NoAVL* adicionarAVL(ArvoreAVL *arv, int valor);
void removerAVL(ArvoreAVL *arv, int valor);

void balancearAVL(ArvoreAVL *arv, NoAVL *noAVL);
NoAVL* rseAVL(NoAVL* noAVL);
NoAVL* rsdAVL(NoAVL* noAVL);
NoAVL* rdeAVL(NoAVL* noAVL);
NoAVL* rddAVL(NoAVL* noAVL);

void percorrePrintAVL(NoAVL *noAVL);

#endif