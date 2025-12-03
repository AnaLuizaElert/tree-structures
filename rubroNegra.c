#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rubroNegra.h" // Certifique-se de incluir o header se necessário, ou mantenha as structs aqui se for um arquivo unico de compilacao

#define MAX_LARGURA_RB 256
#define MAX_ALTURA_RB 50

// As structs já devem estar no .h, mas se estiver compilando tudo junto ou precisar redefinir:
/*
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
*/

char _buffer[MAX_ALTURA_RB][MAX_LARGURA_RB];

int alturaNoRB(NoRB *noRB, NoRB *folha) {
    if (noRB == folha) {
        return 0;
    }
    int esq = alturaNoRB(noRB->esquerda, folha);
    int dir = alturaNoRB(noRB->direita, folha);
    return 1 + (esq > dir ? esq : dir);
}

void limparBufferRB(int altura_max) {
    for (int i = 0; i < altura_max; i++) {
        for (int j = 0; j < MAX_LARGURA_RB; j++) {
            _buffer[i][j] = ' ';
        }
        _buffer[i][MAX_LARGURA_RB - 1] = '\0';
    }
}

char obterCorCharRB(NoRB *noRB) {
    return (noRB->cor == VERMELHO) ? 'R' : 'P';
}

int desenhaNoRB(NoRB *noRB, ArvoreRB *arvoreRB, int linha, int coluna) {
    if (noRB == arvoreRB->folha) {
        return 3;
    }

    int largura_esq = desenhaNoRB(noRB->esquerda, arvoreRB, linha + 2, coluna);
    int largura_dir = desenhaNoRB(noRB->direita, arvoreRB, linha + 2, coluna + largura_esq - 1);

    int centro = coluna + largura_esq - 1;
    char str[15];
    snprintf(str, sizeof(str), "[%d:%c]", noRB->valor, obterCorCharRB(noRB));
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

void imprimirArvoreRB(ArvoreRB *arvoreRB) {
    if (arvoreRB->raiz == arvoreRB->folha) {
        printf("\nA árvore está vazia.\n");
        return;
    }
    int altura_arvoreRB = alturaNoRB(arvoreRB->raiz, arvoreRB->folha);
    int linhas_necessarias = altura_arvoreRB * 2 + 1;
    limparBufferRB(linhas_necessarias);
    desenhaNoRB(arvoreRB->raiz, arvoreRB, 0, 0);
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
    NoRB* direita = noRB->direita;
    noRB->direita = direita->esquerda;

    (*esforco)++;
    // CORREÇÃO: Usar arvoreRB->folha em vez de NULL
    if (direita->esquerda != arvoreRB->folha) {
        direita->esquerda->pai = noRB;
    }

    direita->pai = noRB->pai;

    (*esforco)++;
    // CORREÇÃO: Comparar pai com folha, não NULL (se pai da raiz for folha)
    if (noRB->pai == arvoreRB->folha) {
        arvoreRB->raiz = direita;
    } else if (noRB == noRB->pai->esquerda) {
        (*esforco)++;
        noRB->pai->esquerda = direita;
    } else {
        (*esforco)++;
        noRB->pai->direita = direita;
    }

    direita->esquerda = noRB;
    noRB->pai = direita;

    return direita;
}

// Rotação Simples Direita (CORRIGIDA)
NoRB* rsdRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    NoRB* esquerda = noRB->esquerda;
    noRB->esquerda = esquerda->direita;

    (*esforco)++;
    // CORREÇÃO: Usar arvoreRB->folha em vez de NULL
    if (esquerda->direita != arvoreRB->folha) {
        esquerda->direita->pai = noRB;
    }

    esquerda->pai = noRB->pai;

    (*esforco)++;
    if (noRB->pai == arvoreRB->folha) {
        arvoreRB->raiz = esquerda;
    } else if (noRB == noRB->pai->esquerda) {
        (*esforco)++;
        noRB->pai->esquerda = esquerda;
    } else {
        (*esforco)++;
        noRB->pai->direita = esquerda;
    }

    esquerda->direita = noRB;
    noRB->pai = esquerda;

    return esquerda;
}

