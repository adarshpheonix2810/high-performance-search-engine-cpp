#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Listnode.hpp"

#ifndef TRIE_HPP
#define TRIE_HPP
using namespace std;
class TrieNode
{
    char value;
    TrieNode *sibling;
    TrieNode *child;
    listnode* list;
public:
    TrieNode();
    ~TrieNode();
    void insert(char* token, int id);
    int tfsearchword(int id, char* word, int curr, int wordlen);
};

#endif