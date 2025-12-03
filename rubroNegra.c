#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rubroNegra.h"

#define MAX_LARGURA_RB 256
#define MAX_ALTURA_RB 50

char _buffer[MAX_ALTURA_RB][MAX_LARGURA_RB];

int alturaNoRB(NoRB *noRB, NoRB *folha) {
    if (noRB == folha) return 0;
    int esq = alturaNoRB(noRB->esquerda, folha);
    int dir = alturaNoRB(noRB->direita, folha);
    return 1 + (esq > dir ? esq : dir);
}

void limparBufferRB(int altura_max) {
    for (int i = 0; i < altura_max; i++) {
        for (int j = 0; j < MAX_LARGURA_RB; j++) _buffer[i][j] = ' ';
        _buffer[i][MAX_LARGURA_RB - 1] = '\0';
    }
}

char obterCorCharRB(NoRB *noRB) {
    return (noRB->cor == VERMELHO) ? 'R' : 'P';
}

int desenhaNoRB(NoRB *noRB, ArvoreRB *arvoreRB, int linha, int coluna) {
    if (noRB == arvoreRB->folha) return 3;

    int largura_esq = desenhaNoRB(noRB->esquerda, arvoreRB, linha + 2, coluna);
    int largura_dir = desenhaNoRB(noRB->direita, arvoreRB, linha + 2, coluna + largura_esq - 1);
    int centro = coluna + largura_esq - 1;
    char str[15];
    snprintf(str, sizeof(str), "[%d:%c]", noRB->valor, obterCorCharRB(noRB));
    int len = strlen(str);
    int inicio_texto = centro - (len / 2);
    if (inicio_texto < 0) inicio_texto = 0;
    if (linha < MAX_ALTURA_RB && inicio_texto + len < MAX_LARGURA_RB)
        strncpy(&_buffer[linha][inicio_texto], str, len);

    if (noRB->esquerda != arvoreRB->folha) {
        int centro_esq = centro - 1;
        if (linha + 1 < MAX_ALTURA_RB && centro_esq >= 0) _buffer[linha + 1][centro_esq] = '/';
        for (int i = inicio_texto - 1; i > centro_esq; i--)
             if (linha < MAX_ALTURA_RB && i >= 0) _buffer[linha][i] = '-';
    }
    if (noRB->direita != arvoreRB->folha) {
        int centro_dir = centro + 1;
        if (linha + 1 < MAX_ALTURA_RB && centro_dir < MAX_LARGURA_RB) _buffer[linha + 1][centro_dir] = '\\';
        for (int i = inicio_texto + len; i < centro_dir; i++)
             if (linha < MAX_ALTURA_RB && i < MAX_LARGURA_RB) _buffer[linha][i] = '-';
    }
    return largura_esq + largura_dir - 1;
}

void imprimirArvoreRB(ArvoreRB *arvoreRB) {
    if (arvoreRB->raiz == arvoreRB->folha) {
        printf("\nA arvore esta vazia.\n");
        return;
    }
    int h = alturaNoRB(arvoreRB->raiz, arvoreRB->folha);
    limparBufferRB(h * 2 + 1);
    desenhaNoRB(arvoreRB->raiz, arvoreRB, 0, 0);
    printf("\n--- Visualizacao ASCII ---\n");
    for (int i = 0; i < h * 2 + 1; i++) {
        if (_buffer[i][0] == '\0') break;
        printf("%s\n", _buffer[i]);
    }
    printf("--------------------------\n");
}

/* Lógica */

ArvoreRB* criarArvoreRB() {
    ArvoreRB *arvoreRB = (ArvoreRB*) malloc(sizeof(ArvoreRB));
    NoRB *folha = (NoRB*) malloc(sizeof(NoRB));
    folha->cor = PRETO;
    folha->valor = 0;
    folha->pai = NULL;
    folha->esquerda = folha;
    folha->direita = folha;
    arvoreRB->folha = folha;
    arvoreRB->raiz = folha;
    return arvoreRB;
}

