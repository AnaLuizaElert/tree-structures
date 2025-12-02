// arvoreB.h

#ifndef ARVOREB_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

ArvoreB *criaArvoreB(int ordem);
NoB *criaNoB(ArvoreB *arvore);

int pesquisaBinariaB(NoB *noB, int chave, int *esforco);
int localizaChaveB(ArvoreB *arvore, int chave, int *esforco);
NoB *localizaNoB(ArvoreB *arvore, int chave, int *esforco);

void adicionaChaveB(ArvoreB *arvore, int chave, int *esforco);
void adicionaChaveNoB(NoB *noB, NoB *direita, int chave, int *esforco);
void adicionaChaveRecursivoB(ArvoreB *arvore, NoB *noB, NoB *noBvo, int chave, int *esforco);
int overflowB(ArvoreB *arvore, NoB *noB);
NoB *splitB(ArvoreB *arvore, NoB *noB, int *esforco);

void removerB(ArvoreB *arvore, int chave, int *esforco);
void removeChaveB(ArvoreB *arvore, NoB *noB, int chave, int *esforco);
int pegaPredecessorB(ArvoreB *arvore, NoB *noB, int indice, int *esforco);
int pegaSucessorB(ArvoreB *arvore, NoB *noB, int indice, int *esforco);
void mergeB(ArvoreB *arvore, NoB *noB, int indice, int *esforco);
void rotacaoDireitaB(ArvoreB *arvore, NoB *noB, int indice);
void rotacaoEsquerdaB(ArvoreB *arvore, NoB *noB, int indice);
void preencheB(ArvoreB *arvore, NoB *noB, int indice, int *esforco);
void removeDeFolhaB(NoB *noB, int indice, int *esforco);
void removeDeNaoFolhaB(ArvoreB *arvore, NoB *noB, int indice, int *esforco);

void imprimeEstruturaB(NoB *noB, int nivel);
void imprimeChaveB(int chave);

#endif