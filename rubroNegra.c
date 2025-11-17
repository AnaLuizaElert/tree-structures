#include <stdio.h>
#include <stdlib.h>

enum cores {VERMELHO, PRETO};
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

Arvore* criarArvore() {
    Arvore *arvore =  (Arvore*) malloc(sizeof(Arvore));
    No *folha = (No*) malloc(sizeof(No));
    folha->cor = PRETO;
    folha->valor = 0;
    folha->pai = NULL;
    folha->esquerda = folha;
    folha->direita = folha;

    arvore->raiz = folha;
    arvore->folha = folha;
    return arvore;
}

No* criarNo(Arvore *arvore, No *pai, int valor) {
    No* no = (No*) malloc(sizeof(No));

    no->pai = pai;
    no->valor = valor;
    no->esquerda = arvore->folha;
    no->direita = arvore->folha;
    no->cor = VERMELHO;

    return no;
}

int vazia(Arvore *arvore) {
    return (arvore->raiz == arvore->folha);
}

No* rse(No* no, Arvore *arvore, int *esforco) {
    No* direita = no->direita; //Pega o nó a direta do pai
    no->direita = direita->esquerda; //Coloca o valor a esquerda do nó direto na direita do nó pai

    (*esforco)++;
    if (direita->esquerda != arvore->folha) {
        direita->esquerda->pai = no; //O filho à esquerda de "Direita" adota um novo pai
    }

    direita->pai = no->pai; //atualiza o pai da "Direita" para seu avô

    (*esforco)++;
    if (no->pai == arvore->folha) {
        arvore->raiz = direita; //se o avô for nulo ele será a nova raiz
    } else if (no == no->pai->esquerda) {
        (*esforco)++;
        no->pai->esquerda = direita; //Muda o filho do Avô
    } else {
        (*esforco)++; //referente ao else if
        no->pai->direita = direita; //Muda o filho do Avô
    }

    direita->esquerda = no; //O nó pai virará filho à esquerda da "Direita"
    no->pai = direita; //O filho coloca a 'Direita' como pai

    return direita;
}

No* rsd(No* no, Arvore *arvore, int *esforco) {
    No* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;

    (*esforco)++;
    if (esquerda->direita != arvore->folha) {
        esquerda->direita->pai = no;
    }

    esquerda->pai = no->pai;

    (*esforco)++;
    if (no->pai == arvore->folha) {
        arvore->raiz = esquerda; //se o avô for nulo ele será a nova raiz
    } else if (no == no->pai->esquerda) {
        (*esforco)++;
        no->pai->esquerda = esquerda; //Muda o filho do Avô
    } else {
        (*esforco)++; //referente ao else if
        no->pai->direita = esquerda; //Muda o filho do Avô
    }

    esquerda->direita = no;
    no->pai = esquerda;

    return esquerda;
}

No* rde(No* no, Arvore *arvore, int *esforco) {
    no->direita = rsd(no->direita, arvore, esforco);
    return rse(no, arvore, esforco);
}

No* rdd(No* no, Arvore *arvore, int *esforco) {
    no->esquerda = rse(no->esquerda, arvore, esforco);
    return rsd(no, arvore, esforco);
}

//retornar nó pai
No* percorreAtePai(No *no, int valor, Arvore *arvore) {
    if(no->valor > valor){
        if(no->esquerda == arvore->folha) return no;
        return percorreAtePai(no->esquerda, valor, arvore);
    }

    if(no->valor < valor){
        if(no->direita == arvore->folha) return no;
        return percorreAtePai(no->direita, valor, arvore);
    }

    return no;
}

No *buscarNo(Arvore *arvore, int valor) {
    No *current = arvore->raiz;

    while (current != arvore->folha) {
        if (valor < current->valor) {
            current = current->esquerda;
        } else if (valor > current->valor) {
            current = current->direita;
        } else {
            return current;
        }
    }
    return NULL;
}

// Função auxiliar para obter a cor do nó
char obterCorChar(No *no) {
    return (no->cor == VERMELHO) ? 'R' : 'P';
}

void imprimirArvoreVertical(No *no, Arvore *arvore, char *prefixo, int isUltimo) {
    if (no == arvore->folha) {
        return;
    }

    imprimirArvoreVertical(no->direita, arvore, prefixo, 0);

    printf("%s", prefixo);
    printf(isUltimo ? "└─" : "├─");

    printf("[%d:%c]\n", no->valor, obterCorChar(no));

    char novo_prefixo[256];
    sprintf(novo_prefixo, "%s%s", prefixo, isUltimo ? "  " : "│ ");

    imprimirArvoreVertical(no->esquerda, arvore, novo_prefixo, 1);
}