NoRB* rdeRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    noRB->direita = rsdRB(noRB->direita, arvoreRB, esforco);
    return rseRB(noRB, arvoreRB, esforco);
}

NoRB* rddRB(NoRB* noRB, ArvoreRB *arvoreRB, long long *esforco) {
    noRB->esquerda = rseRB(noRB->esquerda, arvoreRB, esforco);
    return rsdRB(noRB, arvoreRB, esforco);
}

NoRB *buscarNoRB(ArvoreRB *arvoreRB, int valor) {
    NoRB *current = arvoreRB->raiz;
    while (current != arvoreRB->folha) {
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

void balancearRB(ArvoreRB *arvoreRB, NoRB* noRB, long long *esforco) {
    (*esforco)++;
    while (noRB->pai->cor == VERMELHO) {
        (*esforco)++;
        if (noRB->pai == arvoreRB->raiz) break;

        NoRB *grandpa = noRB->pai->pai;

        // Verifica se avô é válido antes de acessar (segurança extra)
        if (grandpa == arvoreRB->folha) break;

        (*esforco)++;
        if (noRB->pai == grandpa->esquerda) {
            NoRB* uncle = grandpa->direita;

            (*esforco)++;
            if (uncle->cor == VERMELHO) {
                uncle->cor = PRETO;
                noRB->pai->cor = PRETO;
                grandpa->cor = VERMELHO;
                noRB = grandpa;
            } else {
                (*esforco)++;
                if (noRB == noRB->pai->direita) {
                    noRB = noRB->pai;
                    rseRB(noRB, arvoreRB, esforco);
                }
                noRB->pai->cor = PRETO;
                grandpa->cor = VERMELHO;
                rsdRB(grandpa, arvoreRB, esforco);
            }
        } else {
            NoRB* uncle = grandpa->esquerda;

            (*esforco)++;
            if (uncle->cor == VERMELHO) {
                uncle->cor = PRETO;
                noRB->pai->cor = PRETO;
                grandpa->cor = VERMELHO;
                noRB = grandpa;
            } else {
                (*esforco)++;
                if (noRB == noRB->pai->esquerda) {
                    noRB = noRB->pai;
                    rsdRB(noRB, arvoreRB, esforco);
                }
                noRB->pai->cor = PRETO;
                grandpa->cor = VERMELHO;
                rseRB(grandpa, arvoreRB, esforco);
            }
        }
    }
    arvoreRB->raiz->cor = PRETO;
}

NoRB *procuraPaiRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    NoRB *current = arvoreRB->raiz;
    NoRB *pai = arvoreRB->folha;

    (*esforco)++;
    while (current != arvoreRB->folha) {
        pai = current;
        (*esforco)++;
        if (valor < current->valor) {
            current = current->esquerda;
        } else if (valor > current->valor) {
            (*esforco)++;
            current = current->direita;
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
        NoRB *noRB = criarNoRB(arvoreRB, arvoreRB->folha, valor);
        arvoreRB->raiz = noRB;
        noRB->cor = PRETO;
        return noRB;
    }

    NoRB *pai = procuraPaiRB(arvoreRB, valor, esforco);

    (*esforco)++;
    if (pai == NULL) return NULL; // Elemento já existe

    NoRB *noRB = criarNoRB(arvoreRB, pai, valor);

    (*esforco)++;
    if (valor < pai->valor) {
        pai->esquerda = noRB;
    } else {
        pai->direita = noRB;
    }

    balancearRB(arvoreRB, noRB, esforco);
    return noRB;
}

void trocaRB(NoRB *noRBToRemove, NoRB *noRBSubstitute, ArvoreRB *arvoreRB, long long *esforco) {
    (*esforco)++;
    if (noRBToRemove->pai == arvoreRB->folha) {
        arvoreRB->raiz = noRBSubstitute;
    } else if (noRBToRemove == noRBToRemove->pai->esquerda) {
        (*esforco)++;
        noRBToRemove->pai->esquerda = noRBSubstitute;
    } else {
        (*esforco)++;
        noRBToRemove->pai->direita = noRBSubstitute;
    }
    // CORREÇÃO CRÍTICA: Atualizar o pai do substituto (mesmo que seja folha)
    noRBSubstitute->pai = noRBToRemove->pai;
}

NoRB *minimoRB(ArvoreRB *arvoreRB, NoRB *noRB) {
    while (noRB->esquerda != arvoreRB->folha) {
        noRB = noRB->esquerda;
    }
    return noRB;
}

void removerCorrecaoRB(ArvoreRB *arvoreRB, NoRB *substitute, long long *esforco) {
    (*esforco)++;
    while (substitute != arvoreRB->raiz && substitute->cor == PRETO) {
        (*esforco)++;
        if (substitute == substitute->pai->esquerda) { // Correção de sintaxe para segurança
            NoRB *brother = substitute->pai->direita;
            (*esforco)++;
            if (brother->cor == VERMELHO) {
                brother->cor = PRETO;
                substitute->pai->cor = VERMELHO;
                rseRB(substitute->pai, arvoreRB, esforco);
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
                    rsdRB(brother, arvoreRB, esforco);
                    brother = substitute->pai->direita;
                }

                brother->cor = substitute->pai->cor;
                substitute->pai->cor = PRETO;
                brother->direita->cor = PRETO;
                rseRB(substitute->pai, arvoreRB, esforco);
                substitute = arvoreRB->raiz;
            }
        } else {
            NoRB *brother = substitute->pai->esquerda;

            (*esforco)++;
            if (brother->cor == VERMELHO) {
                brother->cor = PRETO;
                substitute->pai->cor = VERMELHO;
                rsdRB(substitute->pai, arvoreRB, esforco);
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
                    rseRB(brother, arvoreRB, esforco);
                    brother = substitute->pai->esquerda;
                }

                brother->cor = substitute->pai->cor;
                substitute->pai->cor = PRETO;
                brother->esquerda->cor = PRETO;
                rsdRB(substitute->pai, arvoreRB, esforco);
                substitute = arvoreRB->raiz;
            }
        }
        (*esforco)++;
    }
    substitute->cor = PRETO;
}

