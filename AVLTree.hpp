#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <string>
#include <ctime>
#include <vector>
using namespace std;

struct AVLNode 
{
    string content;
    time_t timestamp;
    int height;
    AVLNode* left;
    AVLNode* right;
    
    AVLNode(string content) 
    {
        this->content = content;
        timestamp = time(0);
        height = 1;
        left = right = nullptr;
    }

};

class AVLTree
{
    AVLNode* root;

int Height(AVLNode* p)
{
    int hl,hr;
    hl = p && p->left ? p->left->height : 0;
    hr = p && p->right ? p->right->height : 0;
    return max(hl, hr) + 1;
}

int BalanceFactor(AVLNode* p)
{
    int hl,hr;
    hl = p && p->left ? p->left->height : 0;
    hr = p && p->right ? p->right->height : 0;
    return hl - hr;
}

AVLNode* LLRotation(AVLNode* p)
{
    AVLNode* pl =p->left;
    AVLNode* plr = pl->right;
    p->left = plr;
    pl->right = p;
    p->height = Height(p);
    pl->height = Height(pl);
    return pl;
}

AVLNode* LRRotation(AVLNode* p)
{
    AVLNode* pl =p->left;
    AVLNode* plr = pl->right;
    p->left = plr->right;
    pl->right = plr->left;
    plr->right = p;
    plr->left = pl;
    p->height = Height(p);
    pl->height = Height(pl);
    plr->height = Height(plr);
    return plr;
}

AVLNode* RRRotation(AVLNode* p)
{
    AVLNode* pr =p->right;
    AVLNode* prl = pr->left;
    p->right = prl;
    pr->left = p;
    p->height = Height(p);
    pr->height = Height(pr);
    return pr;
}

AVLNode* RLRotation(AVLNode* p)
{
    AVLNode* pr =p->right;
    AVLNode* prl = pr->left;
    p->right = prl->left;
    pr->left = prl->right;
    prl->left = p;
    prl->right = pr;
    p->height = Height(p);
    pr->height = Height(pr);
    prl->height = Height(prl);
    return prl;
}

void reverseInorder (AVLNode* p, int &count, int N, vector<string> &result)
{
    if(count >= N && N!=-1) return;
    if(p)
    {
        reverseInorder(p->right, count, N, result);
        if(count < N || N==-1) 
        {
        result.push_back(p->content);
        count++;
        }
        reverseInorder(p->left, count, N, result);
    }
}

public:
    AVLTree() {root = NULL;} 

AVLNode* Insert(AVLNode* p, string content)
{
    if(p==NULL)
        {
            AVLNode* t=new AVLNode(content);
            return t;
        }

    time_t currentTime = time(0);
    if(p->timestamp <= currentTime){
        p->right=Insert(p->right,content);
    }
    else{
        p->left=Insert(p->left,content);
    }

    p->height=Height(p);

    if(BalanceFactor(p)==2 && BalanceFactor(p->left)>=0)
        return LLRotation(p);
    else if(BalanceFactor(p)==2 && BalanceFactor(p->left)==-1)
        return LRRotation(p);
    else if(BalanceFactor(p)==-2 && BalanceFactor(p->right)<=0)
        return RRRotation(p);
    else if(BalanceFactor(p)==-2 && BalanceFactor(p->right)==1)
        return RLRotation(p);

    return p;
}

void insertPosts(string content) {
    root = Insert(root, content);
}

vector<string> getRecentPosts(int N)
{
    vector<string> result;
    int count=0;
    reverseInorder(root, count, N, result);
    return result;
}


};

#endif