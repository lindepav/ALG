#include <iostream>
#include <vector>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::stable_sort
#include <chrono> 
#include <set>

using namespace std;
using namespace std::chrono; 

struct Edge {
    int to;
    int cost;
};

struct Vertex {
    char type;
    vector<Edge> adj;
}; 

int best_score, type_count;

class Graph 
{
    int N;
    int L;
    int cheapest_edge;
    Vertex *V;
    set<int> visited;

public:
    Graph(int N, int L);
    void addEdge(int v, int w, int cost);
    void setCheapest(int cost);
    void deleteGraph();
    void printGraph();
    void computeDegrees();
    void printDegrees();
    void addType(char type, int v);
    void findTour(vector<int> path, set<int> visited, int v, int score, int recursion_lvl);
    bool isSafe(vector<int> path, int v, char type);

};

void Graph::setCheapest(int cost)
{
    this->cheapest_edge = cost;
}

Graph::Graph(int N, int L) 
{ 
    this->N = N; 
    this->L = L;
    this->V = new Vertex[N]; 
} 
void Graph::addType(char type, int v)
{   
    this->V[v].type = type;
}
void Graph::addEdge(int v, int w, int cost) 
{ 
    Edge e;
    e.to = w;
    e.cost = cost;
    V[v].adj.push_back(e); 
} 
void Graph::deleteGraph()
{
    delete [] V;
}
void Graph::printGraph() 
{
    for(int i=0; i<N; i++) {
        auto edges = V[i].adj;
        cout << i << "(" << V[i].type << "): ";
        for(auto edge : edges) {
            cout << edge.to << "[" << edge.cost << "]" << " ";
        }
        cout << endl;
    }
    cout << endl;
}
bool Graph::isSafe(vector<int> path, int v, char type)
{
    for(auto w : path) {
        if(w == v || type == V[w].type) 
            return false;
   } 
   return true;
}
void Graph::findTour(vector<int> path, set<int> visited, int v, int score, int recursion_lvl)
{
    if(path.size() == type_count) {
        for(auto edge : V[path.at(type_count-1)].adj) {
            if(edge.to == path.at(0)) {
                if(score + edge.cost < best_score) {
                    best_score = score + edge.cost;
                }
                return;
            }
        }
        return;
    }
    // PRUNING
    if(score + (type_count + 1 - path.size()) * cheapest_edge >= best_score)
        return;

    for(auto neighbour : V[v].adj) {
        if(!visited.count(neighbour.to)) {
            int new_score = score + neighbour.cost;
            if(new_score <= L && isSafe(path, neighbour.to, V[neighbour.to].type)) {
                path.push_back(neighbour.to);
                visited.insert(neighbour.to);
                findTour(path, visited, neighbour.to, new_score, recursion_lvl);
                path.pop_back();
                visited.erase(neighbour.to);
            }
        }
    }
}

int getTypeIdx(char type)
{
    char types[] = {'A','B','C','D','E','F','G','H','I','J'};
    for(int i=0; i<10; i++) {
        if(types[i] == type) 
            return i;
    }
}

int main(void) 
{
    int N, L;
    char types[] = {'A','B','C','D','E','F','G','H','I','J'};
    scanf("%d %d\n", &N, &L);
    Graph g(N, L);
    type_count = 0;
    int cheapest = INT_MAX;
    for(int i=0; i<N; i++) {
        int v, n;
        char type;
        scanf("%d %c %d", &v, &type, &n);
        int t = getTypeIdx(type);;
        type_count = (t > type_count) ? t : type_count;
        g.addType(type, v);
        for(int e=0; e<n; e++) {
            int w, cost;
            scanf("%d %d", &w, &cost);
            g.addEdge(v, w, cost);
            if(cost < cheapest)
                cheapest = cost;
        }
    }
    g.setCheapest(cheapest);
    set<int> visited;
    type_count++;
    best_score = INT_MAX;
    auto start = high_resolution_clock::now(); 
    for(int i=0; i<N; i++) {
        vector<int> path;
        path.push_back(i);
        visited.insert(i);
        g.findTour(path, visited, i, 0, 1);
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    cout << "Program ended in " << duration.count() << "ms" << endl; 
    cout << best_score << endl;
    g.deleteGraph();
    return 0;
}
