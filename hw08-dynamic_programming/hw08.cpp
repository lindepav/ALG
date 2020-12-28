#include <iostream>
#include <vector>
#include <chrono> 

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
int main(void) 
{
    int N, H, C;
    scanf("%d %d %d\n", &N, &H, &C);
    int *P = new int[N];
    for(int i=0; i<N; i++) {
        scanf("%d", &P[i]);
    }

    int **S = new int*[N];
    for(int i=0; i<N; i++)
        S[i] = new int[N];
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            if(i==j)
                S[i][i] = P[i];
            else if(j<i)
                S[i][j] = 0;
        }
    }
    for(int i=0; i<N; i++) {
        for(int j=i+1; j<N; j++) {
            S[i][j] = S[i][j-1] + S[j][j];
        }
    }
    printTable(S, N);

    int bestProfit = 0;
    int edgesToMain[] = {H-2, H-1};
    for(auto j : edgesToMain) {
        if(j < 0)
            continue;
        for(int i=0; i<=j; i++) {
            for(int k=j+1; k<N; k++) {
                int S1 = S[i][j];
                int S2 = S[j+1][k];
                int S_min = min(S1, S2);
                int S_max = max(S1, S2);
                int S = min(abs(S_min), abs(S_max));
                int L = k - i;
                int profit = 0;
                if(S_min < 0 && S_max > 0 && S > L)
                    profit = S - L - C;
                else 
                    profit = -C;
                if(profit > bestProfit) 
                    bestProfit = profit;
                cout << i << ".." << j << ".." << k << " -> " << profit << endl;
            }
        }
    }
    cout << bestProfit << endl;

    delete [] P;
    for(int i=0; i<N; i++)
        delete[] S[i];
    delete [] S;
    return 0;
}

