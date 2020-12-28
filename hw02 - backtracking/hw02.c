#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct AdjListNode { 
    int dest; 
    struct AdjListNode* next; 
} AdjListNode; 
  
typedef struct AdjList { 
    int agent_type;
    struct AdjListNode *head;  
} AdjList;

typedef struct Graph  { 
    struct AdjList* array; 
} Graph; 


Graph *graph;
//int *node_degrees, *sorted_idxs, 
int *base_arr;
//bool *visited;
int N, M, A, B, best_score;
int *types;

void load_input();
void print_list(AdjListNode *head);
void print_adjacent_list();
void print_assigned_agents();
void print_sorted_vertices(int *sorted_idxs);
void print_visited(bool *visited);
void backtrack(int current_node, int current_score, int a_count, int b_count, bool *visited, int *node_degrees, int *sorted_idxs);
int get_score(int current_score, int current_node, int type, bool *visited);
int get_upper_bound(int A_count, int B_count, bool *visited, int *node_degrees, int *sorted_idxs);
AdjListNode* new_adj_list_node(int dest);
Graph* create_graph();
void add_edge(Graph* graph, int src, int dest);
void print_graph(Graph* graph);

AdjListNode* new_adj_list_node(int dest) { 
    AdjListNode* newNode = (AdjListNode*) malloc(sizeof(AdjListNode)); 
    newNode->dest = dest; 
    newNode->next = NULL; 
    return newNode; 
} 

Graph* create_graph() { 
    Graph* graph = (Graph*) malloc(sizeof(Graph));   

    graph->array =  (AdjList*) malloc(N * sizeof(AdjList)); 
  
    int i; 
    for (i = 0; i < N; i++) {
        graph->array[i].head = NULL; 
        graph->array[i].agent_type = -1;
    }
  
    return graph; 
} 

void add_edge(Graph* graph, int src, int dest) { 
    AdjListNode* newNode = new_adj_list_node(dest); 
    newNode->next = graph->array[src].head; 
    graph->array[src].head = newNode; 

    newNode = new_adj_list_node(src); 
    newNode->next = graph->array[dest].head; 
    graph->array[dest].head = newNode; 
} 

void print_assigned_agents() {
    printf("-> Currently assigned agents:\n");
    for(int i=0; i<N; i++) {
        printf("%d: %d\n", i, graph->array[i].agent_type);
    }
}

void print_adjacent_list() {
    printf("List of adjacent vertices:\n");
    print_graph(graph);
    printf("-----------------------\n");
}

void print_graph(Graph* graph) { 
    int v; 
    for (v = 0; v < N; ++v) { 
        AdjListNode* neighbour = graph->array[v].head; 
        printf("%d: head ", v+1); 
        while (neighbour) { 
            if(neighbour->next == NULL) {
                printf("%d\n", neighbour->dest+1); 
                break;
            }
            printf("%d ", neighbour->dest+1); 
            neighbour = neighbour->next; 
        } 
    } 
} 

static int cmp (const void *a, const void *b) {
    int aa = *((int *) a), bb = *((int *) b);
    if (base_arr[aa] > base_arr[bb])
        return -1;
    if (base_arr[aa] < base_arr[bb])
        return 1;
    return 0;
}

void load_input() {
    graph = create_graph();
    bool *visited = malloc(N * sizeof(int));
    int *node_degrees = malloc(N  * sizeof(int));
    int *sorted_idxs = malloc (N * sizeof(int));
    types = malloc(3 * sizeof(int));
    types[0] = 1;
    types[1] = 2;
    types[2] = 0;
    int src, dist;
    for(int i=0; i<N; i++) {
        sorted_idxs[i] = i;
        node_degrees[i] = 0;
    }

    for(int e=0; e<M; e++) {
        scanf("%d %d\n", &src, &dist);
        add_edge(graph, src-1, dist-1);
        node_degrees[src-1]++;
        node_degrees[dist-1]++;
    }

    qsort (sorted_idxs, N, sizeof (int), cmp); 
    print_sorted_vertices(sorted_idxs);
    print_adjacent_list();
    print_assigned_agents();

    for(int i=0; i<N; i++)
        visited[i] = false;

    printf("\n");
}

void print_sorted_vertices(int *sorted_idxs) {
    printf("Sorted list of vertices:\n");
    for(int i=0; i<N; i++) {
        if(i == N-1) {
            printf("%d\n-----------------------\n", sorted_idxs[i]+1);
            break;
        }
        printf("%d ", sorted_idxs[i]+1);
    }
}

void print_visited(bool *visited) {
    printf("\nVisited: ");
    for(int i=0; i<N; i++) {
        if(visited[i]) {
            if(i == N-1) {
                printf("%d\n", i+1);
                break;
            }
            printf("%d ", i+1);
        }
    }
}

