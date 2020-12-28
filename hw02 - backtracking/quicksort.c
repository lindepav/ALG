#include <stdio.h>
#include <stdlib.h>
 

int *base_arr; 
int *shared_arr;
int *shared_idx;


static int cmp (const void *a, const void *b) {
    int aa = *((int *) a), bb = *((int *) b);
    if (base_arr[aa] > base_arr[bb])
        return -1;
    if (base_arr[aa] == base_arr[bb])
        return 0;
    if (base_arr[aa] < base_arr[bb])
        return 1;
}

void f(int n) {
  for(int i=0; i<n; i++) {
    printf("d:%d -> %d\n", shared_arr[i], shared_idx[i]);
  }
}


 
int main (void) {
  int *arr, *idx;
  int n, i;
  printf ("\nEnter n: ");
  scanf ("%d", &n);
  arr = malloc (sizeof (int) * n);
  idx = malloc (sizeof (int) * n);
  printf ("\nEnter the list: ");
  for (int i = 0; i < n; i++) {
    scanf ("%d", &arr[i]);
  }
  for (int i = 0; i < n; i++){
    idx[i] = i;
  }

  base_arr = arr;
  shared_arr = malloc(n * sizeof(int));
  shared_idx = malloc(n * sizeof(int));
  qsort (idx, n, sizeof (int), cmp);
  for(int i=0; i<n; i++) {
    shared_arr[i] = 0;
    shared_idx[i] = 0;
  }
  shared_arr = arr;
  shared_idx = idx;
 
  printf ("\nOriginal list: ");
  for (int i = 0; i < n; i++) {
      printf ("%d ", shared_arr[i]);
    }
 
  printf ("\nSorted index: ");
  for (i = 0; i < n; i++) {
      printf ("%d ", shared_idx[i]);
    }
 
  printf ("\nSorted array using arr[sorted_idx[i]]: ");
  for (i = 0; i < n; i++) {
      printf ("%d ", shared_arr[shared_idx[i]]);
  }
 
  printf("\nCalling function...\n");
  f(n);
  free (shared_arr);
  free (shared_idx);
  base_arr = NULL;
 
  printf ("\n");
  return 0;
}