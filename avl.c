#include <stdio.h>
#include <stdlib.h>

typedef struct noAVL {
    int valor;
    struct noAVL* pai;
    struct noAVL* direita;
    struct noAVL* esquerda;
    int fb;
    int altura;
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

// Função auxiliar para pegar altura segura (trata NULL)
int get_altura(NoAVL *no) {
    if (no == NULL) return -1;
    return no->altura;
}

// Função para atualizar altura baseada nos filhos
void atualizar_altura(NoAVL *no, long long *esforco) {
    int he = get_altura(no->esquerda);
    int hd = get_altura(no->direita);
    no->altura = (he > hd ? he : hd) + 1;
}

int vaziaAVL(ArvoreAVL *arv) {
    return (arv->root == NULL);
}

// Agora alturaAVL é O(1) apenas lendo o valor armazenado
int alturaAVL(NoAVL *noAVL, long long *esforco){
    (*esforco)++;
    return get_altura(noAVL);
}

int fbAVL(NoAVL *noAVL, long long *esforco){
    (*esforco)++;
    if (noAVL == NULL) return 0;
    return get_altura(noAVL->esquerda) - get_altura(noAVL->direita);
}

// Rotações atualizadas para corrigir alturas
NoAVL* rseAVL(NoAVL* noAVL, long long *esforco) {
    NoAVL* pai = noAVL->pai; (*esforco)++;
    NoAVL* direita = noAVL->direita; (*esforco)++;

    noAVL->direita = direita->esquerda; (*esforco)++;
    (*esforco)++;
    if (noAVL->direita != NULL) {
        noAVL->direita->pai = noAVL; (*esforco)++;
    }
    direita->esquerda = noAVL; (*esforco)++;
    noAVL->pai = direita; (*esforco)++;
    direita->pai = pai; (*esforco)++;
    (*esforco)++;
    if (pai != NULL) {
        (*esforco)++;
        if (noAVL == pai->direita) {
            pai->direita = direita; (*esforco)++;
        } else {
            pai->esquerda = direita; (*esforco)++;
        }
    }

    // Atualiza alturas
    atualizar_altura(noAVL, esforco);
    atualizar_altura(direita, esforco);

    return direita;
}

NoAVL* rsdAVL(NoAVL* noAVL, long long *esforco) {
    NoAVL* pai = noAVL->pai; (*esforco)++;
    NoAVL* esquerda = noAVL->esquerda; (*esforco)++;

    noAVL->esquerda = esquerda->direita; (*esforco)++;
    (*esforco)++;
    if (noAVL->esquerda != NULL) {
        noAVL->esquerda->pai = noAVL; (*esforco)++;
    }
    esquerda->direita = noAVL; (*esforco)++;
    noAVL->pai = esquerda; (*esforco)++;
    esquerda->pai = pai; (*esforco)++;
    (*esforco)++;
    if (pai != NULL) {
        (*esforco)++;
        if (noAVL == pai->esquerda) {
            pai->esquerda = esquerda; (*esforco)++;
        } else {
            pai->direita = esquerda; (*esforco)++;
        }
    }

    // Atualiza alturas
    atualizar_altura(noAVL, esforco);
    atualizar_altura(esquerda, esforco);

    return esquerda;
}

NoAVL* rdeAVL(NoAVL* noAVL, long long *esforco) {
    noAVL->direita = rsdAVL(noAVL->direita, esforco); (*esforco)++;
    return rseAVL(noAVL, esforco);
}

NoAVL* rddAVL(NoAVL* noAVL, long long *esforco) {
    noAVL->esquerda = rseAVL(noAVL->esquerda, esforco); (*esforco)++;
    return rsdAVL(noAVL, esforco);
}

void balancearAVL(ArvoreAVL *arv, NoAVL *noAVL, long long *esforco) {
    while (noAVL != NULL) {
        atualizar_altura(noAVL, esforco); // Mantém altura correta subindo a árvore
        int fb = fbAVL(noAVL, esforco);

        (*esforco)++;
        if (fb > 1) {
            (*esforco)++;
            if (fbAVL(noAVL->esquerda, esforco) < 0) {
                noAVL = rddAVL(noAVL, esforco); (*esforco)++;
            } else {
                noAVL = rsdAVL(noAVL, esforco); (*esforco)++;
            }
        } else if (fb < -1) {
            (*esforco)+=2;
            if (fbAVL(noAVL->direita, esforco) > 0) {
                noAVL = rdeAVL(noAVL, esforco); (*esforco)++;
            } else {
                noAVL = rseAVL(noAVL, esforco); (*esforco)++;
            }
        } else {
            (*esforco)++; //referente ao else if
        }
        (*esforco)++;
        if (noAVL->pai == NULL) {
            arv->root = noAVL; (*esforco)++;
        }
        noAVL = noAVL->pai; (*esforco)++;
        (*esforco)++;
    }
}

NoAVL* buscaNoAVL(NoAVL *noAVL, int valor, long long *esforco) {
    (*esforco)++;
    if(noAVL == NULL) return NULL;
    (*esforco)++;
    if(noAVL->valor == valor) return noAVL;

    (*esforco)++;
    if(valor < noAVL->valor)
        return buscaNoAVL(noAVL->esquerda, valor, esforco);
    return buscaNoAVL(noAVL->direita, valor, esforco);
}

NoAVL* noTrocaAVL(NoAVL *raiz, long long *esforco) {
    NoAVL* atual = raiz;
    while (atual != NULL && atual->esquerda != NULL) {
        atual = atual->esquerda; (*esforco)++;
        (*esforco)++;
    }
    return atual;
}

NoAVL* adicionarAVL(ArvoreAVL *arv, int valor, long long *esforco) {
    NoAVL *noAVL = (NoAVL*)malloc(sizeof(NoAVL));
    noAVL->esquerda = NULL;
    noAVL->direita = NULL;
    noAVL->valor = valor;
    noAVL->fb = 0;
    noAVL->altura = 0; // Altura inicial de folha é 0

    (*esforco)++;
    if(arv->root == NULL){
        arv->root = noAVL; (*esforco)++;
        noAVL->pai = NULL; (*esforco)++;
        return noAVL;
    }

    NoAVL *atual = arv->root; (*esforco)++;
    NoAVL *pai = NULL; (*esforco)++;
    while (atual != NULL) {
        pai = atual; (*esforco)++;
        (*esforco)++;
        if (valor < atual->valor) {
            atual = atual->esquerda; (*esforco)++;
        } else if (valor > atual->valor) {
            (*esforco)++;
            atual = atual->direita; (*esforco)++;
        } else {
            (*esforco)++; //referente ao else if
            free(noAVL);
            return NULL;
        }
        (*esforco)++;
    }

    noAVL->pai = pai; (*esforco)++;
    (*esforco)++;
    if (valor < pai->valor) {
        pai->esquerda = noAVL; (*esforco)++;
    } else {
        pai->direita = noAVL; (*esforco)++;
    }

    balancearAVL(arv, noAVL, esforco);
    return noAVL;
}

void removerAVL(ArvoreAVL *arv, int valor, long long *esforco) {
    NoAVL *noAVLRemover = buscaNoAVL(arv->root, valor, esforco);
    (*esforco)++;
    if (noAVLRemover == NULL) return;

    NoAVL *pai = noAVLRemover->pai; (*esforco)++;
    NoAVL *noAVLParaBalancear = NULL; (*esforco)++;

    // Caso 1: Folha
    (*esforco)++;
    if (noAVLRemover->esquerda == NULL && noAVLRemover->direita == NULL) {
        noAVLParaBalancear = pai; (*esforco)++;
        (*esforco)++;
        if (pai == NULL) {
            arv->root = NULL; (*esforco)++;
        } else if (pai->esquerda == noAVLRemover) {
            (*esforco)++;
            pai->esquerda = NULL; (*esforco)++;
        } else {
            (*esforco)++; //referente ao else if
            pai->direita = NULL; (*esforco)++;
        }
        free(noAVLRemover);
    }
    // Caso 3: Dois filhos
    else if (noAVLRemover->esquerda != NULL && noAVLRemover->direita != NULL) {
        (*esforco)++;
        NoAVL *sucessor = noTrocaAVL(noAVLRemover->direita, esforco); (*esforco)++;
        noAVLRemover->valor = sucessor->valor; (*esforco)++;

        NoAVL *paiDoSucessor = sucessor->pai; (*esforco)++;
        NoAVL *filhoDoSucessor = sucessor->direita; (*esforco)++;

        noAVLParaBalancear = paiDoSucessor; (*esforco)++;
        // Se o sucessor é filho direto do nó a remover, o pai para balancear é o próprio nó (agora com valor trocado)
        (*esforco)++;
        if (paiDoSucessor == noAVLRemover) {
            noAVLParaBalancear = noAVLRemover; (*esforco)++;
        }

        (*esforco)++;
        if (paiDoSucessor->esquerda == sucessor) {
            paiDoSucessor->esquerda = filhoDoSucessor; (*esforco)++;
        } else {
            paiDoSucessor->direita = filhoDoSucessor; (*esforco)++;
        }

        (*esforco)++;
        if (filhoDoSucessor != NULL) {
            filhoDoSucessor->pai = paiDoSucessor; (*esforco)++;
        }
        free(sucessor);
    }
    // Caso 2: Um filho
    else {
        (*esforco)++; //referente ao else if
        noAVLParaBalancear = pai; (*esforco)++;
        (*esforco)++;
        NoAVL *filho = (noAVLRemover->esquerda != NULL) ? noAVLRemover->esquerda : noAVLRemover->direita;

        (*esforco)++;
        if (pai == NULL) {
            arv->root = filho; (*esforco)++;
        } else if (pai->esquerda == noAVLRemover) {
            (*esforco)++;
            pai->esquerda = filho; (*esforco)++;
        } else {
            (*esforco)++;
            pai->direita = filho; (*esforco)++;
        }

        (*esforco)++;
        if (filho != NULL) filho->pai = pai;
        free(noAVLRemover);
    }

    (*esforco)++;
    if (noAVLParaBalancear != NULL) {
        balancearAVL(arv, noAVLParaBalancear, esforco);
    }
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
