#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "avl.h"
#include "rubroNegra.h"
#include "b.h"

void gerarDadosAleatorios(int *dados, int tamanho) {
  int max_val = 100000;
  int *pool = malloc(max_val * sizeof(int));
  for(int i = 0; i < max_val; i++) pool[i] = i;
  for (int i = 0; i < tamanho; i++) {
    int j = i + rand() % (max_val - i);
    int temp = pool[j];
    pool[j] = pool[i];
    pool[i] = temp;
    dados[i] = pool[i];
  }
  free(pool);
}

void teste(FILE *arquivoAdicao, FILE *arquivoRemocao) {
  fprintf(arquivoAdicao,"Tamanho,AVL_Media,RB_Media,B1_Media,B5_Media,B10_Media\n");
  fprintf(arquivoRemocao,"Tamanho,AVL_Media,RB_Media,B1_Media,B5_Media,B10_Media\n");

  for (int tamanho = 1; tamanho <= 10000; tamanho += 1) {
    long long somaAddAVL = 0, somaAddRB = 0, somaAddB1 = 0, somaAddB5 = 0, somaAddB10 = 0;
    long long somaRemAVL = 0, somaRemRB = 0, somaRemB1 = 0, somaRemB5 = 0, somaRemB10 = 0;

    for (int teste = 0; teste < 10; teste++) {
      int *dados = malloc(tamanho * sizeof(int));
      int *dadosRemover = malloc((tamanho / 2) * sizeof(int));
      gerarDadosAleatorios(dados, tamanho);
      for (int i = 0; i < tamanho / 2; i++) dadosRemover[i] = dados[i];

      // AVL
      long long esforco = 0;
      ArvoreAVL* avl = criarAVL();
      for (int i = 0; i < tamanho; i++) adicionarAVL(avl, dados[i], &esforco);
      somaAddAVL += esforco;
      esforco = 0;
      for (int i = 0; i < tamanho / 2; i++) removerAVL(avl, dadosRemover[i], &esforco);
      somaRemAVL += esforco;
      free(avl);

      // Rubro Negra
      esforco = 0;
      ArvoreRB* rb = criarArvoreRB();
      for (int i = 0; i < tamanho; i++) adicionarRB(rb, dados[i], &esforco);
      somaAddRB += esforco;
      esforco = 0;
      for (int i = 0; i < tamanho / 2; i++) removerRB(rb, dadosRemover[i], &esforco);
      somaRemRB += esforco;
      free(rb);

      // Árvore B ordem 1
      esforco = 0;
      ArvoreB *b1 = criaArvoreB(1);
      for (int i = 0; i < tamanho; i++) adicionaChaveB(b1, dados[i], &esforco);
      somaAddB1 += esforco;
      esforco = 0;
      for (int i = 0; i < tamanho / 2; i++) removerB(b1, dadosRemover[i], &esforco);
      somaRemB1 += esforco;
      free(b1);

      // Árvore B ordem 5
      esforco = 0;
      ArvoreB *b5 = criaArvoreB(5);
      for (int i = 0; i < tamanho; i++) adicionaChaveB(b5, dados[i], &esforco);
      somaAddB5 += esforco;
      esforco = 0;
      for (int i = 0; i < tamanho / 2; i++) removerB(b5, dadosRemover[i], &esforco);
      somaRemB5 += esforco;
      free(b5);

      // Árvore B ordem 10
      esforco = 0;
      ArvoreB *b10 = criaArvoreB(10);
      for (int i = 0; i < tamanho; i++) adicionaChaveB(b10, dados[i], &esforco);
      somaAddB10 += esforco;
      esforco = 0;
      for (int i = 0; i < tamanho / 2; i++) removerB(b10, dadosRemover[i], &esforco);
      somaRemB10 += esforco;
      free(b10);

      free(dados);
      free(dadosRemover);
    }

    fprintf(
      arquivoAdicao,
      "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
      tamanho,
      (double)somaAddAVL / 10.0,
      (double)somaAddRB / 10.0,
      (double)somaAddB1 / 10.0,
      (double)somaAddB5 / 10.0,
      (double)somaAddB10 / 10.0
    );

    fprintf(
      arquivoRemocao,
      "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
      tamanho,
      (double)somaRemAVL / 10.0,
      (double)somaRemRB / 10.0,
      (double)somaRemB1 / 10.0,
      (double)somaRemB5 / 10.0,
      (double)somaRemB10 / 10.0
    );

  }
}

int main() {
    srand(time(NULL));
    FILE *arquivoAdicao = fopen("resultadosAdicao.csv", "w");
    FILE *arquivoRemocao = fopen("resultadosRemocao.csv", "w");
    if (!arquivoAdicao || !arquivoRemocao) {
        printf("Erro ao criar arquivos!\n");
        return 1;
    }
    teste(arquivoAdicao, arquivoRemocao);
    fclose(arquivoRemocao);
    fclose(arquivoAdicao);
    return 0;
}