#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "Score.hpp"
#include "Map.hpp"
#include "Trie.hpp"
#include "Maxheap.hpp"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

using namespace std;

// Function declarations
void search(char* token, TrieNode *trie, Mymap *map, int k);
void df(TrieNode* trie);
int tf(char* token, TrieNode* trie);

