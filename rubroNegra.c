#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LARGURA_RB 256
#define MAX_ALTURA_RB 50

// As structs já devem estar no .h, mas se estiver compilando tudo junto ou precisar redefinir:
enum coresRB {VERMELHO, PRETO};
typedef enum coresRB CorRB;

typedef struct noRB {
    int valor;
    CorRB cor;
    struct noRB* pai;
    struct noRB* direita;
    struct noRB* esquerda;
} NoRB;

typedef struct arvoreRB {
    struct noRB* raiz;
    struct noRB* folha;
} ArvoreRB;


char _buffer[MAX_ALTURA_RB][MAX_LARGURA_RB];

int alturaNoRB(NoRB *noRB, NoRB *folha, long long *esforco) {
    (*esforco)++;
    if (noRB == folha) {
        return 0;
    }
    int esq = alturaNoRB(noRB->esquerda, folha, esforco);
    int dir = alturaNoRB(noRB->direita, folha, esforco);
    return 1 + (esq > dir ? esq : dir);
}

void limparBufferRB(int altura_max, long long *esforco) {

    for (int i = 0; i < altura_max; i++) {
        for (int j = 0; j < MAX_LARGURA_RB; j++) {
            _buffer[i][j] = ' ';
            (*esforco)++;
        }
        _buffer[i][MAX_LARGURA_RB - 1] = '\0';
        (*esforco)++;
    }
}

char obterCorCharRB(NoRB *noRB, long long *esforco) {
    (*esforco)++;
    return (noRB->cor == VERMELHO) ? 'R' : 'P';
}

int desenhaNoRB(NoRB *noRB, ArvoreRB *arvoreRB, int linha, int coluna, long long *esforco) {
    if (noRB == arvoreRB->folha) {
        return 3;
    }

    int largura_esq = desenhaNoRB(noRB->esquerda, arvoreRB, linha + 2, coluna, esforco);
    int largura_dir = desenhaNoRB(noRB->direita, arvoreRB, linha + 2, coluna + largura_esq - 1, esforco);

    int centro = coluna + largura_esq - 1;
    char str[15];
    snprintf(str, sizeof(str), "[%d:%c]", noRB->valor, obterCorCharRB(noRB, esforco));
    int len = strlen(str);

    int inicio_texto = centro - (len / 2);
    if (inicio_texto < 0) inicio_texto = 0;

    if (linha < MAX_ALTURA_RB && inicio_texto + len < MAX_LARGURA_RB) {
        strncpy(&_buffer[linha][inicio_texto], str, len);
    }

    if (noRB->esquerda != arvoreRB->folha) {
        int centro_esq_conexao = centro - 1;
        if (linha + 1 < MAX_ALTURA_RB && centro_esq_conexao >= 0) {
            _buffer[linha + 1][centro_esq_conexao] = '/';
        }
        for (int i = inicio_texto - 1; i > centro_esq_conexao; i--) {
             if (linha < MAX_ALTURA_RB && i >= 0) _buffer[linha][i] = '-';
        }
    }

    if (noRB->direita != arvoreRB->folha) {
        int centro_dir_conexao = centro + 1;
        if (linha + 1 < MAX_ALTURA_RB && centro_dir_conexao < MAX_LARGURA_RB) {
            _buffer[linha + 1][centro_dir_conexao] = '\\';
        }
        for (int i = inicio_texto + len; i < centro_dir_conexao; i++) {
             if (linha < MAX_ALTURA_RB && i < MAX_LARGURA_RB) _buffer[linha][i] = '-';
        }
    }

    return largura_esq + largura_dir - 1;
}

void imprimirArvoreRB(ArvoreRB *arvoreRB, long long *esforco) {
    if (arvoreRB->raiz == arvoreRB->folha) {
        printf("\nA árvore está vazia.\n");
        return;
    }
    int altura_arvoreRB = alturaNoRB(arvoreRB->raiz, arvoreRB->folha, esforco);
    int linhas_necessarias = altura_arvoreRB * 2 + 1;
    limparBufferRB(linhas_necessarias, esforco);
    desenhaNoRB(arvoreRB->raiz, arvoreRB, 0, 0, esforco);
    printf("\n--- Visualizacao ASCII Art (Compacta) ---\n");
    for (int i = 0; i < linhas_necessarias; i++) {
        int linha_vazia = 1;
        for (int j = 0; j < MAX_LARGURA_RB; j++) {
            if (_buffer[i][j] != ' ' && _buffer[i][j] != '\0') {
                linha_vazia = 0;
                break;
            }
        }
        if (!linha_vazia) {
            printf("%s\n", _buffer[i]);
        }
    }
    printf("-----------------------------------------\n");
}

