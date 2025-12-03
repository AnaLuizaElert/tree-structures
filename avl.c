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

ArvoreAVL* criarAVL() {
    ArvoreAVL *arv = NULL;
    arv = (ArvoreAVL*) malloc(sizeof(ArvoreAVL));
    arv->root = NULL;
    return arv;
}

int vaziaAVL(ArvoreAVL *arv) {
    return (arv->root == NULL);
}

int alturaAVL(NoAVL *noAVL, int *esforco){
    (*esforco)++;
    if (noAVL == NULL) {
        return -1;
    }
    int alturaAVL_esquerda = alturaAVL(noAVL->esquerda, esforco);
    int alturaAVL_direita = alturaAVL(noAVL->direita, esforco);
    (*esforco)++;
    return 1 + (alturaAVL_esquerda > alturaAVL_direita ? alturaAVL_esquerda : alturaAVL_direita);
}

int fbAVL(NoAVL *noAVL, int *esforco){
    (*esforco)++;
    if (noAVL == NULL) {
        return 0;
    }
    return alturaAVL(noAVL->esquerda, esforco) - alturaAVL(noAVL->direita, esforco);
}

NoAVL* rseAVL(NoAVL* noAVL, int *esforco) {
    NoAVL* pai = noAVL->pai;
    NoAVL* direita = noAVL->direita;

    noAVL->direita = direita->esquerda;
    (*esforco)++;
    if (noAVL->direita != NULL) {
        noAVL->direita->pai = noAVL;
    }
    direita->esquerda = noAVL;
    noAVL->pai = direita;
    direita->pai = pai;
    (*esforco)++;
    if (pai != NULL) {
        if (noAVL == pai->direita) {
            (*esforco)++;
            pai->direita = direita;
        } else {
            (*esforco)++;
            pai->esquerda = direita;
        }
    }
    return direita;
}

NoAVL* rsdAVL(NoAVL* noAVL, int *esforco) {
    NoAVL* pai = noAVL->pai;
    NoAVL* esquerda = noAVL->esquerda;

    noAVL->esquerda = esquerda->direita;
    (*esforco)++;
    if (noAVL->esquerda != NULL) {
        noAVL->esquerda->pai = noAVL;
    }
    esquerda->direita = noAVL;
    noAVL->pai = esquerda;
    esquerda->pai = pai;
    (*esforco)++;
    if (pai != NULL) {
        if (noAVL == pai->esquerda) {
            (*esforco)++;
            pai->esquerda = esquerda;
        } else {
            (*esforco)++;
            pai->direita = esquerda;
        }
    }
    return esquerda;
}

NoAVL* rdeAVL(NoAVL* noAVL, int *esforco) {
    noAVL->direita = rsdAVL(noAVL->direita, esforco);
    return rseAVL(noAVL, esforco);
}

NoAVL* rddAVL(NoAVL* noAVL, int *esforco) {
    noAVL->esquerda = rseAVL(noAVL->esquerda, esforco);
    return rsdAVL(noAVL, esforco);
}

void balancearAVL(ArvoreAVL *arv, NoAVL *noAVL, int *esforco) {
    while (noAVL != NULL) {
        noAVL->fb = fbAVL(noAVL, esforco);
        (*esforco) += 2;
        if (noAVL->fb > 1) {
            if (fbAVL(noAVL->esquerda, esforco) < 0) {
                (*esforco)++;
                noAVL = rddAVL(noAVL, esforco);
            } else {
                (*esforco)++;
                noAVL = rsdAVL(noAVL, esforco);
            }
        } else if (noAVL->fb < -1) {
            (*esforco)++;
            if (fbAVL(noAVL->direita, esforco) > 0) {
                (*esforco)++;
                noAVL = rdeAVL(noAVL, esforco);
            } else {
                (*esforco)++;
                noAVL = rseAVL(noAVL, esforco);
            }
        }
        (*esforco)++;
        if (noAVL->pai == NULL) {
            arv->root = noAVL;
        }
        noAVL = noAVL->pai;
    }
}

NoAVL* buscaNoAVL(NoAVL *noAVL, int valor, int *esforco) {
    (*esforco)++;
    if(noAVL == NULL) return NULL; // Adicionado verificação de NULL
    (*esforco)++;
    if(noAVL->valor == valor) return noAVL;
    (*esforco)++;
    if(noAVL->valor > valor){
        return buscaNoAVL(noAVL->esquerda, valor, esforco);
    }
    return buscaNoAVL(noAVL->direita, valor, esforco);
}