int get_upper_bound(int A_count, int B_count, bool *visited, int *node_degrees, int *sorted_idxs) {
    int score = 0;
    printf("starting function...");
    for(int v=0; v<N; v++) {
        int node_degree = node_degrees[v];
        int i = sorted_idxs[v];
        if(visited[i])
            continue;
        if(A_count >= A) {
            if(B_count >= B) 
                return score;
            else {
                B_count++;
                score = score + node_degree;
            }
        } else {
            A_count++;
            score = score + 2 * node_degree;
        }
    }
    return score;
}

int get_score(int current_score, int current_node, int type, bool *visited) {
    int reward_a[3] = {0, 1, 0}; 
    int reward_b[3] = {0, 0, 1}; 
    int diffs[3] = {0, 1, -1};
    int new_count = 0;
    int dif = 0;
    
    /* getting new rewards */
    AdjListNode *neighbour = graph->array[current_node].head;
    while(neighbour != NULL) { 
        int nidx = neighbour->dest;
        int ntype = graph->array[nidx].agent_type;
        if(!visited[nidx]) {
            new_count = new_count + reward_b[type];
        } else {
            new_count = new_count + reward_a[type];
            if(type > 0) 
                dif = dif + diffs[ntype];
        }
        neighbour = neighbour->next;
    }

    return new_count + dif;
}

void backtrack(int current_node, int current_score, int a_count, int b_count, bool *visited, int *node_degrees, int *sorted_idxs) { 
    /* STOP CONDITION */
    if(a_count >= A && b_count >= B) {
        printf("STOP!\n");
        if(current_score > best_score)
            best_score = current_score;
        return;
    }

    //printf("\nNode #%d", current_node+1);
    //printf("\n%d %d %d %d\n", current_node, current_score, a_count, b_count);
    //print_visited();
    printf("wtf?");

    /* PRUNING */
    int bound = get_upper_bound(a_count, b_count, visited, node_degrees, sorted_idxs);
    printf(" bound: %d, ", bound);
    if(bound <= best_score)
        return;

    for(int i=0; i<3; i++) {
        int type = types[i];
        if(type==1) {
            a_count++;
            if(a_count > A)
                continue;
        } else if(type==2) {
            b_count++;
            if(b_count > B)
                continue;
        }
        int new_score = get_score(current_score, current_node, type, visited);
        graph->array[current_node].agent_type = type;
        printf("new score for %d {T%d} is %d", current_node+1, type, new_score);

        AdjListNode *neighbour = graph->array[current_node].head;
        while(neighbour != NULL) {
            int nidx = neighbour->dest;
            if(visited[nidx]) 
                continue;
            visited[current_node] = true; // <--- BACKTRACKING
            backtrack((int) nidx, new_score, a_count, b_count, visited, node_degrees, sorted_idxs);
            visited[current_node] = false; // <--- BACKTRACKING
            neighbour = neighbour->next;
        }
    }
    return;
}

void free_input() {
    //free(node_degrees);
    //free(visited);
    free(types);
}

int main(void) {
    scanf("%d %d %d %d\n", &N, &M, &A, &B);
    printf("N= %d, M= %d, A= %d, B= %d\n-----------------------\n", N, M, A, B);
    //load_input();
    graph = create_graph();
    bool *visited = malloc(N * sizeof(bool));
    int *node_degrees = malloc(N  * sizeof(int));
    int *sorted_idxs = malloc (N * sizeof(int));
    types = malloc(3 * sizeof(int));
    types[0] = 1;
    types[1] = 2;
    types[2] = 0;
    int src, dist;
    for(int i=0; i<N; i++) {
        sorted_idxs[i] = i;
        node_degrees[i] = 0;
    }

    for(int e=0; e<M; e++) {
        scanf("%d %d\n", &src, &dist);
        add_edge(graph, src-1, dist-1);
        node_degrees[src-1]++;
        node_degrees[dist-1]++;
    }
    base_arr = node_degrees;
    qsort (sorted_idxs, N, sizeof (int), cmp); 
    int *shared_idxs = malloc(N * sizeof(int));
    int *shared_arr = malloc(N * sizeof(int));
    for(int i=0; i<N; i++) {
        shared_arr[i] = node_degrees[i];
        shared_idxs[i] = sorted_idxs[i];
    }

    print_sorted_vertices(shared_idxs);
    print_adjacent_list();
    print_assigned_agents();

    for(int i=0; i<N; i++)
        visited[i] = false;

    printf("\n");
    

    //long score = backtrack(V[0], 0, 0, 0);
    //printf("\n\nScore is %ld\n", score);
    best_score = 0;

    backtrack(sorted_idxs[0], 0, 0, 0, visited, shared_arr, shared_idxs);
    printf("\n----------------------\nBEST SCORE: %d\n", best_score);



    return 0;
}