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
    int grau;
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
    nova->grau = 0;
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
    lista->grau++;
}

void adicionaAresta(int v, int w, Grafo *grafo)
{
    adicionarNo(v, grafo->adj[w]);
    adicionarNo(w, grafo->adj[v]);
}

void desconectaVertices(Grafo *grafo, int start, int v)
{
    if (start < 0 || start >= grafo->vertices || v < 0 || v >= grafo->vertices)
    {
        return;
    }

    No *no = grafo->adj[start]->cabeca;
    while (no != NULL)
    {
        if (no->valor == v)
        {
            if (no->ant != NULL)
            {
                no->ant->prox = no->prox;
            }
            else
            {
                grafo->adj[start]->cabeca = no->prox;
            }
            if (no->prox != NULL)
            {
                no->prox->ant = no->ant;
            }
            else
            {
                grafo->adj[start]->cauda = no->ant;
            }
            free(no);
            no = NULL;
        }
        if (no != NULL)
            no = no->prox;
    }
    grafo->adj[start]->grau--;
    no = grafo->adj[v]->cabeca;
    while (no != NULL)
    {
        if (no->valor == start)
        {
            if (no->ant != NULL)
            {
                no->ant->prox = no->prox;
            }
            else
            {
                grafo->adj[v]->cabeca = no->prox;
            }
            if (no->prox != NULL)
            {
                no->prox->ant = no->ant;
            }
            else
            {
                grafo->adj[v]->cauda = no->ant;
            }
            free(no);
            no = NULL;
        }
        if (no != NULL)
            no = no->prox;
    }
    grafo->adj[v]->grau--;
}

//==========================================================================================
// Função recursiva para encontrar as pontes

bool encontrarPontes(Grafo *grafo, int u, bool *visitado, int *descoberta, int *baixo, int *pai, int i, int j)
{
    bool ponte = false;
    visitado[u] = true;

    descoberta[u] = baixo[u] = ++grafo->tempo;

    No *tmp = grafo->adj[u]->cabeca;
    while (tmp != NULL)
    {

        int v = tmp->valor;

        if (!visitado[v])
        {
            pai[v] = u;
            ponte = encontrarPontes(grafo, v, visitado, descoberta, baixo, pai, i, j);

            baixo[u] = baixo[u] < baixo[v] ? baixo[u] : baixo[v];

            if (baixo[v] > descoberta[u] && ((i == u && j == v) || (i == v && j == u)))
            {
                return true;
            }
        }

        else if (v != pai[u])
        {
            baixo[u] = baixo[u] < descoberta[v] ? baixo[u] : descoberta[v];
        }
        tmp = tmp->prox;
    }
    return ponte;
}

// Método principal da busca de pontes,que chama a recursão e testa em todos os vértices caso haja mais de um componente no grafo
bool pontes(Grafo *grafo, int u, int v)
{
    bool resp = false;
    bool *visitado = (bool *)malloc(grafo->vertices * sizeof(bool));
    int *descoberta = (int *)malloc(grafo->vertices * sizeof(int));
    int *baixo = (int *)malloc(grafo->vertices * sizeof(int));
    int *pai = (int *)malloc(grafo->vertices * sizeof(int));

    for (int i = 0; i < grafo->vertices; i++)
    {
        visitado[i] = false;
        pai[i] = -1;
    }

    for (int i = 0; i < grafo->vertices && !resp; i++)
    {
        if (visitado[i] == false && !resp)
            resp = encontrarPontes(grafo, i, visitado, descoberta, baixo, pai, u, v);
    }
    return resp;
}

//==========================================================================================

bool podeSerEuleriano(Grafo *grafo)
{
    int impares = 0;
    for (int i = 0; i < grafo->vertices; i++)
    {
        if (grafo->adj[i]->grau % 2 != 0)
        {
            impares++;
        }
    }
    if (impares == 0 || impares == 2)
    {
        return true;
    }
    return false;
}

void DFS(Grafo *grafo, int v, bool *visitado)
{
    visitado[v] = true;
    No *no = grafo->adj[v]->cabeca;
    while (no != NULL)
    {
        if (!visitado[no->valor])
        {
            DFS(grafo, no->valor, visitado);
        }
        no = no->prox;
    }
}

bool estaConexo(Grafo *grafo)
{
    bool *visitado = (bool *)malloc(grafo->vertices * sizeof(bool));
    for (int i = 0; i < grafo->vertices; i++)
    {
        visitado[i] = false;
    }

    DFS(grafo, 0, visitado);

    bool conexo = true;

    for (int i = 0; i < grafo->vertices; i++)
    {
        if (!visitado[i])
        {
            conexo = false;
        }
    }

    return conexo;
}

int estaConectado(Grafo *grafo, int start, int v)
{

    if (start < 0 || start >= grafo->vertices || v < 0 || v >= grafo->vertices)
    {
        return 0;
    }

    No *no = grafo->adj[start]->cabeca;
    while (no != NULL)
    {
        if (no->valor == v)
        {
            return 1;
        }
        no = no->prox;
    }

    return 0;
}

int encontraInicial(Grafo *grafo)
{
    int grau = 0;
    for (int i = 0; i < grafo->vertices; i++)
    {
        grau = grafo->adj[i]->grau;
        if (grau % 2 != 0)
        {
            return i;
        }
    }
    return 0;
}

void fleury(Grafo *grafo, int start, int edge)
{
    for (int v = 0; v < grafo->vertices; v++)
    {
        if (estaConectado(grafo, start, v))
        {
            if (grafo->adj[start]->grau == 1 || !pontes(grafo, start, v))
            {
                printf("%d--%d ", start, v);
                desconectaVertices(grafo, start, v);
                edge--;
                fleury(grafo, v, edge);
            }
        }
    }
}

int contaArestas(Grafo *grafo)
{
    int count = 0;

    for (int i = 0; i < grafo->vertices; i++)
    {
        count += grafo->adj[i]->grau;
    }

    return count / 2;
}



//==========================================================================================
// Método principal

int main()
{
    int vertices[] = {3333};
    int n_vertices = 1;
    for(int i = 0; i < n_vertices; i++){
        Grafo *grafo = novoGrafo(vertices[i]);
    
        for(int j = 0; j < grafo->vertices; j++){
            for(int k = 0; k < grafo->vertices;k++){
                if(k != j && j < k) adicionaAresta(j, k, grafo);
            }
            printf("%d\n", j);
        }

        if (!podeSerEuleriano(grafo) || !estaConexo(grafo))
        {
            printf("Grafo nao possui caminho ou ciclo euleriano\n");
            return 0;
        }

        clock_t start_time, end_time;
        double total_time;

        //printf("Caminho ou ciclo eulriano: ");

        start_time = clock();

        fleury(grafo, encontraInicial(grafo), contaArestas(grafo));

        end_time = clock();

        total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        printf("\nTempo de execucao da funcao Fleury com %d vertices: %.6f segundos\n", vertices[i], total_time);

        liberaGrafo(grafo);
    }
    return 0;
}