/* --- FUNÇÕES LÓGICAS CORRIGIDAS --- */

ArvoreRB* criarArvoreRB() {
    ArvoreRB *arvoreRB = (ArvoreRB*) malloc(sizeof(ArvoreRB));
    NoRB *folha = (NoRB*) malloc(sizeof(NoRB));
    folha->cor = PRETO;
    folha->valor = 0;
    // O pai da folha é irrelevante na criação, mas nao deve ser NULL para evitar segfaults se acessado indevidamente
    folha->pai = NULL;
    folha->esquerda = folha; // Aponta para si mesmo
    folha->direita = folha;  // Aponta para si mesmo

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

// Rotação Simples Esquerda (CORRIGIDA)
NoRB* rseRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    NoRB* direita = noRB->direita; (*esforco)++;
    noRB->direita = direita->esquerda; (*esforco)++;

    (*esforco)++;
    if (direita->esquerda != arvoreRB->folha) {
        direita->esquerda->pai = noRB; (*esforco)++;
    }

    direita->pai = noRB->pai; (*esforco)++;

    (*esforco)++;
    if (noRB->pai == arvoreRB->folha) {
        arvoreRB->raiz = direita; (*esforco)++;
    } else if (noRB == noRB->pai->esquerda) {
        (*esforco)++;
        noRB->pai->esquerda = direita; (*esforco)++;
    } else {
        (*esforco)++;
        noRB->pai->direita = direita; (*esforco)++;
    }

    direita->esquerda = noRB; (*esforco)++;
    noRB->pai = direita; (*esforco)++;

    return direita;
}

// Rotação Simples Direita (CORRIGIDA)
NoRB* rsdRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    NoRB* esquerda = noRB->esquerda; (*esforco)++;
    noRB->esquerda = esquerda->direita; (*esforco)++;

    (*esforco)++;
    if (esquerda->direita != arvoreRB->folha) {
        esquerda->direita->pai = noRB; (*esforco)++;
    }

    esquerda->pai = noRB->pai; (*esforco)++;

    (*esforco)++;
    if (noRB->pai == arvoreRB->folha) {
        arvoreRB->raiz = esquerda; (*esforco)++;
    } else if (noRB == noRB->pai->esquerda) {
        (*esforco)++;
        noRB->pai->esquerda = esquerda; (*esforco)++;
    } else {
        (*esforco)++;
        noRB->pai->direita = esquerda; (*esforco)++;
    }

    esquerda->direita = noRB; (*esforco)++;
    noRB->pai = esquerda; (*esforco)++;

    return esquerda;
}

NoRB* rdeRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    (*esforco)++;
    noRB->direita = rsdRB(noRB->direita, arvoreRB, esforco);
    return rseRB(noRB, arvoreRB, esforco);
}

NoRB* rddRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    (*esforco)++;
    noRB->esquerda = rseRB(noRB->esquerda, arvoreRB, esforco);
    return rsdRB(noRB, arvoreRB, esforco);
}

