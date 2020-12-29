#include <iostream>
#include <vector>
#include <chrono> 
#include <climits>

using namespace std;
using namespace std::chrono; 

void printTable(int **S, int N) 
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

int profitOfBuiltPaths(int **acc, int C, int i, int k)
{
    if(i == k)
        return 0;
    int best_profit = INT_MIN;
    for(int j=i; j<k; j++) {
        int S1 = acc[i][j];
        int S2 = acc[j+1][k];
        int S_min = min(S1, S2);
        int S_max = max(S1, S2);
        int S = min(abs(S_min), abs(S_max));
        int L = k - i;
        int profit = 0;
        if(S_min < 0 && S_max > 0 && S > L) { // TODO: lze zoptimalizovat
            profit = S - L;
        } else {
            profit = 0;
        }
        profit -= C;
        profit += profitOfBuiltPaths(acc, C, i, j) + profitOfBuiltPaths(acc, C, j+1, k);
        if(profit > best_profit) 
            best_profit = profit;
    }
    return best_profit;
}

int main(void) 
{
    int N, H, C;
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
    //printTable(S, N);

    int bestProfit = 0;
    for(int j=0; j<N; j++) {
        for(int i=0; i<=j; i++) {
            if(H-1 < i) continue;
            for(int k=j+1; k<N; k++) {
                if(H-1 > k) continue;
                int S1 = acc[i][j];
                int S2 = acc[j+1][k];
                int S_min = min(S1, S2);
                int S_max = max(S1, S2);
                int S = min(abs(S_min), abs(S_max));
                int L = k - i;
                int profit = 0;
                if(S_min < 0 && S_max > 0 && S > L) { // TODO: lze zoptimalizovat
                    profit = S - L;
                } else {
                    profit = 0;
                }
                profit -= C;
                profit += profitOfBuiltPaths(acc, C, i, j) + profitOfBuiltPaths(acc, C, j+1, k);

                if(profit > bestProfit)
                    bestProfit = profit;
            }
        }
    }
    cout << bestProfit << endl;

    delete [] P;
    for(int i=0; i<N; i++)
        delete[] acc[i];
    delete [] acc;
    return 0;
}

