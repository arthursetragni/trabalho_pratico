#include <stdio.h>
#include <stdlib.h>

#define NODE 100000

int **grafo;
int **tempgrafo;


//int grafo[NODE][NODE] = {0};

void adicionaAresta(int origem, int destino) {
    grafo[origem][destino] = 1;
    grafo[destino][origem] = 1;
}



int encontraInicial() {
    for (int i = 0; i < NODE; i++) {
        int grau = 0;
        for (int j = 0; j < NODE; j++) {
            if (tempgrafo[i][j]) {
                grau++;
            }
        }
        if (grau % 2 != 0) {
            return i;
        }
    }
    return 0;
}

int ehPonte(int u, int v) {
    int grau = 0;
    for (int i = 0; i < NODE; i++) {
        if (tempgrafo[v][i]) {
            grau++;
        }
    }
    if (grau > 1) {
        return 0;
    }
    return 1;
}

int contaAresta() {
    int count = 0;
    for (int i = 0; i < NODE; i++) {
        for (int j = i; j < NODE; j++) {
            if (tempgrafo[i][j]) {
                count++;
            }
        }
    }
    return count;
}

void fleury(int start, int edge) {
    
    for (int v = 0; v < NODE; v++) {
        if (tempgrafo[start][v]) {
            if (edge <= 1 || !ehPonte(start, v)) {
                printf("%d--%d ", start, v);
                tempgrafo[start][v] = tempgrafo[v][start] = 0;
                edge--;
                fleury(v, edge);
            }
        }
    }
}

int main() {
  grafo = (int **)calloc(NODE, sizeof(int *));
  tempgrafo = (int **)calloc(NODE, sizeof(int *));
  for (int i = 0; i < NODE; i++) {
      grafo[i] = (int *)calloc(NODE, sizeof(int));
      tempgrafo[i] = (int *)calloc(NODE, sizeof(int));
  }

    for (int i = 0; i < NODE - 1; i++) {
        adicionaAresta(i, i + 1);
    }
    adicionaAresta(NODE - 1, 0);

    for (int i = 0; i < NODE; i++) {
        for (int j = 0; j < NODE; j++) {
            tempgrafo[i][j] = grafo[i][j];
        }
    }

    printf("Caminho ou ciclo eulriano: ");
    int edge = contaAresta();
    fleury(encontraInicial(), edge);

    return 0;
}
