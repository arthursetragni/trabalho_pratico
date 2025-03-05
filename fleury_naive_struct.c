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

Grafo *grafo;
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

void desconectaVertices(int start, int v) {
    if (start < 0 || start >= grafo->vertices || v < 0 || v >= grafo->vertices) {
        return;
    }

    No *no = grafo->adj[start]->cabeca;
    No *prev = NULL;
    while (no != NULL) {
        if (no->valor == v) {
            if (prev == NULL) {
                grafo->adj[start]->cabeca = no->prox;
            } else {
                prev->prox = no->prox;
            }
            if (no->prox != NULL) {
                no->prox->ant = prev;
            }
            free(no);
            break;
        }
        prev = no;
        no = no->prox;
    }

    no = grafo->adj[v]->cabeca;
    prev = NULL;
    while (no != NULL) {
        if (no->valor == start) {
            if (prev == NULL) {
                grafo->adj[v]->cabeca = no->prox;
            } else {
                prev->prox = no->prox;
            }
            if (no->prox != NULL) {
                no->prox->ant = prev;
            }
            free(no);
            break;
        }
        prev = no;
        no = no->prox;
    }
}


//==========================================================================================




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

int contaArestas() {
    int count = 0;
    
    for (int i = 0; i < grafo->vertices; i++) {
        No *no = grafo->adj[i]->cabeca;
        
        while (no != NULL) {
            count++;
            no = no->prox;
        }
    }
    
    return count / 2;
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
                desconectaVertices(start, v);
                edge--;
                fleury(v, edge);
            }
        }
    }
}


int main()
{
    // Grafo teste mostrado no documento

    grafo = novoGrafo(10000);
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
    for (int i = 0; i < grafo->vertices - 1; i++) {
        adicionaAresta(i, i + 1, grafo);
    }
    adicionaAresta(grafo->vertices - 1, 0, grafo);

    printf("Caminho ou ciclo eulriano: ");
    fleury(encontraInicial(), contaArestas());

    return 0;
}