#include "Search.hpp"
using namespace std;

void search(char* token, TrieNode *trie, Mymap *map, int k){
    cout<<"not implemented yet"<<endl;
}

void df(TrieNode* trie){
    cout<<"not implemented yet"<<endl;
}

int tf(char* token, TrieNode* trie){
    // Get document ID
    char* token2 = strtok(NULL, " \t\n");
    if(token2 == NULL){
        cout<<"Error: Missing document ID. Usage: /tf <doc_id> <word>"<<endl;
        return -1;
    }
    
    // Validate document ID is a number
    int len = strlen(token2);
    for(int i=0; i<len; i++){
        if(!isdigit(token2[i])){
            cout<<"Error: Document ID must be a number"<<endl;
            return -1;
        }
    }
    int id = atoi(token2);
    
    // Get the word to search
    token2 = strtok(NULL, " \t\n");
    if(token2 == NULL){
        cout<<"Error: Missing word. Usage: /tf <doc_id> <word>"<<endl;
        return -1;
    }
    
    // Search for the word and get frequency
    int wordlen = strlen(token2);
    int frequency = trie->tfsearchword(id, token2, 0, wordlen);
    
    // Display result with clear message
    if(frequency == 0){
        cout<<"Term '"<<token2<<"' not found in document "<<id<<endl;
    } else {
        cout<<"Term '"<<token2<<"' appears "<<frequency<<" time(s) in document "<<id<<endl;
    }
    
    return 0;
}