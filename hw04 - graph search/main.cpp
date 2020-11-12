#include<iostream> 
#include <vector> 
#include <list>
#include <chrono> 


#define DEFAULT_DISTANCE INT_R

using namespace std; 
using namespace std::chrono; 

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
    void BFS(int s);   
    void deleteGraph();
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
    this->types[(r-1) * R + c] = type;
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

void Graph::BFS(int s) 
{ 
    // Mark all the vertices as not visited 
    bool *visited = new bool[R * S]; 
    for(int i = 0; i < R*S; i++) 
        visited[i] = false; 
  
    // Create a queue for BFS 
    list<int> queue; 
  
    // Mark the current node as visited and enqueue it 
    visited[s] = true; 
    queue.push_back(s); 
  
    while(!queue.empty()) 
    { 
        // Dequeue a vertex from queue and print it 
        s = queue.front(); 
        cout << s << " "; 
        queue.pop_front(); 
  
        // Get all adjacent vertices of the dequeued 
        // vertex s. If a adjacent has not been visited,  
        // then mark it visited and enqueue it 
        for (auto i = adj[s].begin(); i != adj[s].end(); ++i) 
        { 
            if (!visited[*i]) 
            { 
                visited[*i] = true; 
                queue.push_back(*i); 
            } 
        } 
    } 
} 

int main(void) 
{
    auto start = high_resolution_clock::now(); 
    int R, S, P, T;
    scanf("%d %d %d %d\n", &R, &S, &P, &T);
    printf("# rs: %d, # cs: %d, # obstacles: %d, # of marks: %d\n---------------------------------------------------\n", R, S, P, T);
    
    int row, col;
    bool **obstacles = new bool*[R];
    for(int i=0; i<R; ++i) {
        obstacles[i] = new bool[S];
        for(int j=0; j<S; j++) 
            obstacles[i][j] = false;
    }
        
    for(int t=0; t<T; t++) {
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
    for(int r = 0; r < R; r++){
        for(int c = 0; c < S; c++) {
            if(obstacles[r][c]) {
                continue;
            } else if(r == 0 && c == 0){
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + R));
            } else if(r == 0 && c == (S - 1) ) {
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + R));
            } else if(r == (R - 1) && c == (S - 1) ) {
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - R));
            } else if(r == (R - 1) && c == 0){
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - R));
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
            } else if(r == R - 1){
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - R));
            } else if(c == S - 1){
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - R));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + R));
            } else if(c == 0){
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + R));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - R));
            } else if(r == 0){
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + R));
            } else{
                if(!obstacles[r][c+1])
                    g.addEdge(arr[r][c],(arr[r][c] + 1));
                if(!obstacles[r][c-1])
                    g.addEdge(arr[r][c],(arr[r][c] - 1));
                if(!obstacles[r+1][c])
                    g.addEdge(arr[r][c],(arr[r][c] + R));
                if(!obstacles[r-1][c])
                    g.addEdge(arr[r][c],(arr[r][c] - R));
            }
        }
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start); 
    cout << "Graph loaded in " << duration.count() << "ms" << endl; 

    for(int i=0; i<R; ++i) {
        delete [] arr[i];
        delete [] obstacles[i];
    }
    delete [] arr;
    delete [] obstacles;
    g.deleteGraph();
    return 0;
}
