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
vector<int> *V;
int *degrees;
int *sorted_idxs;
int *upper_bounds;


void add_edge(vector<int> adj[], int u, int v);
void print_graph(vector<int> adj[], int N);
void print_sorted_degrees(vector<pair<int,int> > a);
void backtrack(int *agent_types, int *degrees_sums, int current_score, int a_count, int b_count, int recursion_lvl);
int get_score(int *agent_types, bool *visited, int current_node, int type);
void sort_vertices(vector<int> *V, int* degrees);
int get_initial_score();

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

int get_score(int *agent_types, bool *visited, int current_node, int type) {
    int new_count = 0;
    int dif = 0;
    
    /* getting new rewards */
    for (int nidx : V[current_node]) {
        if(!visited[nidx]) {
            if(type == 2) 
                new_count++;
        } else {
            if(type == 1)
                new_count++;
            if(agent_types[nidx] == 1)
                dif++;
            else if(agent_types[nidx] == 2)
                dif--;
        }
    }

    return new_count + dif;
}

int count_score(int v, int degree, int type) {
    if(type == 1) 
        return 2 * degree; 
    return degree;
}

bool randomBool() {
    return 0 + (rand() % (1 - 0 + 1)) == 1;
}

int get_initial_score() {
    int a=0, b=0, score=0;
    int *types = new int[30];
    bool *visit = new bool[30];
    for(int i=0; i<N; i++) {
        types[i] = 0;
        visit[i] = false;
    }

    for(int i=0; i<N; i++) {
        int idx = sorted_idxs[i];
        visit[idx] = true;
        if(a != A) {
            types[idx] = 1;
            score = score + get_score(types, visit, idx, 1);
            a++;
        } else if(b != B) {
            types[idx] = 2;
            score = score + get_score(types, visit, idx, 2);
            b++;
        }
    } 
    free(types);
    free(visit);
    return score;
}

int get_upper_bound(int a_count, int b_count, int *agent_types, vector<pair<int,int> > sorted) {
    int sum = 0;
    for(pair<int, int> vertex : sorted) {
        int degree = vertex.first;
        if(agent_types[vertex.second] > 0) {
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

void backtrack(int *agent_types, int *degrees_sums, int current_score, int a_count, int b_count, int recursion_lvl) {
    /* STOP CONDITION */
    if(a_count == A && b_count == B) {
        if(current_score > best_score) {
            best_score = current_score;
        }
        return;
    }

    /* PRUNING */
    if (best_score > current_score + 2*(degrees_sums[recursion_lvl] - degrees_sums[recursion_lvl + (A-a_count)]) + (degrees_sums[recursion_lvl + (A-a_count)] - degrees_sums[recursion_lvl + (A-a_count) + (B-b_count)])) {
        return;
    }
      
    /* BACKTRACKING */
    int indx = sorted_idxs[recursion_lvl];
    if(a_count != A) {
        agent_types[indx] = 1;
        int new_score = 0;
        for (int nidx : V[indx])
            if (agent_types[nidx] == 1)
                new_score += 2;
        backtrack(agent_types, degrees_sums, current_score + new_score, a_count+1, b_count, recursion_lvl+1);
        agent_types[indx] = 0;
    }
    if(b_count != B) {
        agent_types[indx] = 2;
        int new_score = 0;;
        for (int nidx : V[indx])
            if (agent_types[nidx] == 2)
                new_score--;
            else
                new_score++;
        backtrack(agent_types, degrees_sums, current_score + new_score, a_count, b_count+1, recursion_lvl+1);
        agent_types[indx] = 0;
    }
    if(N - recursion_lvl - 1 >= A - a_count + B - b_count) {
        backtrack(agent_types, degrees_sums, current_score, a_count, b_count, recursion_lvl+1);
    }

    return;
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
    V = new vector<int>[N];
    int *agent_types = new int[30];
    bool *visited = new bool [30];
    upper_bounds = new int[30];
    int src, dist;
    degrees = new int[30];
    auto start = high_resolution_clock::now(); 

    for(int i=0; i<N; i++) {
        degrees[i] = 0;
        agent_types[i] = 0;
        upper_bounds[i] = 0;
    }
    for(int e=0; e<M; e++) {
        scanf("%d %d\n", &src, &dist);
        add_edge(V, src-1, dist-1); 
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
    sorted_idxs = new int[N];
    for(int i=0; i<N; i++) {
        sorted_idxs[i] = increased.at(i).second;
    }


    //print_sorted_degrees(a);
    //print_graph(V, N);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    for(int i=0; i<N; i++)
        visited[i] = false;
    //print_graph(V, N);
    int initial_score = get_initial_score();
    //int initial_score = 0;

    start = high_resolution_clock::now(); 
    best_score = initial_score;

    int *degrees_sums = new int[N+1];
    for (int i = N - 1; i >= 0; i--)
        degrees_sums[i] = degrees_sums[i+1] + degrees[i];

    backtrack(agent_types, degrees_sums, 0, 0, 0, 0);


    
    stop = high_resolution_clock::now(); 
    duration = duration_cast<milliseconds>(stop - start); 
    cout << "Program ended in " << duration.count() << "ms" << endl; 
    /*free(V);
    free(agent_types);
    free(visited);
    free(degrees);
    free(increased_idxs);*/

    printf("%d\n", best_score);
    return 0; 
} 