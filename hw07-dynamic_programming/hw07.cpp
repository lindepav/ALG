#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <limits.h>

using namespace std;

typedef struct {
    int to;
    int cost;
} Edge;

class Graph 
{ 
    int M;  // # of vertices
    int N; // # of meridians
    int *meridians;   // array of zero meridians
    int *in_degrees;
    vector<Edge> *adj;
    std::pair <int, vector<int> >* paths;    
public: 
    Graph(int M, int N, int *meridians);  // Constructor 
    void addEdge(int v, int w, int cost);
    void deleteGraph();
    void printGraph();
    bool isMeridian(int v);
    int getMeridianIndex(int v);
    vector<int> topologicalOrder(int from);
    void computeDegrees();
    void printDegrees();
    void printTopological(vector<int> topological);
    std::pair <int, vector<int> > findShortestPath(int from);
    int* removeNode(int x, int *degrees);
    void findPaths();
    void printPaths();
    int aroundTheWorld(int i, int P);
    int getInitialMeridian();
    std::pair <int, vector<int> > getShortestPath(int i);
}; 

Graph::Graph(int M, int N, int *meridians) 
{ 
    this->M = M; 
    this->N = N;
    this->meridians = meridians;
    this->in_degrees = new int[M];
    this->adj = new vector<Edge>[M]; 
    this->paths = new std::pair <int, vector<int> >[N];
} 
void Graph::addEdge(int v, int w, int cost) 
{ 
    Edge e;
    e.to = w;
    e.cost = cost;
    adj[v].push_back(e); 
} 
void Graph::deleteGraph()
{
    delete [] adj;
    delete [] in_degrees;
    delete [] paths;
}
void Graph::printGraph() 
{
    for(int i=0; i<M; i++) {
        auto edges = adj[i];
        if(isMeridian(i)) 
            cout << "-> ";
        else if(i >= M-N)
            cout << "<- ";
        else
            cout << "   ";
        cout << i+1 << ": ";
        for(auto edge : edges) {
            cout << edge.to+1 << "[" << edge.cost << "]" << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int* Graph::removeNode(int x, int *degrees) 
{
    for(auto edge : adj[x]) {
        degrees[edge.to]--;
        if(degrees[edge.to] == 0)   
            degrees = removeNode(edge.to, degrees);
    }
    return degrees;
}

vector<int> Graph::topologicalOrder(int from) 
{
    vector<int> topological;
    queue<int> Q;
    int *local_in_degrees = new int[M];
    for(int d=0; d<M; d++) 
        local_in_degrees[d] = in_degrees[d];
    for(int m=0; m<N; m++) {
        int mer_idx = meridians[m];
        if(mer_idx != from) {
            for(auto edge : adj[mer_idx]) {
                local_in_degrees[edge.to]--; 
                if(local_in_degrees[edge.to] == 0) 
                    local_in_degrees = removeNode(edge.to, local_in_degrees);
            }
        }
    }

    topological.push_back(from);
    Q.push(from);
    while(Q.size() > 0) {
        int v = Q.front();
        Q.pop();
        for(auto edge : adj[v]) {
            if(local_in_degrees[edge.to] > 0) 
                local_in_degrees[edge.to]--;
            if(local_in_degrees[edge.to] == 0) {
                Q.push(edge.to);
                topological.push_back(edge.to);
            } 
        }

    }
    delete [] local_in_degrees;
    return topological;
}

void Graph::printTopological(vector<int> topological)
{
    cout << "Topological order: ";
    for (std::vector<int>::const_iterator i = topological.begin(); i != topological.end(); ++i) {
        cout << (*i)+1 << ", ";
    }
    cout << endl;
}
void Graph::computeDegrees()
{
    for(int v=0; v<M; v++) 
        in_degrees[v] = 0;
    for(int v=0; v<M; v++) {
        for(auto edge : adj[v]) 
            in_degrees[edge.to]++;
    }
}

void Graph::printDegrees()
{
    cout << "In-degrees of nodes: " << endl;
    for(int d=0; d<M; d++) {
        cout << d+1 << ": " << in_degrees[d] << endl;
    }
    cout << endl << "---------------------------------" << endl;
}

bool Graph::isMeridian(int v) 
{
    for(int i=0; i<N; i++) {
        if(meridians[i] == v)
            return true;
    }
    return false;
}

int Graph::getMeridianIndex(int v)
{
    for(int i=0; i<N; i++) {
    if(meridians[i] == v)
        return i;
    }
    return -1;
}

std::pair <int, vector<int> > Graph::findShortestPath(int from)
{
    std::pair <int, vector<int> > ret;
    int *d = new int[M];

    for(int i=0; i<M; i++) {
        d[i] = INT_MAX;
    } 

    vector<int> topological = topologicalOrder(from);

    for(int i=0; i<topological.size(); i++) {
        int x = topological[i];
        // TODO: tohle muze byt blbost
        if(d[x] == INT_MAX) 
            d[x] = 0;
        for(auto edge : adj[x]) {
            if(d[x] + edge.cost < d[edge.to]) {
                d[edge.to] = d[x] + edge.cost;
            }
        }
    }
    int min_cost = INT_MAX;
    vector<int> best_target_meridians;
    for(int m=0; m<N; m++) {
        if(d[M-N+m] < min_cost) {
            min_cost = d[M-N+m];
            best_target_meridians.clear();
            best_target_meridians.push_back(m);
        } else if(d[M-N+m] == min_cost) {
            best_target_meridians.push_back(m);
        }
    }
    ret.first = min_cost;
    ret.second = best_target_meridians;
    delete [] d;
    return ret;
}

void Graph::findPaths() 
{
    for(int m=0; m<N; m++) {
        paths[m] = findShortestPath(meridians[m]);
    }

}

void Graph::printPaths() 
{
    for(int p=0; p<N; p++) {
        auto path = paths[p];
        cout << p+1 << " -> " << "cost: " << path.first << ", targets: "; 
        for (std::vector<int>::const_iterator i = path.second.begin(); i != path.second.end(); ++i)
            cout << (*i)+1 << " ";
        cout << endl;
    }
}

std::pair <int, vector<int> > Graph::getShortestPath(int i) 
{
    return paths[i];
}

int Graph::getInitialMeridian() 
{
    int best_cost = INT_MAX;
    int best_meridian = -1;
    for(int i=0; i<N; i++) {
        auto meridian_best_path = paths[i];
        if(meridian_best_path.first < best_cost) {
            best_cost = meridian_best_path.first;
            best_meridian = i;
        }
    }
    return best_meridian;
}

int Graph::aroundTheWorld(int i, int P) 
{
    if(P == 0)
        return 0;
    auto current_path = paths[i];

    int best_path_cost = INT_MAX;
    int best_meridian = -1;
    for (std::vector<int>::const_iterator i = current_path.second.begin(); i != current_path.second.end(); ++i) {
        int path_cost = paths[(*i)].first;
        if(path_cost < best_path_cost) {
            best_path_cost = path_cost;
            best_meridian = (*i);
        }
    }

    return current_path.first + aroundTheWorld(best_meridian, P-1);
}

int main(void) 
{
    int M, T, N, P;
    scanf("%d %d %d %d\n", &M, &T, &N, &P);

    int *zero_meridians = new int[N];
    for(int i=0; i<N; i++) {
        int x;
        scanf("%d ", &x);
        zero_meridians[i] = x-1;
    }
    
    Graph g(M+N, N, zero_meridians);
    for(int e=0; e<T; e++) {
        int from, to, cost;
        scanf("%d %d %d\n", &from, &to, &cost);
        int mer_idx = g.getMeridianIndex(to-1);
        if(mer_idx >= 0) {
            g.addEdge(from-1, mer_idx+M, cost);
        } else
            g.addEdge(from-1, to-1, cost);
    } 
    //g.printGraph();

    g.computeDegrees();
    //g.printDegrees();
    g.findPaths();
    //g.printPaths();
    int best_cost = INT_MAX;
    for (int i=0; i<N; i++) {
        int meridian_cost = g.aroundTheWorld(i, P);
        if(meridian_cost < best_cost) 
            best_cost = meridian_cost;
    }
    cout << best_cost << endl;

    g.deleteGraph();
    delete [] zero_meridians;
    return 0;
}

