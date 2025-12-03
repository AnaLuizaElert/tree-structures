#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "avl.h"
#include "rubroNegra.h"
#include "b.h"

void gerarDadosAleatorios(int *dados, int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    int novo;
    int repetido;
    do {
      novo = rand() % 100000;
      repetido = 0;
      for (int j = 0; j < i; j++) {
        if (dados[j] == novo) {
          repetido = 1;
          break;
        }
      }
    } while (repetido);

    dados[i] = novo;
  }
}

void teste(FILE *arquivoAdicao, FILE *arquivoRemocao) {
  fprintf(
    arquivoAdicao,"Tamanho,AVL_Media,RB_Media,B1_Media,B5_Media,B10_Media\n"
  );
  fprintf(
    arquivoRemocao,"Tamanho,AVL_Media,RB_Media,B1_Media,B5_Media,B10_Media\n"
  );
  fflush(arquivoAdicao);

  for (int tamanho = 1; tamanho <= 10000; tamanho++) {
    int somaAddAVL = 0, somaAddRB = 0, somaAddB1 = 0, somaAddB5 = 0, somaAddB10 = 0;
    int somaRemAVL = 0, somaRemRB = 0, somaRemB1 = 0, somaRemB5 = 0, somaRemB10 = 0;

    for (int teste = 0; teste < 10; teste++) {
      int *dados = malloc(tamanho * sizeof(int));
      int *dadosRemover = malloc((tamanho / 2) * sizeof(int));
      gerarDadosAleatorios(dados, tamanho);

      for (int i = 0; i < tamanho / 2; i++) {
        dadosRemover[i] = dados[i];
      }

      // //AVL
      ArvoreAVL* avl = criarAVL();
      for (int i = 0; i < tamanho; i++) {
        adicionarAVL(avl, dados[i], &somaAddAVL);
      }
      for (int i = 0; i < tamanho / 2; i++) {
        removerAVL(avl, dadosRemover[i], &somaRemAVL);
      }
      free(avl);

      // Rubro Negra
      ArvoreRB* rb = criarArvoreRB();
      for (int i = 0; i < tamanho; i++) {
        adicionarRB(rb, dados[i], &somaAddRB);
      }
      for (int i = 0; i < tamanho / 2; i++) {
        removerRB(rb, dadosRemover[i], &somaRemRB);
      }
      free(rb);

      // Árvore B ordem 1
      ArvoreB *b1 = criaArvoreB(1);
      for (int i = 0; i < tamanho; i++) {
        adicionaChaveB(b1, dados[i], &somaAddB1);
      }
      for (int i = 0; i < tamanho / 2; i++) {
        removerB(b1, dadosRemover[i], &somaRemB1);
      }
      free(b1);

      // Árvore B ordem 5
      ArvoreB *b5 = criaArvoreB(5);
      for (int i = 0; i < tamanho; i++) {
        adicionaChaveB(b5, dados[i], &somaAddB5);
      }
      for (int i = 0; i < tamanho / 2; i++) {
        removerB(b5, dadosRemover[i], &somaRemB5);
      }
      free(b5);

      // Árvore B ordem 10
      ArvoreB *b10 = criaArvoreB(10);
      for (int i = 0; i < tamanho; i++) {
        adicionaChaveB(b10, dados[i], &somaAddB10);
      }
      for (int i = 0; i < tamanho / 2; i++) {
        removerB(b10, dadosRemover[i], &somaRemB10);
      }
      free(b10);

      free(dados);
      free(dadosRemover);
    }

    double mediaAddAVL = (double)somaAddAVL / 10.0;
    double mediaAddRB = (double)somaAddRB / 10.0;
    double mediaAddB1 = (double)somaAddB1 / 10.0;
    double mediaAddB5 = (double)somaAddB5 / 10.0;
    double mediaAddB10 = (double)somaAddB10 / 10.0;

    fprintf(
      arquivoAdicao,
      "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
      tamanho, mediaAddAVL, mediaAddRB, mediaAddB1, mediaAddB5, mediaAddB10
    );

    double mediaRemAVL = (double)somaRemAVL / 10.0;
    double mediaRemRB = (double)somaRemRB / 10.0;
    double mediaRemB1 = (double)somaRemB1 / 10.0;
    double mediaRemB5 = (double)somaRemB5 / 10.0;
    double mediaRemB10 = (double)somaRemB10 / 10.0;

    fprintf(
      arquivoRemocao,
      "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
      tamanho, mediaRemAVL, mediaRemRB, mediaRemB1, mediaRemB5, mediaRemB10
    );
  }
}

int main() {
    srand(time(NULL));

    FILE *arquivoAdicao = fopen("resultadosAdicao.csv", "w");
    if (arquivoAdicao == NULL) {
        printf("Erro ao criar arquivo de resultados para adição!\n");
        return 1;
    }

    FILE *arquivoRemocao = fopen("resultadosRemocao.csv", "w");
    if (arquivoRemocao == NULL) {
      printf("Erro ao criar arquivo de resultados para remoção!\n");
      fclose(arquivoAdicao);
      return 1;
    }

    teste(arquivoAdicao, arquivoRemocao);

    fflush(arquivoAdicao);
    fflush(arquivoRemocao);
    fclose(arquivoRemocao);
    fclose(arquivoAdicao);

    return 0;
}