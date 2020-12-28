#include <iostream>
#include <climits>
#include <stdlib.h>
#include <cmath>
using namespace std;

int minInSubArray(int from, int to, int *arr)
{
    int curr_min = INT_MAX;
    int curr_min_i = -1;
    for(int i=from; i<=to; i++) {
        if(arr[i] < curr_min) {
            curr_min = arr[i];
            curr_min_i = i;
        }
    }
    return curr_min_i;
}

int maxInSubArray(int from, int to, int *arr)
{
    int curr_max = INT_MIN;
    int curr_max_i = -1;
    for(int i=from; i<=to; i++) {
        if(arr[i] > curr_max) {
            curr_max = arr[i];
            curr_max_i = i;
        }
    }
    return curr_max_i;
}

int linearSearch(int from, int to, float n, int *arr) 
{
    float difference = LONG_MAX;
    int closest_i = -1;
    for(int i=from+1; i<to; i++) {
        if( std::abs(float(arr[i]) - (float) n) < difference ) {
            difference = std::abs(float(arr[i]) - (float) n);
            closest_i = i;
        }
    }
    return closest_i;
}

long maxQuality(int low, int high, int *arr) 
{
    if(low == high)
        return 0;
    int mid = (low + high) / 2;
    long leftQuality = maxQuality(low, mid, arr);
    long rightQuality = maxQuality(mid+1, high, arr);

    int rightMaxIdx = maxInSubArray(mid+1, high, arr);
    int leftMinIdx = minInSubArray(low, mid, arr);
    float midEstimate = (float) ((float)(arr[rightMaxIdx] - (float) arr[leftMinIdx]) / 2) + (float) arr[leftMinIdx];
    int bestMidIdx = linearSearch(leftMinIdx, rightMaxIdx, midEstimate, arr);

    long wholeQuality = std::min(arr[bestMidIdx] - arr[leftMinIdx], arr[rightMaxIdx] - arr[bestMidIdx]);
    return std::max(std::max(leftQuality, rightQuality), wholeQuality);
}   

int main(void) 
{
    int N;
    scanf("%d\n", &N);
    int *arr = new int[N];
    for(int i=0; i<N; i++) 
        scanf("%d ", &arr[i]);
    long quality = 0;
    quality = maxQuality(0, N-1, arr);
    std::cout << quality << std::endl;
    delete[] arr;
    return 0;   
}
