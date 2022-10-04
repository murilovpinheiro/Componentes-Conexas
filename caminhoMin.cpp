#include <iostream>
#include <cstdio>
#include <list>

using namespace std;

struct vertice{

    int data;
    float weight;

};
// heap que não é heap pois não consegui
struct Heap{

    int atual, max;
    int* pos;
    vertice* vetor;

    // Inicia o Heap vazio
    void inicializar(int n){
        atual = 0;
        max = n;
        vetor = new vertice[n];
        pos = new int[n];
        for(int i = 0; i < max; i++){
            vetor[i].data = -1;
            vetor[i].weight = 0;
            pos[i] = -1;
        }
    }

    // remove o menor
    vertice removerMin(){
        vertice min;
        int m;
        min.data = -1;
        min.weight = 0;
        // Retorna um vertice nulo caso o heap esteja vazio 
        if(atual == 0)
            return min;
        for(int i = 0; i < max; i++){
            if(vetor[i].data != -1){
                min = vetor[i];
                m = i;
                break;
            }
        }
        for(int i = m; i < max; i++){
            if(vetor[i].data != -1 && vetor[i].weight < min.weight){
                min = vetor[i];
            }
        }
        vetor[pos[min.data - 1]].data = -1;
        pos[min.data - 1] = -1;
        atual -= 1;
        return min;
    }

    // Inclui um vértice no heap
    void inserir(int vertice, float peso){
        if(atual == max)
            return;
        atual += 1;
        for(int i = 0; i < max; i++){
            if(vetor[i].data == -1){
                vetor[i].data = vertice;
                vetor[i].weight = peso;
                pos[vertice - 1] = i;
                return;
            }
        }
    }

    bool atualizarSeMenor(int n, float novo){
        if(vetor[pos[n-1]].data != -1 && vetor[pos[n-1]].weight > novo){
            vetor[pos[n-1]].weight = novo;
            return true;
        }
        return false;
    }

    bool pertence(vertice v){
        if(pos[v.data - 1] != -1)
                return true;
        return false;
    }
};

struct Dist{
    bool* Inf;
    float* d;
    void inicializar(int n){
        Inf = new bool[n];
        d = new float[n];
        for(int i = 0; i < n; i++){
            Inf[i] = true;
            d[i] = 0;
        }
    }
};

int main(){

    float c;
    bool* dentro;
    int* pai, a, b, x, n;
    list<vertice>* lista;
    vertice aux;
    Dist dist;
    Heap H;

    scanf("dl\nformat=edgelist1\nn=%d\ndata:", &n);

    //inicia os vetores e as estruturas de dados de vamos utilizar
    dentro = new bool[n];
    pai = new int[n];
    lista = new list<vertice>[n];
    H.inicializar(n);
    dist.inicializar(n);
    for(int i = 0; i < n; i++){
        dentro[i] = false;
    }
    //importante fazer antes do while pois estava dando problema depois

    //recebendo as entradas
    while(1){

        x = scanf("%d %d %f", &a, &b, &c);

        if(x != 3)
            break;

        aux.data = b;
        aux.weight = c;

        lista[a - 1].push_back(aux);
    }

    pai[0] = 1;
    aux.data = 1;
    aux.weight = 0;
    H.inserir(aux.data, aux.weight);

    //LOOP de Dijsktra padrão sem diferenças
    while(H.atual > 0){
        aux = H.removerMin();
        dentro[aux.data - 1] = true;
        dist.d[aux.data - 1] = aux.weight;
        dist.Inf[aux.data - 1] = false;
        for(vertice v : lista[aux.data - 1]){
            if(!dentro[v.data - 1] && !H.pertence(v)){
                H.inserir(v.data, dist.d[aux.data - 1] + v.weight);
                pai[v.data - 1] = aux.data;
            }
            else if(H.pertence(v)){
                if(H.atualizarSeMenor(v.data, dist.d[aux.data - 1] + v.weight))
                    pai[v.data - 1] = aux.data;
            }
        }
    }
//formatando a saida do jeito sair
    for(int i = 0; i < n; i++){
        printf("%d ", i + 1);
        if(dist.Inf[i])
            printf("INFINITO\n");
        else
            printf("%.3f\n", dist.d[i]);
    }
}
