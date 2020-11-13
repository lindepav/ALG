#include<iostream> 
#include <vector> 
#include <list>
#include <chrono> 
#include <limits.h>
 
 
#define DEFAULT_DISTANCE 9223372036854775807
 
using namespace std; 
using namespace std::chrono; 
 
typedef struct {
    int index;
    int mark;
    int distance;
} Field;
 
class Graph 
{ 
    int R;  // # of rs
    int S;  // # of cumns
    int T;  // # of marks
    vector<int> *adj;    
    int *types; // types of field: 0 = no mark, i = # of mark
public: 
    Graph(int R, int S, int T) ;  // Constructor 
    void addEdge(int v, int w);  
    void addType(int type, int x, int y);
    void BFS(Field f);   
    void deleteGraph();
    int getLastMark(bool *field_visited, int mark);
}; 
 
Graph::Graph(int R, int S, int T) 
{ 
    this->R = R; 
    this->S = S; 
    this->T = T; 
    adj = new vector<int>[R * S]; 
    types = new int[R * S];
    for(int i=0; i<R*S; i++) {
        types[i] = 0;
    }
} 
 
void Graph::addType(int type, int r, int c)
{
    types[r * S + c] = type;
}
 
void Graph::addEdge(int v, int w) 
{ 
    adj[v].push_back(w); 
} 
void Graph::deleteGraph()
{
    delete [] adj;
    delete [] types;
}
 
int Graph::getLastMark(bool *field_visited, int mark) 
{
    int last = mark;
    for(int i=mark+1; i<T; i++) {
        if(field_visited[i])
            last = i;
    }
    return last;
}
 
void Graph::BFS(Field f) 
{ 
    // Mark all the vertices as not visited 
    bool **visited = new bool*[R * S]; 
    for(int i = 0; i < R*S; i++) {
        visited[i] = new bool[T];
        for(int t=0; t<T; t++)
            visited[i][t] = false; 
    }
 
    // Set optimal paths for each field
    long *optimal_dist = new long[T];
    for(int i=0; i<T; i++)
        optimal_dist[i] = DEFAULT_DISTANCE;
   
    // Create a queue for BFS 
    list<Field> queue; 
   
    // Mark the current node as visited and enqueue it 
    visited[f.index][f.mark] = true; 
    queue.push_back(f); 
   
    while(!queue.empty()) 
    { 
        // Dequeue a vertex from queue and print it 
        f = queue.front(); 
        //cout << f.index+1 << " "; 
        queue.pop_front(); 
   
        // Get all adjacent vertices of the dequeued 
        // vertex s. If a adjacent has not been visited,  
        // then mark it visited and enqueue it 
        for (auto i = adj[f.index].begin(); i != adj[f.index].end(); ++i) 
        { 
            if (!visited[*i][f.mark]) { 
                visited[*i][f.mark] = true; 
                Field g;
                g.index = *i;
                g.mark = f.mark;
                g.distance = f.distance + 1;
 
                if(types[*i] == g.mark) {
                    if(g.distance < optimal_dist[g.mark-1]) {
                        optimal_dist[g.mark-1] = g.distance;
                        if(g.mark == T)
                            continue;
                        //cout << ">M" << g.mark << ":" << g.distance << "< ";
                        g.mark = g.mark + 1;
                    }
                }
                int lastMark = getLastMark(visited[g.index], g.mark);
                if(lastMark > g.mark && g.distance >= optimal_dist[lastMark-1])
                    continue;
 
                queue.push_back(g); 
            } 
        } 
    } 
    cout << optimal_dist[T-1] << endl;
    for(int i = 0; i < R*S; i++) 
        delete [] visited[i];
    delete[] visited;
    delete[] optimal_dist;
 
} 
 
int main(void) 
{
    //auto start = high_resolution_clock::now(); 
    int R, S, P, T, start_idx;
    scanf("%d %d %d %d\n", &R, &S, &P, &T);
    //printf("# of rows: %d, # of cols: %d, # of obstacles: %d, # of marks: %d\n---------------------------------------------------\n", R, S, P, T);
     
 
    int row, col;
    scanf("%d %d\n", &row, &col);
    start_idx = (row - 1) * S + (col - 1);
    //printf("start index: %d [%d, %d]\n", (R - row) * S + (col - 1) , row, col);
    bool **obstacles = new bool*[R];
    for(int i=0; i<R; ++i) {
        obstacles[i] = new bool[S];
        for(int j=0; j<S; j++) 
            obstacles[i][j] = false;
    }
 
    for(int p=0; p<P; p++) {
        scanf("%d %d\n", &row, &col);
        obstacles[row-1][col-1] = true;
    }
 
    int **arr = new int*[R];
    for(int i=0; i<R; ++i)
        arr[i] = new int[S];
    int x = 0;
    for(int r = 0; r<R; r++){
        for(int c = 0; c<S; c++){ 
            arr[r][c] = x;
            x++;
        }
    }
    Graph g(R, S, T);
    for(int t=1; t<=T; t++) {
        int N, row, col;
        scanf("%d\n", &N);
        for(int m=0; m<N; m++) {
            scanf("%d %d\n", &row, &col);
            //printf("%d %d\n", row, col);
            g.addType(t, row-1, col-1);
        }
    }
    for(int r = 0; r < R; r++){
        for(int c = 0; c < S; c++) {
            if(obstacles[r][c]) {
                continue;
            } else if(r == 0 && c == 0){
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + S));
            } else if(r == 0 && c == (S - 1) ) {
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + S));
            } else if(r == (R - 1) && c == (S - 1) ) {
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - S));
            } else if(r == (R - 1) && c == 0){
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - S));
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
            } else if(r == R - 1){
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - S));
            } else if(c == S - 1){
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - S));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + S));
            } else if(c == 0){
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + S));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - S));
            } else if(r == 0){
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + S));
            } else{
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + S));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - S));
            }
        }
    }
    /*auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    std::cout << "Graph loaded in " << duration.count() << "ms" << std::endl; 
 
    start = high_resolution_clock::now();*/
    Field f;
    f.index = start_idx;
    f.distance = 0;
    f.mark = 1;
    g.BFS(f);
 
 
    /*stop = high_resolution_clock::now(); 
    duration = duration_cast<milliseconds>(stop - start); 
    std::cout << "BFS done in " << duration.count() << "ms" << std::endl; */
 
    for(int i=0; i<R; ++i) {
        delete [] arr[i];
        delete [] obstacles[i];
    }
    delete [] arr;
    delete [] obstacles;
    g.deleteGraph();
    return 0;
}