NoRB* criarNoRB(ArvoreRB *arvoreRB, NoRB *pai, int valor) {
    NoRB* noRB = (NoRB*) malloc(sizeof(NoRB));
    noRB->pai = pai;
    noRB->valor = valor;
    noRB->esquerda = arvoreRB->folha;
    noRB->direita = arvoreRB->folha;
    noRB->cor = VERMELHO;
    return noRB;
}

int vaziaRB(ArvoreRB *arvoreRB) {
    return (arvoreRB->raiz == arvoreRB->folha);
}

void rseRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    NoRB* direita = noRB->direita;
    noRB->direita = direita->esquerda;
    (*esforco)++;
    if (direita->esquerda != arvoreRB->folha) {
        direita->esquerda->pai = noRB; (*esforco)++;
    }
    direita->pai = noRB->pai; (*esforco)++;
    (*esforco)++;
    if (noRB->pai == arvoreRB->folha) {
        arvoreRB->raiz = direita; (*esforco)++;
    } else if (noRB == noRB->pai->esquerda) {
        noRB->pai->esquerda = direita; (*esforco)+=2;
    } else {
        noRB->pai->direita = direita; (*esforco)+=2;
    }
    direita->esquerda = noRB; (*esforco)++;
    noRB->pai = direita; (*esforco)++;
}

void rsdRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    NoRB* esquerda = noRB->esquerda;
    noRB->esquerda = esquerda->direita;
    (*esforco)++;
    if (esquerda->direita != arvoreRB->folha) {
        esquerda->direita->pai = noRB; (*esforco)++;
    }
    esquerda->pai = noRB->pai; (*esforco)++;
    (*esforco)++;
    if (noRB->pai == arvoreRB->folha) {
        arvoreRB->raiz = esquerda; (*esforco)++;
    } else if (noRB == noRB->pai->esquerda) {
        noRB->pai->esquerda = esquerda; (*esforco)+=2;
    } else {
        noRB->pai->direita = esquerda; (*esforco)+=2;
    }
    esquerda->direita = noRB; (*esforco)++;
    noRB->pai = esquerda; (*esforco)++;
}

NoRB *buscarNoRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    NoRB *current = arvoreRB->raiz; (*esforco)++;
    while (current != arvoreRB->folha) {
        (*esforco)++;
        if (valor < current->valor) {
            current = current->esquerda; (*esforco)++;
        } else if (valor > current->valor) {
            current = current->direita; (*esforco)+=2;
        } else {
            return current;
        }
        (*esforco)++;
    }
    return NULL;
}

void balancearRB(ArvoreRB *arvoreRB, NoRB* noRB, long long *esforco) {
    while (noRB->pai->cor == VERMELHO) {
        (*esforco)++;
        if (noRB->pai == arvoreRB->raiz) break;

        NoRB *pai = noRB->pai; (*esforco)++;
        NoRB *avo = pai->pai; (*esforco)++;

        (*esforco)++;
        if (pai == avo->esquerda) {
            NoRB* tio = avo->direita; (*esforco)++;
            (*esforco)++;
            if (tio->cor == VERMELHO) {
                tio->cor = PRETO; (*esforco)++;
                pai->cor = PRETO; (*esforco)++;
                avo->cor = VERMELHO; (*esforco)++;
                noRB = avo; (*esforco)++;
            } else {
                (*esforco)++;
                if (noRB == pai->direita) {
                    noRB = pai; (*esforco)++;
                    rseRB(noRB, arvoreRB, esforco);
                    pai = noRB->pai;
                }
                pai->cor = PRETO; (*esforco)++;
                avo->cor = VERMELHO; (*esforco)++;
                rsdRB(avo, arvoreRB, esforco);
            }
        } else {
            NoRB* tio = avo->esquerda; (*esforco)++;
            (*esforco)++;
            if (tio->cor == VERMELHO) {
                tio->cor = PRETO; (*esforco)++;
                pai->cor = PRETO; (*esforco)++;
                avo->cor = VERMELHO; (*esforco)++;
                noRB = avo; (*esforco)++;
            } else {
                (*esforco)++;
                if (noRB == pai->esquerda) {
                    noRB = pai; (*esforco)++;
                    rsdRB(noRB, arvoreRB, esforco);
                    pai = noRB->pai;
                }
                pai->cor = PRETO; (*esforco)++;
                avo->cor = VERMELHO; (*esforco)++;
                rseRB(avo, arvoreRB, esforco);
            }
        }
    }
    arvoreRB->raiz->cor = PRETO; (*esforco)++;
}

