#include <stdio.h>
#include <stdlib.h>

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

No* rse(No* no) {
    No* pai = no->pai;
    No* direita = no->direita;

    no->direita = direita->esquerda;
    if (no->direita != NULL) {
        no->direita->pai = no->direita;
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
        no->esquerda->pai = no->esquerda;
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

int altura(No *no){
    int altura_esquerda = 0, altura_direita = 0;

    if(no->esquerda != NULL) {
        altura_esquerda = altura(no->esquerda) + 1;
    }

    if(no->direita != NULL) {
        altura_direita = altura(no->direita) + 1;
    }

    //ele considera o lado que tem mais nós para retornar a altura (altura máxima)
    return altura_esquerda > altura_direita? altura_esquerda : altura_direita;
}

int fb(No *no){
    int altura_esquerda = 0, altura_direita = 0;

    if(no->esquerda != NULL) {
        altura_esquerda = altura(no->esquerda) + 1;
    }

    if(no->direita != NULL) {
        altura_direita = altura(no->direita) + 1;
    }

    //se direita > esquerda = negativo (tem mais nós na direita)
    //se esquerda > direita = positivo (tem mais nós na esquerda)
    return altura_esquerda - altura_direita;
}

//retornar nó pai
No* percorre(No *no, int valor) {

    // if(no->valor = valor) return no;

    if(no->valor > valor){
        if(no->esquerda == NULL) return no;
        return percorre(no->esquerda, valor);
    }

    if(no->valor < valor){
        if(no->direita == NULL) return no;
        return percorre(no->direita, valor);
    }

    return NULL;
}

void percorrePrint(No *no) {
    printf("\nFilho = %d; ", no->valor);

    if (no->pai != NULL) {
        printf("Pai = %d", no->pai->valor);
    }

    if(no->esquerda != NULL){
        percorrePrint(no->esquerda);
    }
    if(no->direita != NULL){
        percorrePrint(no->direita);
    }
}

No* adicionar(Arvore *arv, int valor) {
    printf("\nadicionando %d", valor);
    No *no = malloc(sizeof(No));
    no->esquerda = NULL;
    no->direita = NULL;
    no->valor = valor;
    no->fb = 0;

   //calcular quem será o pai desse filho... ratinho? exame de dna
   //percorrer e achar o local dele
   //terá que ver o equilibrio e se necessário terá que fazer rotação
    if(arv->root == NULL){
        arv->root = no;
        no->pai = NULL;
        return no;
    }

    No *atual = arv->root;
    No *pai = NULL;

    //decidindo o pai... teste de DNA
    while (atual != NULL) {
        pai = atual;
        if (valor < atual->valor) {
            atual = atual->esquerda;
        } else if (valor > atual->valor) {
            atual = atual->direita;
        } else {
            free(no);
            return NULL;
        }
    }

    //Atribui o filho ao pai... recebe pensão
    no->pai = pai;
    if (valor < pai->valor) {
        pai->esquerda = no;
    } else {
        pai->direita = no;
    }

    atual = no;
    while (atual != NULL) {
        atual->fb = fb(atual); // Recalcula o fator de balanceamento

        if (atual->fb > 1) { // Desequilíbrio à esquerda
            if (atual->esquerda->fb < 0) {
                atual = rdd(atual);
            } else {
                atual = rsd(atual);
            }
        } else if (atual->fb < -1) { // Desequilíbrio à direita
            if (atual->direita->fb > 0) {
                atual = rde(atual);
            } else {
                atual = rse(atual);
            }
        }

        // Se o nó atual se tornou a nova raiz, atualize-a
        if (atual->pai == NULL) {
            arv->root = atual;
        }

        atual = atual->pai;
    }

    return no;
}

void remover(Arvore *arv,No* no) {
    if (no->esquerda != NULL)
        remover(arv, no->esquerda);

    if (no->direita != NULL)
        remover(arv, no->direita);

    if (no->pai == NULL) {
        arv->root = NULL;
    } else {
        if (no->pai->esquerda == no)
            no->pai->esquerda = NULL;
        else
        no->pai->direita = NULL;
    }
    free(no);
}

int main() {
    Arvore* arv = criar();

    for(int i = 1; i < 10; i++){
        adicionar(arv, i);
        percorrePrint(arv->root);
        printf("\n");
    };

    return 0;
}
