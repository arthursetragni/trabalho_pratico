#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

Grafo *grafo;


int encontraInicial() {
    for (int i = 0; i < grafo->vertices; i++) {
        int grau = 0;
        No *no = grafo->adj[i]->cabeca;
        
        while (no != NULL) {
            grau++;  
            no = no->prox; 
        }
        if(grau % 2 != 0){
            return i;
        }
        
    }
    return 0;
}

int ehPonte(int vertice){
    int grau = 0;
    No *no = grafo->adj[vertice]->cabeca;
    
    while (no != NULL) {
        grau++;
        no = no->prox;
    }
    if (grau > 1) {
        return 0;
    }
    return 1;
}

int contaArestas(Grafo *grafo) {
    int count = 0;
    
    for (int i = 0; i < grafo->vertices; i++) {
        No *no = grafo->adj[i]->cabeca;
        
        while (no != NULL) {
            count++;
            no = no->prox;
        }
    }
    
    return count;
}

int estaConectado(int start, int v) {

    if (start < 0 || start >= grafo->vertices || v < 0 || v >= grafo->vertices) {
        return 0;  
    }

   
    No *no = grafo->adj[start]->cabeca;
    while (no != NULL) {
        if (no->valor == v) {
            return 1;
        }
        no = no->prox; 
    }

    return 0;
}

void fleury(int start, int edge) {
    for (int v = 0; v < grafo->vertices; v++) {
        if (estaConectado(start, v)){
            if (edge <= 1 || !ehPonte(v)) {
                printf("%d--%d ", start, v);
                tempgrafo[start][v] = tempgrafo[v][start] = 0;
                edge--;
                fleury(v, edge);
            }
        }
    }
}
