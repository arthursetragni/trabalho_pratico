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

int grafoConexo(Grafo *g) {
  int visitado[MAX_VERTICES] = {0};
  dfs(g, 0, visitado);

  for (int i = 0; i < g->num_vertices; i++) {
    if (g->grau[i] > 0 && !visitado[i]) {
      return 0; // Grafo não é conexo
    }
  }
  return 1; // Grafo é conexo
}

// Função auxiliar para DFS de Tarjan
void tarjanDFS(Grafo *g, int u, int parent, int *time, int disc[], int low[],
               int parentArr[], int visitado[], int *pontes) {
  visitado[u] = 1;
  disc[u] = low[u] = ++(*time);

  for (int v = 0; v < g->num_vertices; v++) {
    if (g->adj[u][v]) {
      if (!visitado[v]) {
        parentArr[v] = u;
        tarjanDFS(g, v, u, time, disc, low, parentArr, visitado, pontes);
        low[u] = (low[u] < low[v]) ? low[u] : low[v];

        // Se low[v] > disc[u], então (u, v) é uma ponte
        if (low[v] > disc[u]) {
          printf("Aresta (%d, %d) é uma ponte.\n", u, v);
          (*pontes)++;
        }
      } else if (v != parent) {
        low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
      }
    }
  }
}

// Função para verificar se a aresta (u, v) é uma ponte usando Tarjan
int verificaPonte2(Grafo *g, int u, int v) {
  int disc[MAX_VERTICES] = {-1};      // Tempo de descoberta
  int low[MAX_VERTICES] = {-1};       // Menor tempo de chegada
  int parentArr[MAX_VERTICES] = {-1}; // Vértice pai
  int visitado[MAX_VERTICES] = {0};
  int time = 0;
  int pontes = 0;

  tarjanDFS(g, u, -1, &time, disc, low, parentArr, visitado, &pontes);

  // Verifica se a aresta (u, v) foi identificada como ponte
  for (int i = 0; i < g->num_vertices; i++) {
    if (g->adj[u][i] && low[i] > disc[u] && (i == v)) {
      return 1; // Aresta é uma ponte
    }
  }

  return 0; // Aresta não é uma ponte
}

int podeRemoverAresta(Grafo *g, int origem, int destino) {
  // Verifica se a aresta (origem, destino) não é uma ponte
  if (verificaPonte2(g, origem, destino)) {
    return 0; // Se for ponte, não pode remover
  }
  return 1; // Se não for ponte, pode remover
}

void fleury(Grafo *g, int v) {
  printf("Caminho de Euler: ");

  while (1) {
    printf("%d ", v);
    int encontrado = 0;

    for (int i = 0; i < g->num_vertices; i++) {
      if (existeAresta(g, v, i)) {
        // Se for a última aresta ou se a remoção da aresta não desconectar o
        // grafo
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
