#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdbool.h> 

void binary_search_test();
int binary_search(int arr[], int from, int to, int d, int last);
int* linear_search(int arr[], int from, int to, int first, int W_pow, int D);
long count_possible_routes(int arr[], int N, int D);
long count_cross_routes(int s1[], int s2[], int N1, int N2, int D, int W);
void read_vector(int N, int s[]);
void print_riverside(int s[], char name[], int N);

void print_riverside(int s[], char name[], int N) {
    printf("%s = { ", name);
    for(int i=0; i<N; i++) 
        printf("%d ", s[i]);
    printf("}\n");
}

void binary_search_test() {
    int s1[] = {0, 1, 3, 4};
    int s2[] = {0, 2, 3, 4};
    int s3[] = {0, 1, 2, 4, 5, 6, 7, 8, 11, 13};
    size_t N1 = sizeof(s1)/sizeof(s1[0]);
    size_t N2 = sizeof(s2)/sizeof(s2[0]);
    size_t N3 = sizeof(s3)/sizeof(s3[0]);
    int d = 3;

    int res1[] = {2, 2, 1};
    int res2[] = {2, 2, 1};
    int res3[] = {2, 2, 2, 3, 3, 2, 1, 1, 1};

    for(int i=0; i<N1-1; i++) {
        int largest_idx = binary_search(s1, i, N1-1, d + s1[i], -1);
        int count = (largest_idx < 0) ? 0 : largest_idx - i;
        assert(res1[i] == count);
    }
    for(int i=0; i<N2-1; i++) {
        int largest_idx = binary_search(s2, i, N2-1, d + s2[i], -1);
        int count = (largest_idx < 0) ? 0 : largest_idx - i;
        assert(res2[i] == count);
    }
    for(int i=0; i<N3-1; i++) {
        int largest_idx = binary_search(s3, i, N3-1, d + s3[i], -1);
        int count = (largest_idx < 0) ? 0 : largest_idx - i;
        assert(res3[i] == count);
    }
}

void read_vector(int N,  int s[]) {
    for(int i=0; i < N; i++) {
        scanf("%d", &s[i]);
    }
    return;
}

int binary_search(int arr[], int from, int to, int d, int last) { 
    if (to >= from) { 
        int mid = from + (to - from) / 2; 
  
        if (arr[mid] == d) 
            return mid; 
  
        if (arr[mid] > d) 
            return binary_search(arr, from, mid - 1, d, last); 

        return binary_search(arr, mid + 1, to, d, mid); 
    } 
    return (arr[last] <= d) ? last : -1; 
} 

/* Function to compute all possible routes on a single riverside 
* Input: nodes on a riverside arr[], number of nodes N, the longest distance they can travel d
* Output: count of possible routes
*/
long count_possible_routes(int arr[], int N, int D) {
    long int count = 0;
    for(int i=0; i<N; i++) {
        int current_val = arr[i];
        int largest_idx = binary_search(arr, i, N-1, current_val + D, -1);
        int cur_count = (largest_idx < 0) ? 0 : largest_idx - i;
        count = count + cur_count;
    }
    return count;
}

int* linear_search(int arr[], int from, int to, int first, int W_pow, int D) { 
    int first_idx = -1;
    int last_idx = -1;
    for(int i=from; i<to; i++) {
        int current_val = arr[i];
        int dif = first - current_val;
        float d = sqrt(W_pow + dif * dif);
        if(d <= D) {
            from = i;
            first_idx = i;
            break;
        }
    }
    while(from <= to) {
        int mid = from + (to - from) / 2;
        int current_val = arr[mid];
        int dif = first - current_val;
        float d = sqrt(W_pow + dif * dif);
        if(d == D) {
            last_idx = mid;
            break;
        }
        if(d < D) {
            from = mid + 1;
            last_idx = mid;
        } else {
            to = mid - 1;
        }
    }
    int *ret = malloc(2 * sizeof(int));
    ret[0] = first_idx;
    ret[1] = last_idx;
    return ret;
}  

long count_cross_routes(int s1[], int s2[], int N1, int N2, int D, int W) {
    register int i = 0, j = 0;
    long count = 0;
    W = pow(W, 2);
    while(i < N1 && j < N2) {
        int current_val = s1[i];
        int *ret = linear_search(s2, j, N2 - 1, current_val, W, D);
        if(ret[0] >= j) {
            j = ret[0];
            count = count + ret[1] - j + 1;
            //printf("for %d is new count %d\n", current_val, ret[1] - j + 1);
        } 
        i++;
    }
    return count;
}


int main(void) {
    int W, D, N1, N2;
    int *s1, *s2;
    scanf("%d %d\n", &W, &D);

    scanf("%d\n", &N1);
    s1 = malloc(N1 * sizeof(int));
    read_vector(N1, s1);

    scanf("%d\n", &N2);
    s2 = malloc(N2 * sizeof(int));
    read_vector(N2, s2);

    //print_riverside(s1, "s1", N1);
    //print_riverside(s2, "s2", N2);
    //printf("WIDTH= %d, DISTANCE= %d, N1 = %d, N2 = %d.\n-----------------------\n", W, D, N1, N2);
    //binary_search_test();

    /*printf("testing linear search...\n");
    long *ret = linear_search(s2, 0, N2, 7, W, D);
    printf("ret = {%d, %ld}\n", s2[ret[0]], ret[1]);*/

    //clock_t before = clock();

    long count1 = count_possible_routes(s1, N1, D);
    long count2 = count_possible_routes(s2, N2, D);
    long count3 = count_cross_routes(s1, s2, N1, N2, D, W);
    long count = count1 + count2 + count3;
    /*printf("For s1 is the count of all possible routes %ld.\n", count1);
    printf("For s2 is the count of all possible routes %ld.\n", count2);
    printf("For crossings is the count of all possible routes %ld.\n", count3);
    printf("Count of all possible routes is %ld.\n", count);*/
    printf("%ld\n", count);

    //clock_t difference = clock() - before;
    //int msec = difference * 1000 / CLOCKS_PER_SEC;
    //printf("Program finished in %d ms.\n", msec);

    return 0;
}