NoAVL* noTrocaAVL(NoAVL *raiz, int *esforco) {
    NoAVL* atual = raiz;
    while (atual != NULL && atual->esquerda != NULL) { // Adicionado verificação de NULL
        (*esforco)++;
        atual = atual->esquerda;
    }
    return atual;
}

NoAVL* adicionarAVL(ArvoreAVL *arv, int valor, int *esforco) {
    NoAVL *noAVL = malloc(sizeof(NoAVL));
    noAVL->esquerda = NULL;
    noAVL->direita = NULL;
    noAVL->valor = valor;
    noAVL->fb = 0; // Futuramente 'noAVL->alturaAVL = 0'
    (*esforco)++;
    if(arv->root == NULL){
        arv->root = noAVL;
        noAVL->pai = NULL;
        return noAVL;
    }

    NoAVL *atual = arv->root;
    NoAVL *pai = NULL;
    while (atual != NULL) {
        pai = atual;
        (*esforco)++;
        if (valor < atual->valor) {
            atual = atual->esquerda;
        } else if (valor > atual->valor) {
            (*esforco)++;
            atual = atual->direita;
        } else {
            (*esforco)++;
            free(noAVL);
            return NULL; // Valor duplicado
        }
    }

    noAVL->pai = pai;
    (*esforco)++;
    if (valor < pai->valor) {
        pai->esquerda = noAVL;
    } else {
        (*esforco)++;
        pai->direita = noAVL;
    }

    balancearAVL(arv, noAVL, esforco); // Começa a balancearAVL do nó adicionado
    return noAVL;
}

void removerAVL(ArvoreAVL *arv, int valor, int *esforco) {
    NoAVL *noAVLRemover = buscaNoAVL(arv->root, valor, esforco);
    (*esforco)++;
    if (noAVLRemover == NULL) return;

    NoAVL *pai = noAVLRemover->pai;
    NoAVL *noAVLParaBalancear = NULL; // Nó que iniciará o balanceamento

    (*esforco)++;
    //Caso 1: Nó é folha
    if (noAVLRemover->esquerda == NULL && noAVLRemover->direita == NULL) {
        noAVLParaBalancear = pai; // Balanceia a partir do pai
        (*esforco)++;
        if (pai == NULL) {
            arv->root = NULL;
        } else if (pai->esquerda == noAVLRemover) {
            (*esforco)++;
            pai->esquerda = NULL;
        } else {
            (*esforco)++;
            pai->direita = NULL;
        }
        free(noAVLRemover);

    //Caso 3: Dois filhos
    } else if (noAVLRemover->esquerda != NULL && noAVLRemover->direita != NULL) {
        (*esforco)++;
        NoAVL *sucessor = noTrocaAVL(noAVLRemover->direita, esforco);
        noAVLRemover->valor = sucessor->valor; // Copia o valor

        // Agora, remove o *sucessor* (que é Caso 1 ou 2)
        NoAVL *paiDoSucessor = sucessor->pai;
        NoAVL *filhoDoSucessor = sucessor->direita;

        noAVLParaBalancear = paiDoSucessor; // Balanceia a partir do pai do sucessor
        (*esforco)++;
        if (paiDoSucessor == noAVLRemover) {
            paiDoSucessor->direita = filhoDoSucessor;
        } else {
            paiDoSucessor->esquerda = filhoDoSucessor;
        }
        (*esforco)++;
        if (filhoDoSucessor != NULL) {
            filhoDoSucessor->pai = paiDoSucessor;
        }
        free(sucessor);

    //Caso 2: Um filho
    } else {
        (*esforco)++;
        noAVLParaBalancear = pai; // Balanceia a partir do pai
        (*esforco)++;
        NoAVL *filho = (noAVLRemover->esquerda != NULL) ? noAVLRemover->esquerda : noAVLRemover->direita;
        (*esforco)++;
        if (pai == NULL) {
            arv->root = filho;
            filho->pai = NULL;
        } else if (pai->esquerda == noAVLRemover) {
            (*esforco)++;
            pai->esquerda = filho;
            filho->pai = pai;
        } else {
            (*esforco)++;
            pai->direita = filho;
            filho->pai = pai;
        }
        free(noAVLRemover);
    }

    balancearAVL(arv, noAVLParaBalancear, esforco);
}

 // int main() {
 //     ArvoreAVL* arv = criarAVL();
 //    int esforco = 0;
 //     for(int i = 1; i < 10; i++){
 //         adicionarAVL(arv, i, &esforco);
 //         printf("\n");
 //     };
 //     removerAVL(arv, 6, &esforco);
 //     removerAVL(arv, 3, &esforco);
 //     adicionarAVL(arv, 6, &esforco);
 //     return 0;
 // }