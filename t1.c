#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
 // adicionaAresta(0, 1, grafo);
    // adicionaAresta(1, 2, grafo);
    // adicionaAresta(2, 3, grafo);
    // adicionaAresta(1, 3, grafo);
    // adicionaAresta(0, 3, grafo);
    // adicionaAresta(0, 4, grafo);
    // adicionaAresta(1, 4, grafo);
    // adicionaAresta(2, 4, grafo);
    // adicionaAresta(2, 5, grafo);
    // adicionaAresta(3, 5, grafo);
    // adicionaAresta(4, 5, grafo);
    // adicionaAresta(5, 6, grafo);
    // adicionaAresta(6, 7, grafo);
    // adicionaAresta(6, 8, grafo);
    // adicionaAresta(7, 8, grafo);
// Declaração das estruturas de dados
//==========================================================================================
typedef struct No
{
    struct No *prox;
    struct No *ant;
    int valor;
} No;

typedef struct Lista
{
    No *cabeca;
    No *cauda;
} Lista;

typedef struct Grafo
{
    int vertices;
    int tempo;
    Lista **adj;
} Grafo;

//==========================================================================================

// Funções para criar as estruturas de dados

No *novoNo(int valor)
{
    No *novo = (No *)malloc(sizeof(No));
    novo->valor = valor;
    novo->prox = NULL;
    novo->ant = NULL;
    return novo;
}

Lista *novaLista()
{
    Lista *nova = (Lista *)malloc(sizeof(Lista));
    nova->cabeca = NULL;
    nova->cauda = NULL;
    return nova;
}

Grafo *novoGrafo(int vertices)
{
    Grafo *novo = (Grafo *)malloc(sizeof(Grafo));
    novo->vertices = vertices;
    novo->adj = malloc(vertices * sizeof(Lista *));
    novo->tempo = 0;
    for (int i = 0; i < vertices; i++)
    {
        novo->adj[i] = novaLista();
    }
    return novo;
}

//==========================================================================================

// Funções para adição de arestas e nós

void adicionarNo(int valor, Lista *lista)
{
    No *novo = novoNo(valor);
    if (lista->cabeca == NULL)
    {
        lista->cabeca = novo;
        lista->cauda = novo;
    }
    else
    {
        lista->cauda->prox = novo;
        novo->ant = lista->cauda;
        lista->cauda = novo;
    }
}

void adicionaAresta(int v, int w, Grafo *grafo)
{
    adicionarNo(v, grafo->adj[w]);
    adicionarNo(w, grafo->adj[v]);
}

//==========================================================================================

// Função recursiva para encontrar as pontes

void encontrarPontes(Grafo *grafo, int u, bool *visitado, int *descoberta, int *baixo, int *pai)
{
    visitado[u] = true;

    descoberta[u] = baixo[u] = ++grafo->tempo;

    No *tmp = grafo->adj[u]->cabeca;
    while (tmp != NULL)
    {

        int v = tmp->valor;

        if (!visitado[v])
        {
            pai[v] = u;
            encontrarPontes(grafo, v, visitado, descoberta, baixo, pai);

            baixo[u] = baixo[u] < baixo[v] ? baixo[u] : baixo[v];

            if (baixo[v] > descoberta[u])
            {
                printf("%d %d\n", u, v);
            }
        }

        else if (v != pai[u])
        {
            baixo[u] = baixo[u] < descoberta[v] ? baixo[u] : descoberta[v];
        }
        tmp = tmp->prox;
    }
}

// Método principal da busca de pontes,que chama a recursão e testa em todos os vértices caso haja mais de um componente no grafo
void pontes(Grafo *grafo)
{
    bool *visitado = (bool *)malloc(grafo->vertices * sizeof(bool));
    int *descoberta = (int *)malloc(grafo->vertices * sizeof(int));
    int *baixo = (int *)malloc(grafo->vertices * sizeof(int));
    int *pai = (int *)malloc(grafo->vertices * sizeof(int));

    for (int i = 0; i < grafo->vertices; i++)
    {
        visitado[i] = false;
        pai[i] = -1;
    }

    for (int i = 0; i < grafo->vertices; i++)
    {
        if (visitado[i] == false)
            encontrarPontes(grafo, i, visitado, descoberta, baixo, pai);
    }
}
//==========================================================================================
// Método principal
int main()
{
    // Grafo teste mostrado no documento

    Grafo *grafo = novoGrafo(9);
    adicionaAresta(0, 1, grafo);
    adicionaAresta(1, 2, grafo);
    adicionaAresta(2, 3, grafo);
    adicionaAresta(1, 3, grafo);
    adicionaAresta(0, 3, grafo);
    adicionaAresta(0, 4, grafo);
    adicionaAresta(1, 4, grafo);
    adicionaAresta(2, 4, grafo);
    adicionaAresta(2, 5, grafo);
    adicionaAresta(3, 5, grafo);
    adicionaAresta(4, 5, grafo);
    adicionaAresta(5, 6, grafo);
    adicionaAresta(6, 7, grafo);
    adicionaAresta(6, 8, grafo);
    adicionaAresta(7, 8, grafo);

    pontes(grafo);

    return 0;
}
