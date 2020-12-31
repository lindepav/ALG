#include <iostream>
#include <vector>
#include <chrono> 
#include <climits>

using namespace std;
using namespace std::chrono; 

int N, C, H;

void printTable(int **S) 
{
    cout << "--------------" << endl;
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            cout << S[i][j] << " ";
        }
        cout << endl;
    }
    cout << "--------------" << endl;
}

int calculateImmediateProfit(int **acc, int i, int j, int k) 
{
    int S1 = acc[i][j];
    int S2 = acc[j+1][k];
    int S_min = min(S1, S2);
    int S_max = max(S1, S2);
    int profit = 0;
    if(S_min < 0 && S_max > 0) {
        int S = min(abs(S_min), abs(S_max));
        int L = k - i;
        if(S > L)  
            profit = S - L;
    } 
    profit -= C;
    return profit;
}

int main(void) 
{
    scanf("%d %d %d\n", &N, &H, &C);
    int *P = new int[N];
    for(int i=0; i<N; i++) {
        scanf("%d", &P[i]);
    }
    int **acc = new int*[N];
    for(int i=0; i<N; i++)
        acc[i] = new int[N];
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            if(i==j)
                acc[i][i] = P[i];
            else if(j<i)
                acc[i][j] = 0;
        }
    }
    for(int i=0; i<N; i++) {
        for(int j=i+1; j<N; j++) {
            acc[i][j] = acc[i][j-1] + acc[j][j];
        }
    }
    H--;
    int **profits = new int*[N];
    for(int i=0; i<N; i++) 
        profits[i] = new int[N];
    
    for(int i=0; i<N; i++) {
        for(int k=0; k<N; k++) {
            profits[i][k] = 0;
        }
    }
    int i=0, k=1, lvl=1;
    while(i<N && k<N) {
        int max_profit = INT_MIN;
        for(int j=i; j<k; j++) {
            int current_profit = calculateImmediateProfit(acc, i, j, k);
            int left_profit = profits[i][j];
            int right_profit = profits[j+1][k];
            current_profit += left_profit + right_profit;
            if(current_profit > max_profit)
                max_profit = current_profit;
        }
        profits[i][k] = max_profit;
        if(k == N-1) {
            lvl++; 
            k = lvl;
            i = 0;
        } else {
            i++; k++;
        }
    }
    //printTable(profits);
    int bestProfit = INT_MIN;
    for(int i=0; i<N; i++) {
        for(int k=0; k<N; k++) {
            if(i <= H && H <= k) {
                int profit = profits[i][k];
                if(profit > bestProfit) {
                    bestProfit = profit;
                }
            }
        }
    }
    cout << bestProfit << endl;

    delete [] P;
    for(int i=0; i<N; i++) {
        delete[] acc[i];
        delete[] profits[i];
    }
    delete [] acc;
    delete [] profits;
    return 0;
}

