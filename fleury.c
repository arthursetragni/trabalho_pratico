#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50

typedef struct Grafo {
  int adj[MAX_VERTICES][MAX_VERTICES];
  int grau[MAX_VERTICES];
  int num_vertices;
} Grafo;

void inicializarGrafo(Grafo *g, int vertices) {
  g->num_vertices = vertices;

  for (int i = 0; i < vertices; i++) {
    g->grau[i] = 0;
    for (int j = 0; j < vertices; j++) {
      g->adj[i][j] = 0;
    }
  }
}

void adicionarAresta(Grafo *g, int origem, int destino) {
  g->adj[origem][destino] = 1;
  g->adj[destino][origem] = 1; // Grafo não direcionado
  g->grau[origem]++;
  g->grau[destino]++;
}

int existeAresta(Grafo *g, int origem, int destino) {
  return g->adj[origem][destino] == 1;
}

int grauImpar(Grafo *g) {
  int count = 0;
  for (int i = 0; i < g->num_vertices; i++) {
    if (g->grau[i] % 2 != 0) {
      count++;
    }
  }
  return count;
}

void dfs(Grafo *g, int v, int visitado[]) {
  visitado[v] = 1;
  for (int i = 0; i < g->num_vertices; i++) {
    if (g->adj[v][i] == 1 && !visitado[i]) {
      dfs(g, i, visitado);
    }
  }
}

// Função para verificar se a aresta (u, v) é uma ponte
int verificaPonte(Grafo *g, int u, int v) {
  // Remove a aresta (u, v)
  g->adj[u][v] = 0;
  g->adj[v][u] = 0;
  g->grau[u]--;
  g->grau[v]--;

  // Verifica se o grafo ainda é conexo após a remoção
  int visitado[MAX_VERTICES] = {0};
  dfs(g, u, visitado);

  // Se o vértice v não foi visitado, a aresta (u, v) é uma ponte
  int resultado = !visitado[v];

  // Restaura a aresta (u, v)
  g->adj[u][v] = 1;
  g->adj[v][u] = 1;
  g->grau[u]++;
  g->grau[v]++;

  return resultado;
}

int podeRemoverAresta(Grafo *g, int origem, int destino) {
  // Verifica se a aresta (origem, destino) não é uma ponte
  if (verificaPonte(g, origem, destino) && g->grau[origem] > 1) {
    return 0; // Se for ponte, não pode remover
  }
  return 1; // Se não for ponte, pode remover
}
void fleury(Grafo *g, int v) {
    printf("Caminho de Euler: ");
  
    while (1) {
      printf("%d ", v);
      int encontrado = 0;
  
      // Modificado para iniciar a busca da aresta a partir de 0
      for (int i = 0; i < g->num_vertices; i++) {
        if (existeAresta(g, v, i)) {
          // Se for a última aresta ou se a remoção da aresta não desconectar o grafo
          if (g->grau[v] == 1 || podeRemoverAresta(g, v, i)) {
            printf("%d ", i);
            g->adj[v][i] = 0;
            g->adj[i][v] = 0;
            g->grau[v]--;
            g->grau[i]--;
            v = i;
            encontrado = 1;
            break;
          }
        }
      }
  
      if (!encontrado) {
        break; // Fim do caminho de Euler
      }
    }
  
    printf("\n");
}
  
int main() {
  Grafo g;
  inicializarGrafo(&g, 5);

  // Adicionando arestas ao grafo
  adicionarAresta(&g, 0, 1);
  adicionarAresta(&g, 0, 2);
  adicionarAresta(&g, 1, 2);
  adicionarAresta(&g, 1, 3);
  adicionarAresta(&g, 2, 4);
  adicionarAresta(&g, 3, 4);

  // Verifica se o grafo tem um caminho de Euler
  if (grauImpar(&g) == 0 || grauImpar(&g) == 2) {
    // Inicia o algoritmo de Fleury a partir do vértice 0
    fleury(&g, 0);
  } else {
    printf("O grafo não tem um caminho de Euler.\n");
  }

  return 0;
}
