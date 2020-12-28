#include <iostream>
#include <vector>
using namespace std;

vector<int> *adj;
int *types;
int correct;

void add_edge(vector<int> adj[], int u, int v) {
    adj[u].push_back(v); 
    adj[v].push_back(u); 
} 

void print_graph(int N) { 
    for (int v = 0; v<N; v++) { 
        if(types[v] == 1) {
            cout << "\n Adjacency list of extractor " << v+1 << "\n head "; 
        } else if(types[v] == 2) {
            cout << "\n Adjacency list of validator " << v+1 << "\n head "; 
        } else 
            cout << "\n Adjacency list of router " << v+1 << "\n head "; 
        for (int i=0; i<(int )adj[v].size(); i++) { 
            int x = adj[v][i];
            cout << "-> " << x+1; 
        }
        printf("\n"); 
    } 
} 

bool isStation(int current_node) {
    if(adj[current_node].size() == 1)
        return true;
    return false;
}

pair<bool, bool> getType(int current_node) {
    pair<bool, bool> ret (false, false);
    if(types[current_node] == 1) {
        ret.first = true;
        return ret;
    } else if(types[current_node] == 2) {
        ret.second = true;
        return ret;
    }
    return ret;
}

bool isCorrect(pair<bool, bool> ret) {
    if(ret.first && ret.second)
        return true;
    return false;
}

pair<bool, bool> logAdd(pair<bool, bool> p1, pair<bool, bool> p2) {
    return pair<bool, bool>  (p1.first || p2.first, p1.second || p2.second);
}

pair<bool, bool> search(bool *visited, int current_node) {
    visited[current_node] = true;
    if(isStation(current_node)) {
        return getType(current_node);
    } else {
        pair<bool, bool > stations (false, false);
        for(int neighbour : adj[current_node]) {
            if(!visited[neighbour]) {
                pair<bool, bool> ret = search(visited, neighbour);
                stations = logAdd(stations, ret);
            }
        }
        if(isCorrect(stations)) {
            correct++;
            stations.first = false;
            stations.second = false;
        }  
        return stations;
    }
}

int get_root(int N) {
    for(int i=0; i<N ;i++) {
        if((int) adj[i].size() == 2)
            return i;
    }
}

int main(void) { 
    int P, S, E, V; 
    scanf("%d %d %d %d\n", &P, &S, &E, &V);
    //printf("P= %d, S= %d, E= %d, V= %d\n-----------------------\n", P, S, E, V);
    bool *visited = new bool[P];
    int src, dist;
    adj = new vector<int>[P];
    types = new int[P];

    for(int i=0; i<P;i++) {
        visited[i] = false;
        types[i] = 0;
    }
    for(int e=0; e<S; e++) {
        scanf("%d %d\n", &src, &dist);
        add_edge(adj, src-1, dist-1); 
    }
    for(int e=0; e<E; e++) {
        int i;
        scanf("%d ", &i);
        types[i-1] = 1;
    }
    for(int v=0; v<V; v++) {
        int i;
        scanf("%d ", &i);
        types[i-1] = 2;
    }

    //print_graph(P);
    correct = 0;

    search(visited, get_root(P));
    cout << correct << "\n";


    return 0;
}
