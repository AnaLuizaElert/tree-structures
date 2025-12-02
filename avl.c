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

/*
Parte estética
*/

#define MAX_NIVEIS_AVL 10
#define MAX_LARGURA_AVL 80

char buffer[MAX_NIVEIS_AVL * 2][MAX_LARGURA_AVL];

void limparBufferAVL() {
    for (int i = 0; i < MAX_NIVEIS_AVL * 2; i++) {
        for (int j = 0; j < MAX_LARGURA_AVL; j++) {
            buffer[i][j] = ' ';
        }
        buffer[i][MAX_LARGURA_AVL - 1] = '\0'; // Garantir fim de string
    }
}

int desenhaNoAVL(NoAVL *noAVL, int linha, int coluna, int largura_total) {
    if (noAVL == NULL) {
        return 3; // Largura mínima para um nó 'vazio'
    }

    // 1. Chamadas Recursivas
    int largura_esq = desenhaNoAVL(noAVL->esquerda, linha + 2, coluna, largura_total);
    int largura_dir = desenhaNoAVL(noAVL->direita, linha + 2, coluna + largura_esq - 1, largura_total);

    // 2. Posição do Nó
    int centro = coluna + largura_esq - 1;
    char str[10];
    sprintf(str, "[%d]", noAVL->valor);
    int len = strlen(str);
    int inicio_texto = centro - (len / 2);
    if (inicio_texto < 0) inicio_texto = 0;
    strncpy(&buffer[linha][inicio_texto], str, len);

    // 3. Desenhar Linhas de Conexão
    if (noAVL->esquerda != NULL) {
        int centro_esq = coluna + largura_esq/2;
        buffer[linha + 1][centro_esq] = '/';
        for (int i = centro_esq + 1; i < centro; i++) {
            buffer[linha][i] = '-';
        }
    }
    if (noAVL->direita != NULL) {
        int centro_dir = coluna + largura_esq + largura_dir/2;
        buffer[linha + 1][centro_dir] = '\\';
        for (int i = centro + 1; i < centro_dir; i++) {
            buffer[linha][i] = '-';
        }
    }

    return largura_esq + largura_dir - 1;
}

int alturaAVL(NoAVL *noAVL);

void percorrePrintAVL(NoAVL *noAVL) {
    if (noAVL == NULL) {
        printf("Arv. Vazia.\n");
        return;
    }

    int alturaAVL_arvoreAVL = alturaAVL(noAVL); // Usa a função 'alturaAVL' da parte lógica
    limparBufferAVL();
    int largura_total = 1 << (alturaAVL_arvoreAVL + 1);
    desenhaNoAVL(noAVL, 0, 0, largura_total);

    printf("\n--- Visualizacao ASCII Art ---\n");
    for (int i = 0; i < alturaAVL_arvoreAVL * 2 + 1; i++) {
        if (i < MAX_NIVEIS_AVL * 2) {
            printf("%s\n", buffer[i]);
        }
    }
    printf("------------------------------\n");
}

ArvoreAVL* criarAVL() {
    ArvoreAVL *arv = NULL;
    arv = (ArvoreAVL*) malloc(sizeof(ArvoreAVL));
    arv->root = NULL;
    printf("\ncriarAVL");
    return arv;
}

int vaziaAVL(ArvoreAVL *arv) {
    return (arv->root == NULL);
}

int alturaAVL(NoAVL *noAVL){
    if (noAVL == NULL) {
        return -1;
    }
    int alturaAVL_esquerda = alturaAVL(noAVL->esquerda);
    int alturaAVL_direita = alturaAVL(noAVL->direita);
    return 1 + (alturaAVL_esquerda > alturaAVL_direita ? alturaAVL_esquerda : alturaAVL_direita);
}

int fbAVL(NoAVL *noAVL){
    if (noAVL == NULL) {
        return 0;
    }
    return alturaAVL(noAVL->esquerda) - alturaAVL(noAVL->direita);
}

NoAVL* rseAVL(NoAVL* noAVL) {
    NoAVL* pai = noAVL->pai;
    NoAVL* direita = noAVL->direita;

    noAVL->direita = direita->esquerda;
    if (noAVL->direita != NULL) {
        noAVL->direita->pai = noAVL;
    }
    direita->esquerda = noAVL;
    noAVL->pai = direita;
    direita->pai = pai;

    if (pai != NULL) {
        if (noAVL == pai->direita) {
            pai->direita = direita;
        } else {
            pai->esquerda = direita;
        }
    }
    return direita;
}

NoAVL* rsdAVL(NoAVL* noAVL) {
    NoAVL* pai = noAVL->pai;
    NoAVL* esquerda = noAVL->esquerda;

    noAVL->esquerda = esquerda->direita;
    if (noAVL->esquerda != NULL) {
        noAVL->esquerda->pai = noAVL;
    }
    esquerda->direita = noAVL;
    noAVL->pai = esquerda;
    esquerda->pai = pai;

    if (pai != NULL) {
        if (noAVL == pai->esquerda) {
            pai->esquerda = esquerda;
        } else {
            pai->direita = esquerda;
        }
    }
    return esquerda;
}

NoAVL* rdeAVL(NoAVL* noAVL) {
    noAVL->direita = rsdAVL(noAVL->direita);
    return rseAVL(noAVL);
}

NoAVL* rddAVL(NoAVL* noAVL) {
    noAVL->esquerda = rseAVL(noAVL->esquerda);
    return rsdAVL(noAVL);
}

