#include <iostream>
#include <cstdlib>
#include <cstring>
#ifndef TRIE_HPP
#define TRIE_HPP
using namespace std;
class TrieNode
{
    char value;
    TrieNode *sibling;
    TrieNode *child;
    //listnode* list;
public:
    TrieNode();
    ~TrieNode();
};

#endif