NoRB* adicionarRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    (*esforco)++;
    if(arvoreRB->raiz == arvoreRB->folha){
        NoRB *noRB = criarNoRB(arvoreRB, arvoreRB->folha, valor); (*esforco)++;
        arvoreRB->raiz = noRB; (*esforco)++;
        noRB->cor = PRETO; (*esforco)++;
        return noRB;
    }

    NoRB *atual = arvoreRB->raiz; (*esforco)++;
    NoRB *pai = arvoreRB->folha; (*esforco)++;

    while (atual != arvoreRB->folha) {
        pai = atual; (*esforco)++;
        (*esforco)++;
        if (valor < atual->valor) {
            atual = atual->esquerda; (*esforco)++;
        } else if (valor > atual->valor) {
            atual = atual->direita; (*esforco)+=2;
        } else {
            return NULL;
        }
        (*esforco)++;
    }

    NoRB *noRB = criarNoRB(arvoreRB, pai, valor); (*esforco)++;
    (*esforco)++;
    if (valor < pai->valor) {
        pai->esquerda = noRB; (*esforco)++;
    } else {
        pai->direita = noRB; (*esforco)++;
    }

    balancearRB(arvoreRB, noRB, esforco);
    return noRB;
}

void trocaRB(NoRB *u, NoRB *v, ArvoreRB *arvoreRB, long long *esforco) {
    (*esforco)++;
    if (u->pai == arvoreRB->folha) {
        arvoreRB->raiz = v; (*esforco)++;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v; (*esforco)+=2;
    } else {
        u->pai->direita = v; (*esforco)+=2;
    }
    v->pai = u->pai; (*esforco)++;
}

NoRB *minimoRB(ArvoreRB *arvoreRB, NoRB *noRB, long long *esforco) {
    while (noRB->esquerda != arvoreRB->folha) {
        noRB = noRB->esquerda; (*esforco)++;
        (*esforco)++;
    }
    return noRB;
}

