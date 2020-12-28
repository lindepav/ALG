#include <iostream>
#include <vector>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::stable_sort
#include <chrono> 

using namespace std;
using namespace std::chrono;

size_t N, M, A, B, best_score;

vector<uint8_t> * graph_back, * graph_forward;
uint8_t * agent_types;
size_t * lookup_table_back, * lookup_table_forward;

size_t getThreshold ( size_t lvl, size_t a, size_t b )
{
    return lookup_table_back[N] - lookup_table_back[lvl] + lookup_table_forward[lvl];
}

size_t addScore ( size_t lvl )
{
    size_t score = 0;
    size_t (*adder) (uint8_t) = [] (uint8_t oth)-> size_t { return oth == 1;};
    if ( agent_types[lvl] == 2 )                        //Duvod proc je prohozene A a B
        adder = [] (uint8_t oth)-> size_t { return oth;};
    else if ( agent_types[lvl] == 1 )
        adder = [] (uint8_t oth)-> size_t { return oth != 1;};

    for ( uint8_t neigh : graph_back[lvl] ) 
        score += adder(agent_types[neigh]);
    return score;
}

void recursion (size_t score, size_t a_count, size_t b_count, size_t lvl)
{
    auto fnc = [&] ( size_t isA, size_t isB )->void {
        agent_types[lvl] = isA + isB*2;
        return recursion( score + addScore(lvl), a_count + isA, b_count + isB, lvl+1);
    };

    if ( a_count == A && b_count == B )
        best_score = (score > best_score) ? score : best_score; 

    if ( lvl == N || score + getThreshold(lvl, a_count, b_count) + score / 90 < best_score )
        return;

    if ( a_count < A )
        fnc(1,0);
    if ( b_count < B )
        fnc(0,1);
    if ( (A - a_count) + (B - b_count) + lvl < N )
        fnc(0,0);
}

int main()
{ 
    scanf("%lu%lu%lu%lu", &N, &M, &B, &A);
    graph_back = new vector<uint8_t> [N];
    graph_forward = new vector<uint8_t> [N];
    agent_types = new uint8_t [N];
    lookup_table_back = new size_t [N+1];
    lookup_table_forward = new size_t [N+1];
    auto add_edge = [](uint8_t u, uint8_t v)->void { graph_forward[min(u,v)].push_back(max(u,v));
                                                   graph_back[max(u,v)].push_back(min(u,v));};

    uint8_t src, dist;
    for(; M > 0; --M) 
    {
        scanf("%hhu%hhu", &src, &dist);
        add_edge(src-1, dist-1); 
    }

    lookup_table_back[0] = lookup_table_forward[N] = 0;
    for ( size_t i = 0; i < N; ++i )
    {
        lookup_table_back[i+1] = lookup_table_back[i] + graph_back[i].size();
        lookup_table_forward[N-i-1] = lookup_table_forward[N-i] + graph_forward[N-i-1].size();
    }

    auto start = high_resolution_clock::now(); 
    recursion(0, 0, 0, 0);
    auto duration = duration_cast<milliseconds>( high_resolution_clock::now() - start); 
    
    //cout << "Program ended in " << duration.count() << "ms" << endl; 
    printf("%lu\n", best_score);
    
    delete [] graph_back;
    delete [] graph_forward;
    delete [] agent_types;
    delete [] lookup_table_back;
    delete [] lookup_table_forward;
    return 0; 
} 