void imprimirArvore(Arvore *arvore) {
    printf("\n\n*** Representação Visual (Raiz no Topo) ***\n");
    if (arvore->raiz == arvore->folha) {
        printf("A árvore está vazia.\n");
    } else {
        printf("[RAIZ] [%d:%c]\n", arvore->raiz->valor, obterCorChar(arvore->raiz));

        char prefixo_dir[256] = "|-"; // Prefixo do ramo direito
        char prefixo_esq[256] = "|-"; // Prefixo do ramo esquerdo

        imprimirArvoreVertical(arvore->raiz->direita, arvore, "|-", 0);
        imprimirArvoreVertical(arvore->raiz->esquerda, arvore, "|-", 1);
    }
    printf("****************************************\n");
}

void balancear(Arvore *arvore, No* no, int *esforco) {
    (*esforco)++;
    while (no->pai->cor == VERMELHO) {
        (*esforco)++;
        if (no->pai == arvore->raiz) break;

        No *grandpa = no->pai->pai;
        (*esforco)++;
        if (grandpa == arvore->folha) break;

        (*esforco)++;
        if (no->pai == grandpa->esquerda) {
            No* uncle = grandpa->direita;

            (*esforco)++;
            if (uncle->cor == VERMELHO) {
                uncle->cor = PRETO;
                no->pai->cor = PRETO;
                grandpa->cor = VERMELHO;
                no = grandpa;
            } else {
                (*esforco)++;
                if (no == no->pai->direita) {
                    no = no->pai;
                    rse(no, arvore, esforco);
                }
                no->pai->cor = PRETO;
                grandpa->cor = VERMELHO;
                rsd(grandpa, arvore, esforco);
            }
        } else {
            No* uncle = grandpa->esquerda;

            (*esforco)++;
            if (uncle->cor == VERMELHO) {
                uncle->cor = PRETO;
                no->pai->cor = PRETO;
                grandpa->cor = VERMELHO;
                no = grandpa;
            } else {
                (*esforco)++;
                if (no == no->pai->esquerda) {
                    no = no->pai;
                    rsd(no, arvore, esforco);
                }
                no->pai->cor = PRETO;
                grandpa->cor = VERMELHO;
                rse(grandpa, arvore, esforco);
            }
        }
        (*esforco)++;
    }
    arvore->raiz->cor = PRETO;
}

No *procuraPai(Arvore *arvore, int valor, int *esforco) {
    No *current = arvore->raiz;
    No *pai = arvore->folha;

    (*esforco)++;
    while (current != arvore->folha) {
        pai = current;

        (*esforco)++;
        if (valor < current->valor) {
            current = current->esquerda;
        } else if (valor > current->valor) {
            (*esforco)++;
            current = current->direita;
        } else {
            (*esforco)++; //referente ao else if
            return NULL;
        }
        (*esforco)++;
    }

    return pai;
}

No* adicionar(Arvore *arvore, int valor, int *esforco) {
    printf("\nAdicionando %d", valor);

    (*esforco)++;
    if(arvore->raiz == arvore->folha){
        No *no = criarNo(arvore, arvore->folha, valor);
        arvore->raiz = no;
        no->cor = PRETO;
        return no;
    }

    No *pai = procuraPai(arvore, valor, esforco);

    (*esforco)++;
    if (pai == NULL) return NULL;
    No *no = criarNo(arvore, pai, valor);

    (*esforco)++;
    if (valor < pai->valor) {
        pai->esquerda = no;
    } else {
        pai->direita = no;
    }

    balancear(arvore, no, esforco);

    return no;
}

void troca(No *noToRemove, No *noSubstitute, Arvore *arvore, int *esforco) {
    (*esforco)++;
    if (noToRemove->pai == arvore->folha) {
        arvore->raiz = noSubstitute;
    } else if (noToRemove == noToRemove->pai->esquerda) {
        (*esforco)++;
        noToRemove->pai->esquerda = noSubstitute;
    } else {
        (*esforco)++; //referente ao else if
        noToRemove->pai->direita = noSubstitute;
    }
    noSubstitute->pai = noToRemove->pai;
}

No *minimo(Arvore *arvore, No *no) {
    while (no->esquerda != arvore->folha) {
        no = no->esquerda;
    }
    return no;
}

