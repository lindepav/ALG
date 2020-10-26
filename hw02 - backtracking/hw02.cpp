#include <iostream>
#include <vector>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::stable_sort
#include <chrono> 

using namespace std;
using namespace std::chrono; 

struct comp {
	template<typename T>
	bool operator()(const T& l, const T& r) const {
		return l < r;
	}
};

struct comp_decreased {
	template<typename T>
	bool operator()(const T& l, const T& r) const {
		return l > r;
	}
};

int N, M, A, B, best_score;

void add_edge(vector<int> adj[], int u, int v);
void print_graph(vector<int> adj[], int N);
void print_sorted_degrees(vector<pair<int,int> > a);
void backtrack(int current_score, int a_count, int b_count, bool *visited, int *sorted_vertices, int *degrees, vector<int> *V, int *agent_types);
int get_score(int current_node, bool *visited, vector<int> *V, int type, int *agent_types);
int get_upper_bound(int a_count, int b_count, bool *visited, vector<pair<int,int> > sorted);
void sort_vertices(vector<int> *V, int* degrees);

void add_edge(vector<int> adj[], int u, int v) {
    adj[u].push_back(v); 
    adj[v].push_back(u); 
} 

void print_graph(vector<int> adj[], int N) { 
    for (int v = 0; v<N; v++) { 
        cout << "\n Adjacency list of vertex " << v+1 << "\n head "; 
        for (int i=0; i<(int )adj[v].size(); i++) { 
            int x = adj[v][i];
            cout << "-> " << x+1; 
        }
        printf("\n"); 
    } 
} 

void print_sorted_degrees(vector<pair<int,int> > a) {
    for (int i=0; i<N; i++) {
        cout << a[i].first << " " << a[i].second << "\n";
    }
}

int get_score(int current_node, bool *visited, vector<int> *V, int type, int *agent_types) {
    int reward_a[3] = {0, 1, 0}; 
    int reward_b[3] = {0, 0, 1}; 
    int diffs[3] = {0, 1, -1};
    int new_count = 0;
    int dif = 0;
    
    /* getting new rewards */
    for (int nidx : V[current_node]) {
        if(!visited[nidx]) {
            new_count = new_count + reward_b[type];
        } else {
            new_count = new_count + reward_a[type];
            dif = dif + diffs[agent_types[nidx]];
        }
    }

    return new_count + dif;
}

int get_upper_bound(int a_count, int b_count, bool *visited, int *sorted, int *degrees) {
    int sum = 0;
    for(int i=0; i<N; i++) {
        if(!visited[sorted[i]]) {
            if(a_count >= A) {
                if(b_count >= B) {
                    return sum;
                } else {
                    b_count++;
                    sum = sum + degrees[sorted[i]];
                }
            } else {
                a_count++;
                sum = sum + 2 * degrees[sorted[i]];
            }
        }
    }
    return sum;
}

void backtrack(int current_score, int a_count, int b_count, bool *visited, int *sorted_vertices, int *degrees, vector<int> *V, int *agent_types) {
    if(current_score > best_score) {
        best_score = current_score;
        cout << current_score << "\n";
    }
    /* STOP CONDITION */
    if(a_count >= A && b_count >= B) {
        return;
    }

    /* PRUNING */
    int bound = get_upper_bound(a_count, b_count, visited, sorted_vertices, degrees);
    if(current_score + bound <= best_score) {
        return;
    }

    // assign agent types to nodes or not
    for(int i=0; i<N; i++) {
        if(!visited[sorted_vertices[i]]) {
            agent_types[sorted_vertices[i]] = 1;
            visited[sorted_vertices[i]] = true;
            int score = get_score(sorted_vertices[i], visited, V, 1, agent_types);  //TODO: lze udelat optimalneji
            //cout << "\n" << std::string(2*recursion_lvl, ' ')  << "#" << current_node+1 << "(T" << type << "): " << current_score << " -> "<< score << "+" << current_score << " -> " << current_score + score;
            if(a_count < A) {
                backtrack(current_score + score, a_count+1, b_count, visited, sorted_vertices, degrees, V, agent_types);
            } 
            visited[sorted_vertices[i]] = false;
            agent_types[sorted_vertices[i]] = 0;
        }
    } 
    for(int i=0; i<N; i++) {
        if(!visited[sorted_vertices[i]]) {
            agent_types[sorted_vertices[i]] = 2;
            visited[sorted_vertices[i]] = true;
            int score = get_score(sorted_vertices[i], visited, V, 2, agent_types);  //TODO: lze udelat optimalneji
            //cout << "\n" << std::string(2*recursion_lvl, ' ')  << "#" << current_node+1 << "(T" << type << "): " << current_score << " -> "<< score << "+" << current_score << " -> " << current_score + score;
            if(b_count < B) {
                backtrack(current_score + score, a_count, b_count+1, visited, sorted_vertices, degrees, V, agent_types);
            }
            visited[sorted_vertices[i]] = false;
            agent_types[sorted_vertices[i]] = 0;
        }
    }   
}

void sort_vertices(vector<int> *V, int* degrees) {
    for(int i=0; i<N; i++) {
        vector<pair<int,int> > a;
        int s = (int)V[i].size();
        for (int j=0; j<s; j++) {
            int k = V[i][j];
            a.push_back(make_pair (degrees[k], k)); // k = value, j = original index
        }
    
        sort(a.begin(), a.end(), comp());
        for(int k=0; k<s; k++) {
            V[i].at(k) = a[k].second;
        }
    }
}

int main() { 
    scanf("%d %d %d %d\n", &N, &M, &A, &B);
    //printf("N= %d, M= %d, A= %d, B= %d\n-----------------------\n", N, M, A, B);
    vector<int> *adj = new vector<int>[N];
    int *agent_types = new int[30];
    bool *visited = new bool [30];
    int src, dist;
    int *degrees = new int[30];
   // auto start = high_resolution_clock::now(); 

    for(int i=0; i<N; i++) {
        degrees[i] = 0;
        agent_types[i] = 0;
    }
    for(int e=0; e<M; e++) {
        scanf("%d %d\n", &src, &dist);
        add_edge(adj, src-1, dist-1); 
        degrees[src-1]++;
        degrees[dist-1]++;
    }
    vector<pair<int,int> > increased;
    vector<pair<int,int> > decreased;
    vector<pair<int,int> > original;
    for (int i=0; i<N; i++) {
        increased.push_back(make_pair (degrees[i], i)); // k = value, i = original index
        decreased.push_back(make_pair (degrees[i], i)); // k = value, i = original index
        original.push_back(make_pair (degrees[i], i)); // k = value, i = original index
    }
    
    sort(decreased.begin(), decreased.end(), comp_decreased());
    sort(increased.begin(), increased.end(), comp());
    int *increased_idxs = new int[N];
    for(int i=0; i<N; i++) {
        increased_idxs[i] = increased.at(i).second;
    }
    //print_sorted_degrees(a);
    //print_graph(adj, N);
    //sort_vertices(adj, degrees);
    /*auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    cout << "Vertices sorted in " << duration.count() << "ms" << endl; */
    for(int i=0; i<N; i++)
        visited[i] = false;
    //print_graph(adj, N);
    //print_graph(V, N);
    //start = high_resolution_clock::now(); 
    best_score = 0;
    backtrack(0, 0, 0, visited, increased_idxs, degrees, adj, agent_types);
    
    /*stop = high_resolution_clock::now(); 
    duration = duration_cast<milliseconds>(stop - start); 
    cout << "Program ended in " << duration.count() << "ms" << endl; */


    printf("%d\n", best_score);
    return 0; 
} 