NoRB *buscarNoRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    NoRB *current = arvoreRB->raiz; (*esforco)++;
    while (current != arvoreRB->folha) {
        (*esforco)++;
        if (valor < current->valor) {
            current = current->esquerda; (*esforco)++;
        } else if (valor > current->valor) {
            (*esforco)++;
            current = current->direita; (*esforco)++;
        } else {
            (*esforco)++;
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

        NoRB *grandpa = noRB->pai->pai; (*esforco)++;

        // Verifica se avô é válido antes de acessar (segurança extra)
        (*esforco)++;
        if (grandpa == arvoreRB->folha) break;

        (*esforco)++;
        if (noRB->pai == grandpa->esquerda) {
            NoRB* uncle = grandpa->direita; (*esforco)++;

            (*esforco)++;
            if (uncle->cor == VERMELHO) {
                uncle->cor = PRETO; (*esforco)++;
                noRB->pai->cor = PRETO; (*esforco)++;
                grandpa->cor = VERMELHO; (*esforco)++;
                noRB = grandpa; (*esforco)++;
            } else {
                (*esforco)++;
                if (noRB == noRB->pai->direita) {
                    noRB = noRB->pai; (*esforco)++;
                    rseRB(noRB, arvoreRB, esforco);
                }
                noRB->pai->cor = PRETO; (*esforco)++;
                grandpa->cor = VERMELHO; (*esforco)++;
                rsdRB(grandpa, arvoreRB, esforco);
            }
        } else {
            NoRB* uncle = grandpa->esquerda; (*esforco)++;

            (*esforco)++;
            if (uncle->cor == VERMELHO) {
                uncle->cor = PRETO; (*esforco)++;
                noRB->pai->cor = PRETO; (*esforco)++;
                grandpa->cor = VERMELHO; (*esforco)++;
                noRB = grandpa; (*esforco)++;
            } else {
                (*esforco)++;
                if (noRB == noRB->pai->esquerda) {
                    noRB = noRB->pai; (*esforco)++;
                    rsdRB(noRB, arvoreRB, esforco);
                }
                noRB->pai->cor = PRETO; (*esforco)++;
                grandpa->cor = VERMELHO; (*esforco)++;
                rseRB(grandpa, arvoreRB, esforco);
            }
        }
        (*esforco)++;
    }
    arvoreRB->raiz->cor = PRETO; (*esforco)++;
}

NoRB *procuraPaiRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    NoRB *current = arvoreRB->raiz; (*esforco)++;
    NoRB *pai = arvoreRB->folha; (*esforco)++;

    while (current != arvoreRB->folha) {
        pai = current; (*esforco)++;
        (*esforco)++;
        if (valor < current->valor) {
            current = current->esquerda; (*esforco)++;
        } else if (valor > current->valor) {
            (*esforco)++;
            current = current->direita; (*esforco)++;
        } else {
            (*esforco)++;
            return NULL; // Valor já existe
        }
        (*esforco)++;
    }
    return pai;
}

NoRB* adicionarRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    (*esforco)++;
    if(arvoreRB->raiz == arvoreRB->folha){
        NoRB *noRB = criarNoRB(arvoreRB, arvoreRB->folha, valor); (*esforco)++;
        arvoreRB->raiz = noRB; (*esforco)++;
        noRB->cor = PRETO; (*esforco)++;
        return noRB;
    }

    NoRB *pai = procuraPaiRB(arvoreRB, valor, esforco); (*esforco)++;

    (*esforco)++;
    if (pai == NULL) return NULL; // Elemento já existe

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

void trocaRB(NoRB *noRBToRemove, NoRB *noRBSubstitute, ArvoreRB *arvoreRB, long long *esforco) {
    (*esforco)++;
    if (noRBToRemove->pai == arvoreRB->folha) {
        arvoreRB->raiz = noRBSubstitute; (*esforco)++;
    } else if (noRBToRemove == noRBToRemove->pai->esquerda) {
        (*esforco)++;
        noRBToRemove->pai->esquerda = noRBSubstitute; (*esforco)++;
    } else {
        (*esforco)++;
        noRBToRemove->pai->direita = noRBSubstitute; (*esforco)++;
    }
    noRBSubstitute->pai = noRBToRemove->pai; (*esforco)++;
}

NoRB *minimoRB(ArvoreRB *arvoreRB, NoRB *noRB, long long *esforco) {
    while (noRB->esquerda != arvoreRB->folha) {
        noRB = noRB->esquerda; (*esforco)++;
        (*esforco)++;
    }
    return noRB;
}

