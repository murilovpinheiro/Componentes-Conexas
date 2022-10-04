#include <bits/stdc++.h>
#define INFINITO 99999999.99
using namespace std;
//CÓDIGO PARA FLUXO MÁXIMO - FORGER FULKERSON UTILIZANDO BUSCA EM LARGURA (BFS) PARA ENCONTRAR CAMINHOS AUMENTANTES
//UTILIZO CLASSES EM VEZ DE STRUCTS

class Edge //Classe Aresta
{
    //Uma Aresta A -> B, de capacidade C
    //dest é B, source é C e capacity é C
  public:
    int source;
    int dest;
    float capacity;
    Edge()
    {
        //construtor para criação de Edge vazia
        Edge(0, 0, 0);
    }

    Edge(int src, int des, float cp)
    {
        //construtor para edges com valores
        source = src;
        dest = des;
        capacity = cp;
    }
};

class Residual_Edge : public Edge
{
    //Aresta do Grafo Residual, subclasse de uma aresta, mas com informações a mais
    //sendo elas a identificação se é um aresta Forward, caso não seja é uma Backward do Grafo Residual
    //um ponteiro para a contraparte dele, ou seja a Backward da mesma
  public:
    bool is_forward;
    Residual_Edge *counterpart;
    Residual_Edge() : Edge()
    {
        //construtor para criação de Residual_Edge sem dados
        is_forward = false;
        counterpart = NULL;
    }
    Residual_Edge(int src, int des, float cp, bool fwd) : Edge(src, des, cp)
    {
        //construtor para criação de Residual_Edge com dados
        //contraparte fica nulo pois dps add ela
        is_forward = fwd;
        counterpart = NULL;
    }

    void set_couterpart(Residual_Edge *n)
    {
        //setando contraparte
        counterpart = n;
    }
};

class Graph //classe Grafo em si
{
  private:
    vector<Edge *> *adjacency_list;
    //utilizando um Vetor de Vetor para Edges, ou seja estamos utilizando uma Lista de Adjacências
    //[][]
    //[][][] basicamente cada posição tem um vector, pelo o que eu entendi
    //[][][]

  public:
    //v é o número de vertices do Grafo
    int v;
    Graph(int vertices)
    {
        //criando a base do grafo, ou seja setando o número de vertices e o tamanho da lista de adjacencia
        v = vertices;
        adjacency_list = new vector<Edge *>[v];
    }

    ~Graph()
    {
        //desconstrutor do grafo, apaga a lista de adj e seus nós
        for (int i = 0; i < v; i++)
        {
            for (auto it : adjacency_list[i])
            {
                delete it;
            }
        }
        delete[] adjacency_list;
    }

    void addEdge(int v1, int v2, float capacity)
    {
        //método de adicionar arestas
        Edge *temp = new Edge(v1, v2, capacity); //criando um ponteiro para um edge elas
        adjacency_list[v1].push_back(temp); //coloca esse ponteiro na posição final

    }

    vector<Edge *> getNeighbours(int u)
    {
        //retorna a vizinhança de v
        return adjacency_list[u];
    }

    void printGraph() //sem segredo, printa o grafo
    {
        for (int i = 0; i < v; i++)
        {
            for (auto it : adjacency_list[i])
            {
                cout << "(" << i << "," << it->dest << "," << it->capacity << ")\n";
            }
        }
    }
};

class Residual_Graph
{
    //grafo residual, basicamente um grafo de arestas residuais
  private:
    vector<Residual_Edge *> *adjacency_list;

  public:
    int v;
    Residual_Graph(Graph &g)
    {
        //construtor que transforma um grafo em um grafo Residual
        adjacency_list = new vector<Residual_Edge *>[g.v];
        //criando lista de adjacencia de Arestas Residuais do tamanho do Nº de Vértices do Grafo
        v = g.v;
        for (int i = 0; i < v; i++)
        {
            //For de vértices
            for (auto it : g.getNeighbours(i))
            {
                //esse for(algo : algo2) vai basicamente pegar uma lista encadeada e rodar
                //onde algo vai ser o ponteiro pro atual até nulo
                //ou seja se temos 1->2->3->/ ele vai ser 1 na primeira, 2 na segunda, 3 na terceira e parar antes da 4 iteração

                Residual_Edge *N_fwd = new Residual_Edge(i, it->dest, it->capacity, true);
                Residual_Edge *N_rev = new Residual_Edge(it->dest, i, 0, false);
                //Arestas Residuais, ele cria a Forward a partir da Existente e a cria a Backword invertendo dest e source e coloca capacidade 0

                N_fwd->set_couterpart(N_rev);
                N_rev->set_couterpart(N_fwd);
                //seta uma como a contraparte da outra

                adjacency_list[i].push_back(N_fwd);
                //adiciona a Forward na lista encadeada da source dela
                adjacency_list[it->dest].push_back(N_rev);
                //adiciona a Backward na lista encadeada da source dela, no caso a dest da forward
            }
        }
    }

