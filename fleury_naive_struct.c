#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>




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


void liberaNo(No *no)
{
    if (no != NULL)
    {
        free(no);
    }
}

void liberaLista(Lista *lista)
{
    if (lista != NULL)
    {
        No *atual = lista->cabeca;
        No *temp;
        
        // Liberando os nós da lista
        while (atual != NULL)
        {
            temp = atual;
            atual = atual->prox;
            liberaNo(temp);
        }

        // Liberando a lista em si
        free(lista);
    }
}

void liberaGrafo(Grafo *grafo)
{
    if (grafo != NULL)
    {
        // Liberando as listas de adjacência
        for (int i = 0; i < grafo->vertices; i++)
        {
            liberaLista(grafo->adj[i]);
        }

        // Liberando o array de listas de adjacência
        free(grafo->adj);

        // Liberando o grafo
        free(grafo);
    }
}

//==========================================================================================

// Funções para adição de arestas e nós

int contaGrau(int vertice) {
    int grau = 0;

    if (vertice < 0 || vertice >= grafo->vertices) {
        printf("Vértice inválido.\n");
        return -1;  
    }

    No *no = grafo->adj[vertice]->cabeca;
    
    while (no != NULL) {
        grau++;     
        no = no->prox; 
    }

    return grau;
}

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

void desconectaVertices(int start, int v, int *ignorados) {
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
    if (contaGrau(start) == 0) {
        ignorados[start] = 1;
    }
    if (contaGrau(v) == 0) {
        ignorados[v] = 1;
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

void dfsVisitado(int vertice, bool *visitado) {
    visitado[vertice] = true;
    
    No *no = grafo->adj[vertice]->cabeca;
    while (no != NULL) {
        int v = no->valor;
        if (!visitado[v]) {
            dfsVisitado(v, visitado);
        }
        no = no->prox;
    }
}

void desconectaAresta(int u, int v) {
    No *anterior = NULL;
    No *atual = grafo->adj[u]->cabeca;
    
    while (atual != NULL && atual->valor != v) {
        anterior = atual;
        atual = atual->prox;
    }
    
    if (atual != NULL) {
        if (anterior == NULL) {
            grafo->adj[u]->cabeca = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }
        free(atual);
    }

    anterior = NULL;
    atual = grafo->adj[v]->cabeca;
    
    while (atual != NULL && atual->valor != u) {
        anterior = atual;
        atual = atual->prox;
    }
    
    if (atual != NULL) {
        if (anterior == NULL) {
            grafo->adj[v]->cabeca = atual->prox;
        } else {
            anterior->prox = atual->prox;
        }
        free(atual);
    }
}


int ehPonte(int u, int v, int *ignorados) {
   
    bool *visitado = (bool *)malloc(grafo->vertices * sizeof(bool));
    for (int i = 0; i < grafo->vertices; i++) {
        visitado[i] = false;
        if(ignorados[i] == 1) visitado[i] = true;
    }

    


    
    desconectaAresta(u, v);

    
    dfsVisitado(u, visitado);

    
    bool ehConexo = true;
    for (int i = 0; i < grafo->vertices; i++) {
        if (!visitado[i]) {
            ehConexo = false;
            break;
        }
    }

    No *novoNoU = novoNo(v);
    novoNoU->prox = grafo->adj[u]->cabeca;
    grafo->adj[u]->cabeca = novoNoU;
    
    No *novoNoV = novoNo(u);
    novoNoV->prox = grafo->adj[v]->cabeca;
    grafo->adj[v]->cabeca = novoNoV;

    free(visitado);

    return !ehConexo;
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

void fleury(int start, int edge, int *ignorados) {
    for (int v = 0; v < grafo->vertices; v++) {
        if (estaConectado(start, v)){
            if (contaGrau(start) <= 1 || !ehPonte(start, v, ignorados)) {
                printf("%d--%d ", start, v);
                desconectaVertices(start, v, ignorados);
                edge--;
                fleury(v, edge, ignorados);
            }
        }
    }
}


int main()
{
    int vertices[] = {111};
    int n_vertices = 1;
    for(int i = 0; i < n_vertices; i++){
        grafo = novoGrafo(vertices[i]);
        

        for(int j = 0; j < grafo->vertices; j++){
            for(int k = 0; k < grafo->vertices;k++){
                if(k != j && j < k) adicionaAresta(j, k, grafo);
            }
            printf("%d\n", j);
        }
        

        clock_t start_time, end_time;
        double total_time;

        //printf("Caminho ou ciclo eulriano: ");
        int *ignorados = calloc(vertices[i], sizeof(int));

        start_time = clock();

        fleury(encontraInicial(), contaArestas(), ignorados);

        end_time = clock();

        total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        printf("\nTempo de execucao da funcao Fleury com %d vertices: %.6f segundos\n", vertices[i], total_time);
        liberaGrafo(grafo);
    }
    return 0;
}