void balancearAVL(ArvoreAVL *arv, NoAVL *noAVL) {
    while (noAVL != NULL) {
        noAVL->fb = fbAVL(noAVL);

        if (noAVL->fb > 1) {
            if (fbAVL(noAVL->esquerda) < 0) {
                noAVL = rddAVL(noAVL);
            } else {
                noAVL = rsdAVL(noAVL);
            }
        } else if (noAVL->fb < -1) {
            if (fbAVL(noAVL->direita) > 0) {
                noAVL = rdeAVL(noAVL);
            } else {
                noAVL = rseAVL(noAVL);
            }
        }

        if (noAVL->pai == NULL) {
            arv->root = noAVL;
        }
        noAVL = noAVL->pai;
    }
}

NoAVL* buscaNoAVL(NoAVL *noAVL, int valor) {
    if(noAVL == NULL) return NULL; // Adicionado verificação de NULL
    if(noAVL->valor == valor) return noAVL;
    if(noAVL->valor > valor){
        return buscaNoAVL(noAVL->esquerda, valor);
    }
    return buscaNoAVL(noAVL->direita, valor);
}

NoAVL* noTrocaAVL(NoAVL *raiz) {
    NoAVL* atual = raiz;
    while (atual != NULL && atual->esquerda != NULL) { // Adicionado verificação de NULL
        atual = atual->esquerda;
    }
    return atual;
}

NoAVL* adicionarAVL(ArvoreAVL *arv, int valor) {
    printf("\nadicionando %d", valor);
    NoAVL *noAVL = malloc(sizeof(NoAVL));
    noAVL->esquerda = NULL;
    noAVL->direita = NULL;
    noAVL->valor = valor;
    noAVL->fb = 0; // Futuramente 'noAVL->alturaAVL = 0'

    if(arv->root == NULL){
        arv->root = noAVL;
        noAVL->pai = NULL;
        return noAVL;
    }

    NoAVL *atual = arv->root;
    NoAVL *pai = NULL;
    while (atual != NULL) {
        pai = atual;
        if (valor < atual->valor) {
            atual = atual->esquerda;
        } else if (valor > atual->valor) {
            atual = atual->direita;
        } else {
            free(noAVL);
            return NULL; // Valor duplicado
        }
    }

    noAVL->pai = pai;
    if (valor < pai->valor) {
        pai->esquerda = noAVL;
    } else {
        pai->direita = noAVL;
    }

    balancearAVL(arv, noAVL); // Começa a balancearAVL do nó adicionado
    return noAVL;
}

void removerAVL(ArvoreAVL *arv, int valor) {
    printf("Removendo valor: %d \n", valor);
    NoAVL *noAVLRemover = buscaNoAVL(arv->root, valor);
    if (noAVLRemover == NULL) return;

    NoAVL *pai = noAVLRemover->pai;
    NoAVL *noAVLParaBalancear = NULL; // Nó que iniciará o balanceamento

    //Caso 1: Nó é folha
    if (noAVLRemover->esquerda == NULL && noAVLRemover->direita == NULL) {
        noAVLParaBalancear = pai; // Balanceia a partir do pai
        if (pai == NULL) {
            arv->root = NULL;
        } else if (pai->esquerda == noAVLRemover) {
            pai->esquerda = NULL;
        } else {
            pai->direita = NULL;
        }
        free(noAVLRemover);

    //Caso 3: Dois filhos
    } else if (noAVLRemover->esquerda != NULL && noAVLRemover->direita != NULL) {
        NoAVL *sucessor = noTrocaAVL(noAVLRemover->direita);
        noAVLRemover->valor = sucessor->valor; // Copia o valor

        // Agora, remove o *sucessor* (que é Caso 1 ou 2)
        NoAVL *paiDoSucessor = sucessor->pai;
        NoAVL *filhoDoSucessor = sucessor->direita;

        noAVLParaBalancear = paiDoSucessor; // Balanceia a partir do pai do sucessor

        if (paiDoSucessor == noAVLRemover) {
            paiDoSucessor->direita = filhoDoSucessor;
        } else {
            paiDoSucessor->esquerda = filhoDoSucessor;
        }

        if (filhoDoSucessor != NULL) {
            filhoDoSucessor->pai = paiDoSucessor;
        }
        free(sucessor);

    //Caso 2: Um filho
    } else {
        noAVLParaBalancear = pai; // Balanceia a partir do pai
        NoAVL *filho = (noAVLRemover->esquerda != NULL) ? noAVLRemover->esquerda : noAVLRemover->direita;
        if (pai == NULL) {
            arv->root = filho;
            filho->pai = NULL;
        } else if (pai->esquerda == noAVLRemover) {
            pai->esquerda = filho;
            filho->pai = pai;
        } else {
            pai->direita = filho;
            filho->pai = pai;
        }
        free(noAVLRemover);
    }

    balancearAVL(arv, noAVLParaBalancear);
}

// int main() {
//     ArvoreAVL* arv = criarAVL();
//
//     for(int i = 1; i < 10; i++){
//         adicionarAVL(arv, i);
//         percorrePrintAVL(arv->root);
//         printf("\n");
//     };
//     removerAVL(arv, 6);
//     percorrePrintAVL(arv->root);
//     removerAVL(arv, 3);
//     percorrePrintAVL(arv->root);
//     adicionarAVL(arv, 6);
//     percorrePrintAVL(arv->root);
//     return 0;
// }