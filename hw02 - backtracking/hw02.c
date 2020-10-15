#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct AdjList {
   int vertex;
   struct AdjList *next;
} AdjList;

typedef struct Node {
    int type;
    AdjList *head;
} Node;

Node *V;
int *assigned_T1;
int *assigned_T2;
int N, M, A, B;

void load_input();
void push(AdjList *head, int val);
void print_list(AdjList *head);
void print_adjacent_list();
void print_assigned_agents();

void print_assigned_agents() {
    printf("T1: ");
    for(int i=0; i<A; i++) {
        if(i == A-1)
            printf("%d\n", assigned_T1[i]);
        else 
            printf("%d ", assigned_T1[i]);
    }

    printf("T2: ");
    for(int i=0; i<B; i++) {
        if(i == B-1)
            printf("%d\n", assigned_T2[i]);
        else 
            printf("%d ", assigned_T2[i]);
    }
}

void print_adjacent_list() {
    for(int v=0; v<N; v++) {
        printf("%d: ", v+1);
        print_list(V[v].head);
    }
}

void print_list(AdjList *head) {
    AdjList *current = head;

    while (current != NULL) {
        printf("%d ", current->vertex+1);
        current = current->next;
        if(current->next == NULL) {
            printf("%d\n", current->vertex+1);
            break;
        }
    }
}
void push(AdjList *head, int val) {
    if(head->vertex < 0) {
        head->next = NULL;
        head->vertex = val;
        return;
    }
    AdjList *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = (AdjList *) malloc(sizeof(AdjList));
    current->next->vertex = val;
    current->next->next = NULL;
}

void load_input() {
    V = malloc(N * sizeof(Node));
    assigned_T1 = malloc(A * sizeof(int));
    assigned_T2 = malloc(B * sizeof(int));
    int from, to;
    for(int i=0; i<N; i++) {
        Node current;
        current.type = 0;
        AdjList *head = (AdjList *) malloc(sizeof(AdjList));
        head->next = NULL;
        head->vertex = -1;
        current.head = head;
        V[i] = current;
    }

    for(int e=0; e<M; e++) {
        scanf("%d %d\n", &from, &to);
        push(V[from-1].head, to-1);
        push(V[to-1].head, from-1);
    }
    print_adjacent_list();


    for(int i=0; i<A; i++) 
        assigned_T1[i] = 0;
    for(int i=0; i<B; i++) 
        assigned_T2[i] = 0;

    print_assigned_agents();
}


int main(void) {
    scanf("%d %d %d %d\n", &N, &M, &A, &B);
    printf("N= %d, M= %d, A= %d, B= %d\n-----------------------\n", N, M, A, B);
    load_input();
    




    return 0;
}