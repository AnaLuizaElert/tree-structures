#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    int valor;
    struct no* pai;
    struct no* direita;
    struct no* esquerda;
    int fb;
} No;

typedef struct arvore {
    struct no* root;
} Arvore;

/*
Parte estética
*/

#define MAX_NIVEIS 10
#define MAX_LARGURA 80

char buffer[MAX_NIVEIS * 2][MAX_LARGURA];

void limpar_buffer() {
    for (int i = 0; i < MAX_NIVEIS * 2; i++) {
        for (int j = 0; j < MAX_LARGURA; j++) {
            buffer[i][j] = ' ';
        }
        buffer[i][MAX_LARGURA - 1] = '\0'; // Garantir fim de string
    }
}

int desenhaNo(No *no, int linha, int coluna, int largura_total) {
    if (no == NULL) {
        return 3; // Largura mínima para um nó 'vazio'
    }

    // 1. Chamadas Recursivas
    int largura_esq = desenhaNo(no->esquerda, linha + 2, coluna, largura_total);
    int largura_dir = desenhaNo(no->direita, linha + 2, coluna + largura_esq - 1, largura_total);

    // 2. Posição do Nó
    int centro = coluna + largura_esq - 1;
    char str[10];
    sprintf(str, "[%d]", no->valor);
    int len = strlen(str);
    int inicio_texto = centro - (len / 2);
    if (inicio_texto < 0) inicio_texto = 0;
    strncpy(&buffer[linha][inicio_texto], str, len);

    // 3. Desenhar Linhas de Conexão
    if (no->esquerda != NULL) {
        int centro_esq = coluna + largura_esq/2;
        buffer[linha + 1][centro_esq] = '/';
        for (int i = centro_esq + 1; i < centro; i++) {
            buffer[linha][i] = '-';
        }
    }
    if (no->direita != NULL) {
        int centro_dir = coluna + largura_esq + largura_dir/2;
        buffer[linha + 1][centro_dir] = '\\';
        for (int i = centro + 1; i < centro_dir; i++) {
            buffer[linha][i] = '-';
        }
    }

    return largura_esq + largura_dir - 1;
}

int altura(No *no);

void percorrePrint(No *no) {
    if (no == NULL) {
        printf("Arv. Vazia.\n");
        return;
    }

    int altura_arvore = altura(no); // Usa a função 'altura' da parte lógica
    limpar_buffer();
    int largura_total = 1 << (altura_arvore + 1);
    desenhaNo(no, 0, 0, largura_total);

    printf("\n--- Visualizacao ASCII Art ---\n");
    for (int i = 0; i < altura_arvore * 2 + 1; i++) {
        if (i < MAX_NIVEIS * 2) {
            printf("%s\n", buffer[i]);
        }
    }
    printf("------------------------------\n");
}

/*
Parte Lógica e main
*/

Arvore* criar() {
    Arvore *arv = NULL;
    arv = (Arvore*) malloc(sizeof(Arvore));
    arv->root = NULL;
    printf("\ncriar");
    return arv;
}

int vazia(Arvore *arv) {
    return (arv->root == NULL);
}

int altura(No *no){
    if (no == NULL) {
        return -1;
    }
    int altura_esquerda = altura(no->esquerda);
    int altura_direita = altura(no->direita);
    return 1 + (altura_esquerda > altura_direita ? altura_esquerda : altura_direita);
}

int fb(No *no){
    if (no == NULL) {
        return 0;
    }
    return altura(no->esquerda) - altura(no->direita);
}

No* rse(No* no) {
    No* pai = no->pai;
    No* direita = no->direita;

    no->direita = direita->esquerda;
    if (no->direita != NULL) {
        no->direita->pai = no;
    }
    direita->esquerda = no;
    no->pai = direita;
    direita->pai = pai;

    if (pai != NULL) {
        if (no == pai->direita) {
            pai->direita = direita;
        } else {
            pai->esquerda = direita;
        }
    }
    return direita;
}

No* rsd(No* no) {
    No* pai = no->pai;
    No* esquerda = no->esquerda;

    no->esquerda = esquerda->direita;
    if (no->esquerda != NULL) {
        no->esquerda->pai = no;
    }
    esquerda->direita = no;
    no->pai = esquerda;
    esquerda->pai = pai;

    if (pai != NULL) {
        if (no == pai->esquerda) {
            pai->esquerda = esquerda;
        } else {
            pai->direita = esquerda;
        }
    }
    return esquerda;
}