void removerRB(ArvoreRB *arvoreRB, int valor, long long *esforco) {
    NoRB *noRBToRemove = buscarNoRB(arvoreRB, valor);

    if (noRBToRemove == NULL) {
        // Se a geração de dados for única, isso não deve ocorrer
        // printf("\nErro: Nó com valor %d não encontrado para remoção.", valor);
        return;
    }

    NoRB *y = noRBToRemove;
    NoRB *x;
    CorRB y_original_color = y->cor;

    (*esforco)++;
    if (noRBToRemove->esquerda == arvoreRB->folha) {
        x = noRBToRemove->direita;
        trocaRB(noRBToRemove, noRBToRemove->direita, arvoreRB, esforco);
    } else if (noRBToRemove->direita == arvoreRB->folha) {
        (*esforco)++;
        x = noRBToRemove->esquerda;
        trocaRB(noRBToRemove, noRBToRemove->esquerda, arvoreRB, esforco);
    } else {
        (*esforco)++;
        y = minimoRB(arvoreRB, noRBToRemove->direita);
        y_original_color = y->cor;
        x = y->direita;

        (*esforco)++;
        if (y->pai == noRBToRemove) {
            x->pai = y; // CORREÇÃO: Garante que o sentinela saiba quem é o pai
        } else {
            trocaRB(y, y->direita, arvoreRB, esforco);
            y->direita = noRBToRemove->direita;
            y->direita->pai = y;
        }

        trocaRB(noRBToRemove, y, arvoreRB, esforco);
        y->esquerda = noRBToRemove->esquerda;
        y->esquerda->pai = y;
        y->cor = noRBToRemove->cor;
    }

    (*esforco)++;
    if (y_original_color == PRETO) {
        removerCorrecaoRB(arvoreRB, x, esforco);
    }

    free(noRBToRemove);
}