void removerCorrecaoRB(ArvoreRB *arvoreRB, NoRB *substitute, long long *esforco) {
    (*esforco)++;
    while (substitute != arvoreRB->raiz && substitute->cor == PRETO) {
        (*esforco)++;
        if (substitute == substitute->pai->esquerda) { // Correção de sintaxe para segurança
            NoRB *brother = substitute->pai->direita; (*esforco)++;
            (*esforco)++;
            if (brother->cor == VERMELHO) {
                brother->cor = PRETO; (*esforco)++;
                substitute->pai->cor = VERMELHO; (*esforco)++;
                rseRB(substitute->pai, arvoreRB, esforco);
                brother = substitute->pai->direita; (*esforco)++;
            }

            (*esforco)++;
            if (brother->esquerda->cor == PRETO && brother->direita->cor == PRETO) {
                brother->cor = VERMELHO; (*esforco)++;
                substitute = substitute->pai; (*esforco)++;
            } else {
                (*esforco)++;
                if (brother->direita->cor == PRETO) {
                    brother->esquerda->cor = PRETO; (*esforco)++;
                    brother->cor = VERMELHO; (*esforco)++;
                    rsdRB(brother, arvoreRB, esforco);
                    brother = substitute->pai->direita; (*esforco)++;
                }

                brother->cor = substitute->pai->cor; (*esforco)++;
                substitute->pai->cor = PRETO; (*esforco)++;
                brother->direita->cor = PRETO; (*esforco)++;
                rseRB(substitute->pai, arvoreRB, esforco);
                substitute = arvoreRB->raiz; (*esforco)++;
            }
        } else {
            NoRB *brother = substitute->pai->esquerda; (*esforco)++;

            (*esforco)++;
            if (brother->cor == VERMELHO) {
                brother->cor = PRETO; (*esforco)++;
                substitute->pai->cor = VERMELHO; (*esforco)++;
                rsdRB(substitute->pai, arvoreRB, esforco); (*esforco)++;
                brother = substitute->pai->esquerda; (*esforco)++;
            }

            (*esforco)++;
            if (brother->direita->cor == PRETO && brother->esquerda->cor == PRETO) {
                brother->cor = VERMELHO; (*esforco)++;
                substitute = substitute->pai; (*esforco)++;
            } else {
                (*esforco)++;
                if (brother->esquerda->cor == PRETO) {
                    brother->direita->cor = PRETO; (*esforco)++;
                    brother->cor = VERMELHO; (*esforco)++;
                    rseRB(brother, arvoreRB, esforco);
                    brother = substitute->pai->esquerda; (*esforco)++;
                }

                brother->cor = substitute->pai->cor; (*esforco)++;
                substitute->pai->cor = PRETO; (*esforco)++;
                brother->esquerda->cor = PRETO; (*esforco)++;
                rsdRB(substitute->pai, arvoreRB, esforco);
                substitute = arvoreRB->raiz; (*esforco)++;
            }
        }
        (*esforco)++;
    }
    substitute->cor = PRETO; (*esforco)++;
}

void removerRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    NoRB *noRBToRemove = buscarNoRB(arvoreRB, valor, esforco); (*esforco)++;

    (*esforco)++;
    if (noRBToRemove == NULL) {
        // Se a geração de dados for única, isso não deve ocorrer
        printf("\nErro: No com valor %d não encontrado para remocao.", valor);
        return;
    }

    NoRB *y = noRBToRemove; (*esforco)++;
    NoRB *x; (*esforco)++;
    CorRB y_original_color = y->cor;

    (*esforco)++;
    if (noRBToRemove->esquerda == arvoreRB->folha) {
        x = noRBToRemove->direita; (*esforco)++;
        trocaRB(noRBToRemove, noRBToRemove->direita, arvoreRB, esforco);
    } else if (noRBToRemove->direita == arvoreRB->folha) {
        (*esforco)++;
        x = noRBToRemove->esquerda; (*esforco)++;
        trocaRB(noRBToRemove, noRBToRemove->esquerda, arvoreRB, esforco);
    } else {
        (*esforco)++;
        y = minimoRB(arvoreRB, noRBToRemove->direita, esforco); (*esforco)++;
        y_original_color = y->cor; (*esforco)++;
        x = y->direita; (*esforco)++;

        (*esforco)++;
        if (y->pai == noRBToRemove) {
            x->pai = y; (*esforco)++;
        } else {
            trocaRB(y, y->direita, arvoreRB, esforco);
            y->direita = noRBToRemove->direita; (*esforco)++;
            y->direita->pai = y; (*esforco)++;
        }

        trocaRB(noRBToRemove, y, arvoreRB, esforco);
        y->esquerda = noRBToRemove->esquerda; (*esforco)++;
        y->esquerda->pai = y; (*esforco)++;
        y->cor = noRBToRemove->cor; (*esforco)++;
    }

    (*esforco)++;
    if (y_original_color == PRETO) {
        removerCorrecaoRB(arvoreRB, x, esforco);
    }

    free(noRBToRemove);
}