void removerCorrecao(Arvore *arvore, No *substitute, int *esforco) {
    (*esforco)++;
    while (substitute != arvore->raiz && substitute->cor == PRETO) {
        (*esforco)++;
        if (substitute->pai->esquerda == substitute) {
            No *brother = substitute->pai->direita;
            (*esforco)++;
            if (brother->cor == VERMELHO) {
                brother->cor = PRETO;
                substitute->pai->cor = VERMELHO;
                rse(substitute->pai, arvore, esforco);
                brother = substitute->pai->direita;
            }

            (*esforco)++;
            if (brother->esquerda->cor == PRETO && brother->direita->cor == PRETO) {
                brother->cor = VERMELHO;
                substitute = substitute->pai;
            } else {
                (*esforco)++;
                if (brother->direita->cor == PRETO) {
                    brother->esquerda->cor = PRETO;
                    brother->cor = VERMELHO;
                    rsd(brother, arvore, esforco);
                    brother = substitute->pai->direita;
                }

                brother->cor = substitute->pai->cor;
                substitute->pai->cor = PRETO;
                brother->direita->cor = PRETO;
                rse(substitute->pai, arvore, esforco);
                substitute = arvore->raiz;
            }
        } else {
            No *brother = substitute->pai->esquerda;

            (*esforco)++;
            if (brother->cor == VERMELHO) {
                brother->cor = PRETO;
                substitute->pai->cor = VERMELHO;
                rsd(substitute->pai, arvore, esforco);
                brother = substitute->pai->esquerda;
            }

            (*esforco)++;
            if (brother->direita->cor == PRETO && brother->esquerda->cor == PRETO) {
                brother->cor = VERMELHO;
                substitute = substitute->pai;
            } else {
                (*esforco)++;
                if (brother->esquerda->cor == PRETO) {
                    brother->direita->cor = PRETO;
                    brother->cor = VERMELHO;
                    rsd(brother, arvore, esforco);
                    brother = substitute->pai->esquerda;
                }

                brother->cor = substitute->pai->cor;
                substitute->pai->cor = PRETO;
                brother->esquerda->cor = PRETO;
                rsd(substitute->pai, arvore, esforco);
                substitute = arvore->raiz;
            }
        }
        (*esforco)++;
    }
    substitute->cor = PRETO;
}

void remover(Arvore *arvore,No* noToRemove, int *esforco) {
    No *aux = noToRemove;
    No *substitute;
    Cor origin_cor = noToRemove->cor;

    (*esforco)++;
    if (noToRemove->esquerda == arvore->folha) {
        substitute = noToRemove->direita;
        troca(noToRemove, noToRemove->direita, arvore, esforco);
    } else if (noToRemove->direita == arvore->folha) {
        (*esforco)++;
        substitute = noToRemove->esquerda;
        troca(noToRemove, noToRemove->esquerda, arvore, esforco);
    } else {
        (*esforco)++; //referente ao else if
        aux = minimo(arvore, noToRemove->direita);
        origin_cor = aux->cor;
        substitute = aux->direita;

        (*esforco)++;
        if (aux->pai != noToRemove) {
            troca(aux, aux->direita, arvore, esforco);
            aux->direita = noToRemove->direita;
            aux->direita->pai = aux;
        }

        troca(noToRemove, aux, arvore, esforco);
        aux->esquerda = noToRemove->esquerda;
        aux->esquerda->pai = aux;
        aux->cor = noToRemove->cor;
    }

    (*esforco)++;
    if (origin_cor == PRETO) {
        removerCorrecao(arvore, substitute, esforco);
    }

    free(noToRemove);
}

int main() {
    Arvore* arvore = criarArvore();
    int esforco = 0;

    printf("\n\n****** Adicionando nos de 1 a 9 ******");
    for(int i = 1; i < 10; i++){
        adicionar(arvore, i, &esforco);
    };

    imprimirArvore(arvore);
    printf("Esforco Computacional Total (Adicao): %d \n", esforco);

    printf("\n\n****** Deletando no (Valor 6) ******");
    esforco = 0;

    No *noRemove = buscarNo(arvore, 6);

    if (noRemove != NULL) {
        printf("\nRemovendo %d...", noRemove->valor);
        remover(arvore, noRemove, &esforco);
    } else {
        printf("\nErro: Nó com valor 6 não encontrado para remoção.");
    }

    imprimirArvore(arvore);
    printf("Esforco Computacional Total (Remocao): %d \n", esforco);

    return 0;
}
