#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Listnode.hpp"
#include "Score.hpp"
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
    int dfsearchword(char* word, int curr, int wordlen);
    int tfsearchword(int id, char* word, int curr, int wordlen);
    // void searchall(char* buffer, int curr);  // Disabled: memory corruption issue
    void search(char* word, int curr, Scorelist* scorelist);
};

#endif