    ~Residual_Graph()
    {
        //desconstrutor do grafo residual, apaga tudo
        for (int i = 0; i < v; i++)
        {
            for (auto it : adjacency_list[i])
            {
                delete it;
            }
        }
        delete[] adjacency_list;
    }

    vector<Residual_Edge *> getNeighbours(int u)
    {
        //retorna vizinhança de u
        return adjacency_list[u];
    }

    void printGraph()
    {
        //printa o grafo residual
        for (int i = 0; i < v; i++)
        {
            for (auto it : adjacency_list[i])
            {
                cout << "(" << i << "," << it->dest << "," << it->capacity << "," << it->is_forward << ")\n";
            }
        }
    }

    void printFlow()
    {
        //printa o fluxo de cada aresta
        for (int i = 0; i < v; i++)
        {
            for (auto it : adjacency_list[i])
            {
                if (!it->is_forward)
                {
                    cout << "(" << it->dest << "," << i << "," << it->capacity << ")\n";
                }
            }
        }
    }
};

vector<Residual_Edge *> find_path(Residual_Graph &g, int source, int sink)
{
    //find_path a busca é feita com um BFS
    vector<Residual_Edge *> path;
    queue<int> q;
    //fila que vai armazenar os vizinhos
    bool visited[g.v];
    //bool que vai sinalizar quais vertices nos visitamos
    Residual_Edge *parent[g.v];
    //vetor de pais que vai nos ajudar a construir o path "voltando"
    for (int i = 0; i < g.v; i++)
    {
        visited[i] = false;
    }
    //setando todos os visitados pra falso e setando a source como visitada e adicionando ela na fila
    visited[source] = true;
    q.push(source);
    while ((!q.empty()) && !visited[sink])
    {
        for (auto out_edge : g.getNeighbours(q.front()))
        {
            //visitando todos os vizinhos do primeiro elemento da fila
            if (!visited[out_edge->dest] && out_edge->capacity > 0) //se o vizinho não tiver sido visitado e tiver capacidade maior que 0 (aresta existe)
            {
                //adicionamos ele na fila, colocamos o pai e falamos que já foi visitado
                q.push(out_edge->dest);
                parent[out_edge->dest] = out_edge;
                visited[out_edge->dest] = true;
            }
        }
        //tira o elemento Q da lista pois já fomos nos vizinhos dele
        //a partir daí repetimos o processo como no BFS
        q.pop();
    }
    if (!visited[sink]) //se o T não foi visitado é porque não conseguimos chegar nele, ou seja não há mais caminhos aumentantes e retornamos caminho vazio
        return path;
    int current = sink;
    while ((!current) == source)
    {
        //criamos o caminho a partir dos pais de cada vértice voltando de T
        //exemplo: pedimos o PAI de T, ele é F, depois pedimos pai de F, ele é G, depois pai de G, ele é S, sai do while
        //ao fim criamos o path: T, F, G, S
        path.push_back(parent[current]);
        current = parent[current]->source;
    }
    return path;
}

float max_flow(Graph &g, int source, int sink)
{
    //max_flow em si
    float mx_flow = 0;
    //iniciasse com 0
    Residual_Graph residual(g);
    //criamos o grafo residual de G
    auto curr_path = find_path(residual, source, sink);
    //procuramos 1 caminho aumentante
    while (!curr_path.empty()) //**aqui vai o porquê de mandarmos caminho vazio quando não encontramos
    {
        //enquanto caminho aumentante não for vazio
        float b = INFINITO;
        for (auto edge : curr_path)
        {
            b = min(b, edge->capacity);
            //procuramos o gargalo, menor capacidade do caminho
            //b = INT32_MAX no inicio para pegarmos a primeira capacidade sempre
            //na segunda rodada comparamos a primeira capacidade com a segunda e seguimos assim
        }
        mx_flow = mx_flow + b;
            //soma o valor de b ao fluxo máximo
        for (auto edge : curr_path)
        {
            //após isso vai atualizando as arestas residuais do caminho
            //caso ela seja forward diminuimos b
            //caso ela seja backward a gente aumenta b, pois é o valor que "devolvemos"
            edge->capacity -= b;
            edge->counterpart->capacity += b;
        }
        //procura outro caminho
        curr_path = find_path(residual, source, sink);
    }
    //retorna o max_flow
    //residual.printFlow();
    return mx_flow;
}

int main()
{
    int a, b, x, n;
    float c;
    scanf("dl\nformat=edgelist1\nn=%d\ndata:", &n);
    Graph g(n);

    while(1){

        x = scanf("%d %d %f", &a, &b, &c);

        if(x != 3)
            break;

        g.addEdge(a - 1, b - 1, c);
       
    }
    for(int i = 1; i < (n - 1); i++){
        cout << "s: 1,  t: "<< i + 1 <<",  flow: " << max_flow(g, 0, i) << "\n";
    }
    cout << "s: 1,  t: "<< n <<",  flow: " << max_flow(g, 0, n - 1);
    return 0;
    //funcionou pro exemplo 1 bem OK, depois continuar, receber entrada,
    //tacar no grafo e torcer pra dar bom
}