No* rde(No* no) {
    no->direita = rsd(no->direita);
    return rse(no);
}

No* rdd(No* no) {
    no->esquerda = rse(no->esquerda);
    return rsd(no);
}

void balancear(Arvore *arv, No *no) {
    while (no != NULL) {
        no->fb = fb(no);

        if (no->fb > 1) {
            if (fb(no->esquerda) < 0) {
                no = rdd(no);
            } else {
                no = rsd(no);
            }
        } else if (no->fb < -1) {
            if (fb(no->direita) > 0) {
                no = rde(no);
            } else {
                no = rse(no);
            }
        }

        if (no->pai == NULL) {
            arv->root = no;
        }
        no = no->pai;
    }
}

No* buscaNo(No *no, int valor) {
    if(no == NULL) return NULL; // Adicionado verificação de NULL
    if(no->valor == valor) return no;
    if(no->valor > valor){
        return buscaNo(no->esquerda, valor);
    }
    return buscaNo(no->direita, valor);
}

No* noTroca(No *raiz) {
    No* atual = raiz;
    while (atual != NULL && atual->esquerda != NULL) { // Adicionado verificação de NULL
        atual = atual->esquerda;
    }
    return atual;
}

No* adicionar(Arvore *arv, int valor) {
    printf("\nadicionando %d", valor);
    No *no = malloc(sizeof(No));
    no->esquerda = NULL;
    no->direita = NULL;
    no->valor = valor;
    no->fb = 0; // Futuramente 'no->altura = 0'

    if(arv->root == NULL){
        arv->root = no;
        no->pai = NULL;
        return no;
    }

    No *atual = arv->root;
    No *pai = NULL;
    while (atual != NULL) {
        pai = atual;
        if (valor < atual->valor) {
            atual = atual->esquerda;
        } else if (valor > atual->valor) {
            atual = atual->direita;
        } else {
            free(no);
            return NULL; // Valor duplicado
        }
    }

    no->pai = pai;
    if (valor < pai->valor) {
        pai->esquerda = no;
    } else {
        pai->direita = no;
    }

    balancear(arv, no); // Começa a balancear do nó adicionado
    return no;
}

void remover(Arvore *arv, int valor) {
    printf("Removendo valor: %d \n", valor);
    No *noRemover = buscaNo(arv->root, valor);
    if (noRemover == NULL) return;

    No *pai = noRemover->pai;
    No *noParaBalancear = NULL; // Nó que iniciará o balanceamento

    //Caso 1: Nó é folha
    if (noRemover->esquerda == NULL && noRemover->direita == NULL) {
        noParaBalancear = pai; // Balanceia a partir do pai
        if (pai == NULL) {
            arv->root = NULL;
        } else if (pai->esquerda == noRemover) {
            pai->esquerda = NULL;
        } else {
            pai->direita = NULL;
        }
        free(noRemover);

    //Caso 3: Dois filhos
    } else if (noRemover->esquerda != NULL && noRemover->direita != NULL) {
        No *sucessor = noTroca(noRemover->direita);
        noRemover->valor = sucessor->valor; // Copia o valor

        // Agora, remove o *sucessor* (que é Caso 1 ou 2)
        No *paiDoSucessor = sucessor->pai;
        No *filhoDoSucessor = sucessor->direita;

        noParaBalancear = paiDoSucessor; // Balanceia a partir do pai do sucessor

        if (paiDoSucessor == noRemover) {
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
        noParaBalancear = pai; // Balanceia a partir do pai
        No *filho = (noRemover->esquerda != NULL) ? noRemover->esquerda : noRemover->direita;
        if (pai == NULL) {
            arv->root = filho;
            filho->pai = NULL;
        } else if (pai->esquerda == noRemover) {
            pai->esquerda = filho;
            filho->pai = pai;
        } else {
            pai->direita = filho;
            filho->pai = pai;
        }
        free(noRemover);
    }

    balancear(arv, noParaBalancear);
}

int main() {
    Arvore* arv = criar();

    for(int i = 1; i < 10; i++){
        adicionar(arv, i);
        percorrePrint(arv->root);
        printf("\n");
    };
    remover(arv, 6);
    percorrePrint(arv->root);
    remover(arv, 3);
    percorrePrint(arv->root);
    adicionar(arv, 6);
    percorrePrint(arv->root);
    return 0;
}