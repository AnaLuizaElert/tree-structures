// arvoreRB.h

#ifndef ARVORE_RB_H
#define ARVORE_RB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum coresRB {VERMELHO, PRETO};
typedef enum coresRB CorRB;

typedef struct noRB {
    int valor;
    CorRB cor;
    struct noRB* pai;
    struct noRB* direita;
    struct noRB* esquerda;
} NoRB;

typedef struct arvoreRB {
    struct noRB* raiz;
    struct noRB* folha;
} ArvoreRB;

ArvoreRB* criarArvoreRB();
NoRB* criarNoRB(ArvoreRB *arvoreRB, NoRB *pai, int valor);
int vaziaRB(ArvoreRB *arvoreRB);

NoRB *procuraPaiRB(ArvoreRB *arvoreRB, int valor, int *esforco);
NoRB *buscarNoRB(ArvoreRB *arvoreRB, int valor);
NoRB *minimoRB(ArvoreRB *arvoreRB, NoRB *noRB);

NoRB* adicionarRB(ArvoreRB *arvoreRB, int valor, int *esforco);
void balancearRB(ArvoreRB *arvoreRB, NoRB* noRB, int *esforco);

void removerRB(ArvoreRB *arvoreRB, NoRB* noRBToRemove, int *esforco);
void removerCorrecaoRB(ArvoreRB *arvoreRB, NoRB *substitute, int *esforco);
void trocaRB(NoRB *noRBToRemove, NoRB *noRBSubstitute, ArvoreRB *arvoreRB, int *esforco);

NoRB* rseRB(NoRB* noRB, ArvoreRB *arvoreRB, int *esforco);
NoRB* rsdRB(NoRB* noRB, ArvoreRB *arvoreRB, int *esforco);
NoRB* rdeRB(NoRB* noRB, ArvoreRB *arvoreRB, int *esforco);
NoRB* rddRB(NoRB* noRB, ArvoreRB *arvoreRB, int *esforco);

void imprimirArvoreRB(ArvoreRB *arvoreRB);

#endif