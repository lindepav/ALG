#include <iostream>
#include <stdlib.h>
#include <queue>
#include <set>
#include <vector>

using namespace std;

struct Pos {
    int i;
    int j;

    bool const operator==(const Pos& ob) const
    {
        return i == ob.i && j == ob.j;
    }
 
    bool operator<(const Pos& ob) const
    {
        return i < ob.i || (i == ob.i && j < ob.j);
    }
};

struct Sector {
    Pos x;
    int E;
    vector<Sector> parent;

    bool const operator==(const Sector& ob) const
    {
        return x == ob.x;
    }
 
    bool operator<(const Sector& ob) const
    {
        return x < ob.x || (x == ob.x && x < ob.x);
    }
};

int pos_i[] = {1, 0, -1 ,0};
int pos_j[] = {0, 1, 0, -1};

bool isValid(int i, int j, int M, int N) 
{
    if( ((i >= 0) && (i < M)) && ((j >= 0) && (j < N)) )
        return true;
    return false;
}

int printPath(vector<Sector> path)
{
    if (path.size() == 0)
        return 0;
 
    int len = printPath(path[0].parent) + 1;
    cout << "(" << path[0].x.i << ", " << path[0].x.j << ", " << path[0].E << ") ";
    return len;
}

set<Sector> erase_visited_path(set<Sector> visited, vector<Sector> path)
{
    if (path.size() == 0)
        return visited;
    Sector current = path[0];
    if(visited.count(current)) {
        visited.erase(current);
        visited = erase_visited_path(visited, current.parent);
    }
    return visited;
}

int bfs(int **I, int **E_acc, int M, int N, int E_init, int D_mov, int D_env)
{
    queue<Sector> Q;
    queue<Sector> q;
    Pos source_pos = {0, 0};
    Sector source = {source_pos, E_init};
    Q.push(source);
    set<Pos> visited;
    visited.insert(source_pos);

    while(!Q.empty()) {
        Sector initial_sector = Q.front();
        Q.pop();
        visited.clear();
        visited.insert(initial_sector.x);
        q.push(initial_sector);
        while(!q.empty()) {
            Sector current_sector = q.front();
            q.pop();
            int cur_i = current_sector.x.i;
            int cur_j = current_sector.x.j;

            if(cur_i == M-1 && cur_j == N-1) {
                vector<Sector> cur_parent;
                cur_parent.push_back(current_sector);
                int len = printPath(cur_parent);
                cout << endl;
                return len - 1;
            } 

            int E = current_sector.E;
            if(E == 0 || E < E_acc[cur_i][cur_j]) {
                continue;
            }
            if(E > E_acc[cur_i][cur_j]) {
                Q.push(current_sector);
                E_acc[cur_i][cur_j] = E;
            } 
            int I1 = I[cur_i][cur_j];

            for(int move=0; move<4; move++) {
                int i2 = cur_i + pos_i[move];
                int j2 = cur_j + pos_j[move];
                int E2 = 0;
                if(isValid(i2, j2, M, N)) {
                    int I2 = I[i2][j2];
                    if(I1 < I2) {
                        E2 = E + D_mov;
                    } else if(I1 == I2) {
                        E2 = E;
                    } 
                    else {
                        if(E >= D_mov) {
                            E2 = E - D_mov;
                        } else {
                            E2 = 0;
                        }
                    }
                    if(E2 >= D_env + I2) {
                        E2 -= D_env;
                    } else {
                        if(E2 > I2){
                            E2 = I2;
                        } 
                    }
                    Pos next_pos = {i2, j2};
                    if(!visited.count(next_pos)) {
                        vector<Sector> cur_parent;
                        cur_parent.push_back(current_sector);
                        Sector next = {next_pos, E2, cur_parent};
                        visited.insert(next_pos);
                        q.push(next);
                    } 
                }
            }
        }
    }
    return 0;
}

int main(void) 
{
    int M, N, E_init, D_mov, D_env;
    scanf("%d %d %d %d %d\n", &M, &N, &E_init, &D_mov, &D_env);

    int **I = new int*[M];
    int **E_acc = new int*[M];
    for(int i=0; i<M; i++) {
        I[i] = new int[N];
        E_acc[i] = new int[N]; 
        for(int j=0; j<N; j++) {
            scanf("%d ", &I[i][j]);
            E_acc[i][j] = -1;
        }
    }
    E_acc[0][0] = E_init;

    int result = bfs(I, E_acc, M, N, E_init, D_mov, D_env);
    cout << result << endl;

    for(int i=0; i<M; i++)
        delete [] I[i];
    delete [] I;
    return 0;
}
