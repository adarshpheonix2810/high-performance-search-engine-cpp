#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "Map.hpp"
#include "Trie.hpp"

using namespace std;

// Function declarations
void search(char* token, TrieNode *trie, Mymap *map, int k);
void df(TrieNode* trie);
int tf(char* token, TrieNode* trie);

#endif