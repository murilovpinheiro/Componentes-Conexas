#include <iostream>
#include <cstdio>
#include <new>
using std::scanf;using std::cout; using std::nothrow; 

struct node{
    int data;
    node *next;
    //noh do tipo node com um inteiro e um ponteiro para o proximo noh da lista de adjacencia
    
    void dataInput(int d){ //input data
        data = d;
        next = NULL;
    }
};

struct grafo{
    node *list; //lista de adjacencia

    grafo(int n){ //criando grafo com vetor de tamanho do número de vértices
        node *aux = new(nothrow) node[n];
        list = aux;
    }
    void inicializares(int n){ //inicializando lista de adjacencia com [-1, NULL], detalhe que -1 pode significar valor não printado/visitado
        for(int i = 0; i < n; i++){
            list[i].dataInput(-1);
        }
        //pretendo usar o -1 como valor não printado/visitado, 0 como valor visitado mas não printado e 1 como valor printado
    }
    void add(int a, int b){
        //adiciona o vértice b a lista de adjacencia do vértice a
        node *noh = new(nothrow) node;
        noh = &list[a];
        while(noh->next != NULL){
            noh = noh->next;
        }
        //detalhe que o primeiro nó da lista de adjacencia não tem Data pois ele não é um vértice em si
        //ele é um ponteiro pra o proximo nó da lista de adjacencia, ele armazena -1 ou 0 ou 1 como dito acima
        noh->next = new(nothrow) node;
        //criando noh novo e adicionando ao final da lista de adjacencia do vértice a
        noh->next->dataInput(b);
    }
    void print(int n){
        //imprime a lista de adjacencia do grafo com n vértices
        for(int i = 0; i < n; i++){
            node *aux = list[i].next;
            while(aux != NULL){
                cout << i << " " << aux->data << "\n";
                aux = aux->next;
            }
        }
    }
    void desalocares(int n){
        //desaloca a lista de adjacencia do grafo com n vértices
        for(int i = 0; i < n; i++){
            node *aux = list[i].next;
            while(aux != NULL){
                node *aux2 = aux;
                aux = aux->next;
                delete aux2;
            }
        }
    }
};

struct pilha{
    int *p;
    int topo;
    int max;
    int tam;
    //pilha de inteiros com topo, tamanho máximo e tamanho atual
    pilha(int n){
        p = new(nothrow) int[n];
        max = n;
        tam = 0;
        topo = -1;
    }
    void push(int x){
        if(tam == max){
            cout << "pilha cheia\n";
            return;
        }
        p[++topo] = x;
        tam++;
    }
    int pop(){
        if(tam == 0){
            cout << "pilha vazia\n";
            return -1;
        }
        tam--;
        return p[topo--];
    }
    int top(){
        if(tam == 0){
            cout << "pilha vazia\n";
            return -1;
        }
        return p[topo];
    }
    bool empty(){
        if(tam == 0){
            return true;
        }
        return false;
    }
    void desalocares(){
        delete[] p;
    }
};

void print(grafo *g, int tamanho, int contador){
    for(int i = 0; i < tamanho; i++){
        if(g->list[i].data == 0){
            contador--;
            cout << i + 1;
            g->list[i].data = 1;
            if(contador != 0){
                cout << ' ';
            }
            else{
                cout << '\n';
            } 
        }
    }
}
//falta ajeitar pra 1 e não para 0 e checar o que falta
//aparentemente funcionando dps testar tudo
void buscaPorLargura(grafo *g, int tamanho){
    pilha atual = pilha(tamanho);
    pilha prox = pilha(tamanho);
    for(int i = 0 ; i < tamanho; i++){
        int contador = 0;
        if(g->list[i].data != -1){
            continue;
        }
        atual.push(i); prox = pilha(tamanho);
        while(!atual.empty()){
            int indice = atual.pop();
            g->list[indice].data = 0;
            contador++;
            node *aux = g->list[indice].next;
            while(aux != NULL){
                if(g->list[aux->data].data == -1){
                    g->list[aux->data].data = 0;
                    prox.push(aux->data);
                }
                aux = aux->next;   
            }
            if(atual.empty()){
                atual = prox;
                prox = pilha(tamanho);
            }
        }
        print(g, tamanho, contador);
    }
        atual.desalocares();
        prox.desalocares();
}


int main(){
    int edges; // N° de vértices
    scanf("dl\nformat=edgelist1\nn=%d\ndata:\n", &edges); //entrada padrão
    grafo *g = new grafo(edges); // criar grafo que vai armazenar tudo
    g->inicializares(edges); //inicializando tudo como [-1, null]

    int edge1, edge2; //vértices que vamos receber
    while(scanf("\n%d %d", &edge1, &edge2 ) == 2){
        g->add(edge1 - 1, edge2 - 1); //adicionando vértices a lista de adjacencia
        g->add(edge2 - 1, edge1 - 1); //adicionando vértices a lista de adjacencia
    }
    buscaPorLargura(g, edges);
    g->desalocares(edges);
    delete g;
}