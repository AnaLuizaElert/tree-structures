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

int pesquisaBinariaB(NoB *noB, int chave, long long *esforco);
int localizaChaveB(ArvoreB *arvore, int chave, long long *esforco);
NoB *localizaNoB(ArvoreB *arvore, int chave, long long *esforco);

void adicionaChaveB(ArvoreB *arvore, int chave, long long *esforco);
void adicionaChaveNoB(NoB *noB, NoB *direita, int chave, long long *esforco);
void adicionaChaveRecursivoB(ArvoreB *arvore, NoB *noB, NoB *noBvo, int chave, long long *esforco);
int overflowB(ArvoreB *arvore, NoB *noB);
NoB *splitB(ArvoreB *arvore, NoB *noB, long long *esforco);

void removerB(ArvoreB *arvore, int chave, long long *esforco);
void removeChaveB(ArvoreB *arvore, NoB *noB, int chave, long long *esforco);
int pegaPredecessorB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco);
int pegaSucessorB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco);
void mergeB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco);
void rotacaoDireitaB(ArvoreB *arvore, NoB *noB, int indice);
void rotacaoEsquerdaB(ArvoreB *arvore, NoB *noB, int indice);
void preencheB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco);
void removeDeFolhaB(NoB *noB, int indice, long long *esforco);
void removeDeNaoFolhaB(ArvoreB *arvore, NoB *noB, int indice, long long *esforco);

void imprimeEstruturaB(NoB *noB, int nivel);
void imprimeChaveB(int chave);

#endif