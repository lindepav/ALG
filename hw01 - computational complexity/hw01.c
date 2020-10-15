#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <dirent.h> 

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void binary_search_test();
int binary_search(int arr[], long from, long to, int d, int last);
long linear_search(int arr[], long from, long to, long W_pow, int D, int top_val);
long* find_crosses(int arr[], long from, long to, long W_pow, int D, int top_val);
long count_possible_routes(int arr[], int N, int D);
long count_cross_routes(int s1[], int s2[], int N1, int N2, int D, int W);
void read_vector(int N, int s[]);
void print_riverside(int s[], char name[], int N);
void public_tests();

void public_tests() {
    DIR *d;
    struct dirent *dir;
    d = opendir("datapub/");
    char **filenames = malloc(10 * sizeof(char *));
    if (d) {
        int i = 0;
        while ((dir = readdir(d)) != NULL) {
            printf("%d\n", i/2);
            if (i % 2 == 0) 
                filenames[i/2] = dir->d_name;
            i++;
        }
        closedir(d);
    }
    for(int i=0; i<10; i++)
        printf("%s", filenames[i]);
}

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

int binary_search(int arr[], long from, long to, int d, int last) { 
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

long linear_search(int arr[], long from, long to, long W_pow, int D, int top_val) { 
    //printf("linear search from %ld to %ld: \n", from, to);
    if(from < to) {
        for(long i=from; i<=to; i++) {
            long current_val = arr[i];
            long dif = current_val - top_val; //TODO: odebrat ABS
            float d = sqrt(W_pow + dif * dif);
            //printf("\t current d: %.2f for s1[i]: %d\n", d, top_val);
            if(d <= D) 
                return i;
        }
    } else {
        for(long i=from; i>=to; i--) {
            long current_val = arr[i];
            long dif = current_val - top_val; //TODO: odebrat ABS
            float d = sqrt(W_pow + dif * dif);
            //printf("\t current d: %.2f for s1[i]: %d\n", d, top_val);
            if(d <= D) 
                return i;
        }
    }
    return (long)-1;
}  

long* find_crosses(int arr[], long from, long to, long W_pow, int D, int top_val) {
    long new_from = linear_search(arr, from, to, W_pow, D, top_val);
    if(new_from < 0) {
        return NULL;
    }
        
    to = linear_search(arr, to, from, W_pow, D, top_val);
    long *ret = malloc(2 * sizeof(long));
    ret[0] = new_from;
    ret[1] = to;
    return ret;
}

long count_cross_routes(int s1[], int s2[], int N1, int N2, int D, int W) {
    register long i = 0, j_from = 0, j_to = 0;
    long count = 0;
    long W_pow = pow(W, 2);
    j_to = N2 - 1;
    //printf("max initial difference is %d.\n", j_to);
    while(i < N1 && j_from < N2) {
        int current_val = s1[i];
        long *ret = find_crosses(s2, j_from, MIN(N2 - 1, j_to), W_pow, D, current_val);
        if(ret == NULL) {
            i++;
            continue;
        }
        count = count + ret[1] - ret[0] + (long)1;
        j_from = ret[0];
        j_to = ret[1] + sqrt(D) + 3;
        //printf("i=%ld --> %ld:%ld, count = %ld\n", i, j_from, ret[1], ret[1] - ret[0] + 1);
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