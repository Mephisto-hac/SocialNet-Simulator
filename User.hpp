#ifndef USER_HPP
#define USER_HPP

#include <string>
#include "AVLTree.hpp"
using namespace std;

class User
{
public:
    string username;  
    AVLTree posts;

    User(string username)
    {
        this->username = username;
    }
};

#endif