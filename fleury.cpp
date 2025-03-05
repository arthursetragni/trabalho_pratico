#include<iostream>
#include<vector>
using namespace std;
#define NODE 10000

int grafo[NODE][NODE] = {0};

void adicionaAresta(int origem, int destino){
    grafo[origem][destino] = 1;
    grafo[destino][origem] = 1;
}

int tempgrafo[NODE][NODE];

int encontraInicial(){
   for(int i = 0; i<NODE; i++){
      int grau = 0;
      for(int j = 0; j<NODE; j++){
         if(tempgrafo[i][j])
         grau++;
      }
      if(grau % 2 != 0)
      return i;
   }
   return 0;
}
bool ehPonte(int u, int v){
   int grau = 0;
   for(int i = 0; i<NODE; i++)
      if(tempgrafo[v][i])
         grau++;
      if(grau>1){
         return false;
      }
   return true;
}
int contaAresta(){
   int count = 0;
   for(int i = 0; i<NODE; i++)
      for(int j = i; j<NODE; j++)
         if(tempgrafo[i][j])
            count++;
   return count;
}
void fleury(int start){
   static int edge = contaAresta();
   for(int v = 0; v<NODE; v++){
      if(tempgrafo[start][v]){ 
         if(edge <= 1 || !ehPonte(start, v)){
            cout << start << "--" << v << " ";
            tempgrafo[start][v] = tempgrafo[v][start] = 0; 
            edge--;
            fleury(v);
         }
      }
   }
}
int main(){
    // adicionaAresta(0, 1);
    // adicionaAresta(0, 3);
    // adicionaAresta(0, 4);
    // adicionaAresta(1, 2);
    // adicionaAresta(1, 3);
    // adicionaAresta(1, 4);
    // adicionaAresta(2, 3);
    // adicionaAresta(2, 4);
    // adicionaAresta(2, 5);
    // adicionaAresta(3, 5);
    // adicionaAresta(4, 5);
    // adicionaAresta(5, 6);
    // adicionaAresta(6, 7);
    // adicionaAresta(6, 8);
    // adicionaAresta(7, 8);

    for(int i = 0; i < NODE - 1; i++){
        adicionaAresta(i, i+1);
    }
    adicionaAresta(NODE - 1, 0);
   for(int i = 0; i<NODE; i++)
   for(int j = 0; j<NODE; j++)
   tempgrafo[i][j] = grafo[i][j];
   cout << "Caminho ou ciclo eulriano: ";
   fleury(encontraInicial());
}