#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_set>
using namespace std;

class Graph
{
    vector<unordered_set<int>> adj;   

public:
    int addUserId()
    {
        adj.push_back(unordered_set<int>());
        return adj.size() - 1;  
    }

    void addLink(int u, int v)
    {
        adj[u].insert(v);
        adj[v].insert(u);   
    }

    const unordered_set<int>& neighbors(int u) 
    {
        return adj[u];
    }

    bool areFriends(int u, int v) 
    {
        return adj[u].count(v) > 0;
    }

    int numUsers() 
    {
        return adj.size();
    }

};

#endif