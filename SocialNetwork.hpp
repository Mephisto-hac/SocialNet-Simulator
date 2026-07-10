#ifndef SOCIALNETWORK_HPP
#define SOCIALNETWORK_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <queue>
#include <climits>
#include "Graph.hpp"
#include "User.hpp"
using namespace std;

class SocialNetwork
{
    Graph graph;
    vector<User> users;
    unordered_map<string, int> usernameWithId;

    string convertingToLowercase(string s)
    {
        for (auto &c : s) c = tolower(c);
        return s;
    }

public:

    void addUser(string username)
    {
        username = convertingToLowercase(username);
        if (usernameWithId.find(username) != usernameWithId.end())
        {
            cout << "Error: User already exists." << endl;
            return;
        }
        int newUserId = graph.addUserId();
        usernameWithId[username] = newUserId;
        User newUser(username);
        users.push_back(newUser);
    }

    void addFriend(string u1, string u2)
    {
        u1 = convertingToLowercase(u1);
        u2 = convertingToLowercase(u2);
        if(usernameWithId.find(u1) == usernameWithId.end())
        {
            cout << "Error : User " << u1 << " doesn't exists. " << endl;
            return;
        }
        if(usernameWithId.find(u2) == usernameWithId.end())
        {
            cout << "Error : User " << u2 << " doesn't exists. " << endl;
            return;
        }

        int idOfu1 = usernameWithId[u1];
        int idOfu2 = usernameWithId[u2];

        graph.addLink(idOfu1, idOfu2);
    }

    void listFriends(string username)
    {
        username = convertingToLowercase(username);
        if(usernameWithId.find(username) == usernameWithId.end())
        {
            cout << "Error : User doesn't exists. " << endl;
            return;
        }
        int id = usernameWithId[username];

        const unordered_set<int>& friendsId = graph.neighbors(id);
        vector<string> friendList;
        for (auto &fid : friendsId)
            friendList.push_back(users[fid].username);

        sort(friendList.begin(), friendList.end());

        for (string &s : friendList)
            cout << s << " ";
        cout << endl;
    }

    void suggestFriends(string username, int n)
    {
        if (n == 0) return;

        username = convertingToLowercase(username);
        if(usernameWithId.find(username) == usernameWithId.end())
        {
            cout << "Error : User doesn't exists. " << endl;
            return;
        }

        int id = usernameWithId[username];
        const unordered_set<int>& F = graph.neighbors(id);

        unordered_map<int, int> mutualCount;

        for (auto &f : F) {
            const unordered_set<int>& candidates = graph.neighbors(f);
            for (auto &c : candidates) {
                if (c == id) continue;             
                if (F.count(c) > 0) continue;        
                mutualCount[c]++;
            }
        }

        vector<pair<int,string>> ranked;   
        for (auto &p : mutualCount)
            ranked.push_back({-p.second, users[p.first].username});

        sort(ranked.begin(), ranked.end());
          

        int shown = 0;
        for (auto &p : ranked)
        {
            if (shown >= n) break;
            cout << p.second << " ";
            shown++;
        }
        cout << endl;
    }

    int degreesOfSeparation(string u1, string u2)
    {
        u1 = convertingToLowercase(u1);
        u2 = convertingToLowercase(u2);
        if(usernameWithId.find(u1) == usernameWithId.end())
        {
            cout << "Error : User " << u1 << " doesn't exists. " << endl;
            return -1;
        }
        if(usernameWithId.find(u2) == usernameWithId.end())
        {
            cout << "Error : User " << u2 << " doesn't exists. " << endl;
            return -1;
        }

        int id1 = usernameWithId[u1];
        int id2 = usernameWithId[u2];

        if (id1 == id2) return 0;

        vector<int> dist(users.size(), INT_MAX);
        queue<pair<int,int>> q;   
        q.push({id1, 0});
        dist[id1]=0;
        

        while (!q.empty())
        {
            auto [curNode, dis] = q.front();
            q.pop();
            if(dis > dist[curNode]) continue;

            for (auto &nodes : graph.neighbors(curNode))
            {
                if(dist[nodes] > dist[curNode] + 1){
                    dist[nodes] = dist[curNode] + 1;
                    q.push({nodes, dist[nodes]});
                }
            }
        }
        if(dist[id2] == INT_MAX) return -1;  
        return dist[id2]; 
    }

    void addPost(string username, string content)
    {
        username = convertingToLowercase(username);
        if(usernameWithId.find(username) == usernameWithId.end())
        {
            cout << "Error : User doesn't exists. " << endl;
            return;
        }

        int id = usernameWithId[username];
        users[id].posts.insertPosts(content);
    }

    void outputPosts(string username, int n)
    {
        username = convertingToLowercase(username);
        if(usernameWithId.find(username) == usernameWithId.end())
        {
            cout << "Error : User doesn't exists. " << endl;
            return;
        }

        int id = usernameWithId[username];
        vector<string> posts = users[id].posts.getRecentPosts(n);
        for (auto &p : posts)
            cout << p << endl;
    }

    void run()
    {
        string cmd1;
        while (cin >> cmd1)
        {
            if (cmd1 == "ADD")
            {
                string cmd2; cin >> cmd2;
                if (cmd2 == "USER")
                {
                    string username; cin >> username;
                    addUser(username);
                }
                else if (cmd2 == "FRIEND")
                {
                    string u1, u2; cin >> u1 >> u2;
                    addFriend(u1, u2);
                }
                else if (cmd2 == "POST")
                {
                    string username; cin >> username;
                    string rest;
                    getline(cin, rest);              
                    size_t first = rest.find('"');
                    size_t last = rest.rfind('"');
                    string content = rest.substr(first + 1, last - first - 1);
                    addPost(username, content);
                }
            }
            else if (cmd1 == "LIST")
            {
                string cmd2; cin >> cmd2;             
                string username; cin >> username;
                listFriends(username);
            }
            else if (cmd1 == "SUGGEST")
            {
                string cmd2; cin >> cmd2;            
                string username; int n;
                cin >> username >> n;
                suggestFriends(username, n);
            }
            else if (cmd1 == "DEGREES")
            {
                string of, separation;
                cin >> of >> separation;             
                string u1, u2;
                cin >> u1 >> u2;
                cout << degreesOfSeparation(u1, u2) << endl;
            }
            else if (cmd1 == "OUTPUT")
            {
                string cmd2; cin >> cmd2;             
                string username; int n;
                cin >> username >> n;
                outputPosts(username, n);
            }
        }
    }
};

#endif