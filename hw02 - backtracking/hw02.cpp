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
		return l > r;
	}
};

int N, M, A, B, best_score;

void add_edge(vector<int> adj[], int u, int v);
void print_graph(vector<int> adj[], int N);
void print_sorted_degrees(vector<pair<int,int> > a);
void backtrack(int current_score, int a_count, int b_count, bool *visited, vector<pair<int,int> > sorted, vector<int> *V, int *agent_types, int recursion_lvl);
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
    vector<int> neighbours = V[current_node];
    for(int nidx : neighbours) { 
        int ntype = agent_types[nidx];
        if(!visited[nidx]) {
            new_count = new_count + reward_b[type];
        } else {
            new_count = new_count + reward_a[type];
            dif = dif + diffs[ntype];
        }
    }

    return new_count + dif;
}

int get_upper_bound(int a_count, int b_count, bool *visited, vector<pair<int,int> > sorted) {
    int sum = 0;
    for(pair<int, int> vertex : sorted) {
        int degree = vertex.first;
        if(!visited[vertex.second]) {
            if(a_count >= A) {
                if(b_count >= B) {
                    return sum;
                } else {
                    b_count++;
                    sum = sum + degree;
                }
            } else {
                a_count++;
                sum = sum + 2 * degree;
            }
        }
    }
    return sum;
}

void backtrack(int current_score, int a_count, int b_count, bool *visited, vector<pair<int,int> > sorted, vector<int> *V, int *agent_types, int recursion_lvl) {
    /* STOP CONDITION */
    if(a_count >= A && b_count >= B) {
        //printf(" --> STOP.");
        if(current_score > best_score) {
            best_score = current_score;
        }
        return;
    }

    /* PRUNING */
    int bound = get_upper_bound(a_count, b_count, visited, sorted);
    if(current_score + bound <= best_score) {
        return;
    }

    // assign agent types to nodes or not
    for(pair<int, int> vertex : sorted) {
        int current_node = vertex.second;
        if(!visited[current_node]) {
            for(int type=1; type<3; type++) {
                agent_types[current_node] = type;
                visited[current_node] = true;
                int score = get_score(current_node, visited, V, type, agent_types);  //TODO: lze udelat optimalneji
                //cout << "\n" << std::string(2*recursion_lvl, ' ')  << "#" << current_node+1 << "(T" << type << "): " << current_score << " -> "<< score << "+" << current_score << " -> " << current_score + score;
                if(type == 1 && a_count < A)
                    backtrack(current_score + score, a_count+1, b_count, visited, sorted, V, agent_types, recursion_lvl+1);
                else if(type == 2 && b_count < B)
                    backtrack(current_score + score, a_count, b_count+1, visited, sorted, V, agent_types, recursion_lvl+1);
                visited[current_node] = false;
            }
            agent_types[current_node] = 0;
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
    vector<int> adj[N];
    int agent_types[N];
    bool visited[N];
    int src, dist;
    int degrees[N];
    auto start = high_resolution_clock::now(); 

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
    vector<pair<int,int> > a;
    for (int i=0; i<N; i++) {
        a.push_back(make_pair (degrees[i], i)); // k = value, i = original index
    }
    
    sort(a.begin(), a.end(), comp());
    //print_sorted_degrees(a);
    //print_graph(adj, N);
    sort_vertices(adj, degrees);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    cout << "Vertices sorted in " << duration.count() << "ms" << endl; 
    for(int i=0; i<N; i++)
        visited[i] = false;
    //print_graph(adj, N);
    //print_graph(V, N);
    start = high_resolution_clock::now(); 
    best_score = 0;
    backtrack(0, 0, 0, visited, a, adj, agent_types, 0);
    
    stop = high_resolution_clock::now(); 
    duration = duration_cast<milliseconds>(stop - start); 
    cout << "Program ended in " << duration.count() << "ms" << endl; 


    printf("%d\n", best_score);
    return 0; 
} 