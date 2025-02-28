#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 1000 // Número máximo de vértices no grafo

// Função para adicionar uma aresta
void adicionar_aresta(int matriz[MAX_VERTICES][MAX_VERTICES], int x, int y) {
    matriz[x-1][y-1]++; // Aumenta o número de arestas de x para y
    matriz[y-1][x-1]++; // Aumenta o número de arestas de y para x (grafo não orientado)
}

// Função para verificar se todos os vértices têm grau par
int verificar_grau_par(int matriz[MAX_VERTICES][MAX_VERTICES], int n) {
    for (int i = 0; i < n; i++) {
        int grau = 0;
        for (int j = 0; j < n; j++) {
            grau += matriz[i][j];
        }
        if (grau % 2 != 0) {
            return 0; // Se algum vértice tiver grau ímpar, não é euleriano
        }
    }
    return 1; // Todos os vértices têm grau par
}

// Função para gerar um grafo euleriano e salvar no arquivo
void gerar_grafo_euleriano(int matriz[MAX_VERTICES][MAX_VERTICES], int n, FILE *f) {
    // Criar um grafo simples, onde cada vértice i é conectado ao próximo
    for (int i = 1; i <= n; i++) {
        // Conectar cada vértice ao próximo para formar um ciclo
        adicionar_aresta(matriz, i, (i % n) + 1);
    }

    // Escrever o grafo no arquivo no formato desejado
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) { // Evitar duplicação (i, j) e (j, i)
            if (matriz[i][j] > 0) {
                for (int k = 0; k < matriz[i][j]; k++) {
                    fprintf(f, "%d %c %d\n", i+1, 'A', j+1); // Escreve a aresta
                }
            }
        }
    }
}

int main() {
    int n = 1000; // Número de vértices no grafo
    int matriz[MAX_VERTICES][MAX_VERTICES] = {0}; // Matriz de adjacência inicializada a zero
    
    // Abrir o arquivo para escrita
    FILE *f = fopen("grafoEuleriano.txt", "w");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    // Gerar o grafo euleriano e escrever no arquivo
    gerar_grafo_euleriano(matriz, n, f);

    // Fechar o arquivo
    fclose(f);
    printf("Grafo euleriano gerado e salvo no arquivo 'grafoEuleriano.txt'.\n");

    return 0;
}