void removerCorrecaoRB(ArvoreRB *arvoreRB, NoRB *substitute, NoRB *paiReal, long long *esforco) {
    while (substitute != arvoreRB->raiz && substitute->cor == PRETO) {
        (*esforco)++;
        if (substitute == paiReal->esquerda) {
            NoRB *brother = paiReal->direita; (*esforco)++;
            (*esforco)++;
            if (brother->cor == VERMELHO) {
                brother->cor = PRETO; (*esforco)++;
                paiReal->cor = VERMELHO; (*esforco)++;
                rseRB(paiReal, arvoreRB, esforco);
                brother = paiReal->direita; (*esforco)++;
            }

            (*esforco)++;
            if (brother->esquerda->cor == PRETO && brother->direita->cor == PRETO) {
                if(brother != arvoreRB->folha) brother->cor = VERMELHO; (*esforco)++;
                substitute = paiReal; (*esforco)++;
                paiReal = substitute->pai; (*esforco)++;
            } else {
                (*esforco)++;
                if (brother->direita->cor == PRETO) {
                    if(brother->esquerda != arvoreRB->folha) brother->esquerda->cor = PRETO; (*esforco)++;
                    if(brother != arvoreRB->folha) brother->cor = VERMELHO; (*esforco)++;
                    rsdRB(brother, arvoreRB, esforco);
                    brother = paiReal->direita; (*esforco)++;
                }
                if(brother != arvoreRB->folha) brother->cor = paiReal->cor; (*esforco)++;
                paiReal->cor = PRETO; (*esforco)++;
                if(brother->direita != arvoreRB->folha) brother->direita->cor = PRETO; (*esforco)++;
                rseRB(paiReal, arvoreRB, esforco);
                substitute = arvoreRB->raiz; (*esforco)++;
                paiReal = substitute->pai;
            }
        } else {
            NoRB *brother = paiReal->esquerda; (*esforco)++;
            (*esforco)++;
            if (brother->cor == VERMELHO) {
                brother->cor = PRETO; (*esforco)++;
                paiReal->cor = VERMELHO; (*esforco)++;
                rsdRB(paiReal, arvoreRB, esforco);
                brother = paiReal->esquerda; (*esforco)++;
            }

            (*esforco)++;
            if (brother->direita->cor == PRETO && brother->esquerda->cor == PRETO) {
                if(brother != arvoreRB->folha) brother->cor = VERMELHO; (*esforco)++;
                substitute = paiReal; (*esforco)++;
                paiReal = substitute->pai; (*esforco)++;
            } else {
                (*esforco)++;
                if (brother->esquerda->cor == PRETO) {
                    if(brother->direita != arvoreRB->folha) brother->direita->cor = PRETO; (*esforco)++;
                    if(brother != arvoreRB->folha) brother->cor = VERMELHO; (*esforco)++;
                    rseRB(brother, arvoreRB, esforco);
                    brother = paiReal->esquerda; (*esforco)++;
                }
                if(brother != arvoreRB->folha) brother->cor = paiReal->cor; (*esforco)++;
                paiReal->cor = PRETO; (*esforco)++;
                if(brother->esquerda != arvoreRB->folha) brother->esquerda->cor = PRETO; (*esforco)++;
                rsdRB(paiReal, arvoreRB, esforco);
                substitute = arvoreRB->raiz; (*esforco)++;
                paiReal = substitute->pai;
            }
        }
        (*esforco)++;
    }
    if(substitute != arvoreRB->folha) substitute->cor = PRETO; (*esforco)++;

    arvoreRB->folha->cor = PRETO;
}

void removerRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    NoRB *z = buscarNoRB(arvoreRB, valor, esforco); (*esforco)++;
    if (z == NULL) return;

    NoRB *y = z; (*esforco)++;
    NoRB *x; (*esforco)++;
    NoRB *paiDeX;
    CorRB y_original_color = y->cor; (*esforco)++;

    (*esforco)++;
    if (z->esquerda == arvoreRB->folha) {
        x = z->direita; (*esforco)++;
        paiDeX = z->pai; (*esforco)++;
        trocaRB(z, z->direita, arvoreRB, esforco);
    } else if (z->direita == arvoreRB->folha) {
        (*esforco)++;
        x = z->esquerda; (*esforco)++;
        paiDeX = z->pai; (*esforco)++;
        trocaRB(z, z->esquerda, arvoreRB, esforco);
    } else {
        (*esforco)++;
        y = minimoRB(arvoreRB, z->direita, esforco); (*esforco)++;
        y_original_color = y->cor; (*esforco)++;
        x = y->direita; (*esforco)++;

        (*esforco)++;
        if (y->pai == z) {
            paiDeX = y; (*esforco)++;
        } else {
            paiDeX = y->pai; (*esforco)++;
            trocaRB(y, y->direita, arvoreRB, esforco);
            y->direita = z->direita; (*esforco)++;
            y->direita->pai = y; (*esforco)++;
        }

        trocaRB(z, y, arvoreRB, esforco);
        y->esquerda = z->esquerda; (*esforco)++;
        y->esquerda->pai = y; (*esforco)++;
        y->cor = z->cor; (*esforco)++;
    }

    (*esforco)++;
    if (y_original_color == PRETO) {
        removerCorrecaoRB(arvoreRB, x, paiDeX, esforco);
    }

    arvoreRB->folha->cor = PRETO;
    arvoreRB->folha->pai = NULL;
    free(z);
}