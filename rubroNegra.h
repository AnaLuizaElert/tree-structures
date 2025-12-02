#ifndef RUBRO_NEGRA_H
#define RUBRO_NEGRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum cores { VERMELHO, PRETO };
typedef enum cores Cor;

typedef struct no {
    int valor;
    Cor cor;
    struct no* pai;
    struct no* direita;
    struct no* esquerda;
} No;

typedef struct arvore {
    struct no* raiz;
    struct no* folha;
} Arvore;

Arvore* criarArvore();
No* criarNo(Arvore *arvore, No *pai, int valor);
int vazia(Arvore *arvore);

No* rse(No* no, Arvore *arvore, int *esforco);
No* rsd(No* no, Arvore *arvore, int *esforco);
No* rde(No* no, Arvore *arvore, int *esforco);
No* rdd(No* no, Arvore *arvore, int *esforco);

No* percorreAtePai(No *no, int valor, Arvore *arvore);
No *procuraPai(Arvore *arvore, int valor, int *esforco);
No *buscarNo(Arvore *arvore, int valor);
No *minimo(Arvore *arvore, No *no);
void troca(No *noToRemove, No *noSubstitute, Arvore *arvore, int *esforco);

No* adicionar(Arvore *arvore, int valor, int *esforco);
void balancear(Arvore *arvore, No* no, int *esforco);

void remover(Arvore *arvore,No* noToRemove, int *esforco);
void removerCorrecao(Arvore *arvore, No *substitute, int *esforco);

void imprimirArvore(Arvore *arvore);
char obterCorChar(No *no);
int alturaNo(No *no, No *folha);

#endif