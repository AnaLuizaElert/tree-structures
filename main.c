#include <stdio.h>
#include "avl.h"
#include "rubronegra.h"
#include "b.h"

void gerar_dados_aleatorios(int *dados, int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    dados[i] = rand() % 100000;
  }
}

void teste(FILE *arquivo_adicao, FILE *arquivo_remocao) {
  fprintf(
    arquivo_adicao,"Tamanho,AVL_Media,RB_Media,B1_Media,B5_Media,B10_Media\n"
  );
  fprintf(
    arquivo_remocao,"Tamanho,AVL_Media,RB_Media,B1_Media,B5_Media,B10_Media\n"
  );

  for (int tamanho = 100; tamanho <= 10000; tamanho += 100) {
    long soma_add_avl = 0, soma_add_rb = 0, soma_add_b1 = 0, soma_add_b5 = 0, soma_add_b10 = 0;
    long soma_rem_avl = 0, soma_rem_rb = 0, soma_rem_b1 = 0, soma_rem_b5 = 0, soma_rem_b10 = 0;

    for (int teste = 0; teste < 10; teste++) {
      int *dados = malloc(tamanho * sizeof(int));
      int *dados_remover = malloc((tamanho / 2) * sizeof(int));
      gerar_dados_aleatorios(dados, tamanho);

      for (int i = 0; i < tamanho / 2; i++) {
        dados_remover[i] = dados[i];
      }

      //AVL
      for (int i = 0; i < tamanho; i++) {
        adicionar_avl(avl, dados[i]);
      }
      contador_avl = 0;
      for (int i = 0; i < tamanho / 2; i++) {
        remover_avl(avl, dados_remover[i]);
      }
      soma_avl += contador_avl;

      // Rubro Negra
      RB_Tree *minha_rb = createTree_rb();
      for (int i = 0; i < tamanho; i++) {
        add_rb(minha_rb, 20);
      }
      contador_rb = 0; 
      for (int i = 0; i < tamanho / 2; i++) {
        remover_rb(rb, dados_remover[i]);
      }
      soma_rb += contador_rb;

      // Árvore B ordem 1
      ArvoreB *b1 = criaArvore_b(1);
      for (int i = 0; i < tamanho; i++) {
        adicionaChave_b(b1, dados[i]);
      }
      contador_b = 0; 
      for (int i = 0; i < tamanho / 2; i++) {
        removerChave_b(b1, dados_remover[i]);
      }
      soma_b1 += contador_b;

      // Árvore B ordem 5
      ArvoreB *b5 = criaArvore_b(5);
      for (int i = 0; i < tamanho; i++) {
        adicionaChave_b(b5, dados[i]);
      }
      contador_b = 0; 
      for (int i = 0; i < tamanho / 2; i++) {
        removerChave_b(b5, dados_remover[i]);
      }
      soma_b5 += contador_b;

      // Árvore B ordem 10
      ArvoreB *b10 = criaArvore_b(10);
      for (int i = 0; i < tamanho; i++) {
        adicionaChave_b(b10, dados[i]);
      }
      contador_b = 0; 
      for (int i = 0; i < tamanho / 2; i++) {
        removerChave_b(b10, dados_remover[i]);
      }
      soma_b10 += contador_b;

      free(dados);
      free(dados_remover);
      free(avl);
      free(rb);
      free(b1);
      free(b5);
      free(b10);
    }

    double media_add_avl = (double)soma_avl / 10.0;
    double media_add_rb = (double)soma_rb / 10.0;
    double media_add_b1 = (double)soma_b1 / 10.0;
    double media_add_b5 = (double)soma_b5 / 10.0;
    double media_add_b10 = (double)soma_b10 / 10.0;

    fprintf(
      arquivo_csv_remocao,
      "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
      tamanho, media_add_avl, media_add_rb, media_add_b1, media_add_b5, media_add_b10
    );

    double media_add_avl = (double)soma_avl / 10.0;
    double media_add_rb = (double)soma_rb / 10.0;
    double media_add_b1 = (double)soma_b1 / 10.0;
    double media_add_b5 = (double)soma_b5 / 10.0;
    double media_add_b10 = (double)soma_b10 / 10.0;

    fprintf(
      arquivo_csv_adicao,
      "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
      tamanho, media_rem_avl, media_rem_rb, media_rem_b1, media_rem_b5, media_rem_b10
    );

  }
}

int main() {
    srand(time(NULL));

    FILE *arquivo_adicao = fopen("resultados_adicao.csv", "w");
    if (arquivo_adicao == NULL) {
        printf("Erro ao criar arquivo de resultados para adição!\n");
        return 1;
    }

    FILE *arquivo_remocao = fopen("resultados_remocao.csv", "w");
    if (arquivo_remocao == NULL) {
      printf("Erro ao criar arquivo de resultados para remoção!\n");
      return 1;
    }

    teste(arquivo_adicao, arquivo_remocao);

    fclose(arquivo_remocao);
    fclose(arquivo_adicao);

    return 0;
}