#include "Trie.hpp"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

TrieNode::TrieNode():value(-1), sibling(nullptr), child(nullptr)
{
    list = nullptr;
};

TrieNode::~TrieNode()
{
    if(list!=nullptr){
        delete list;
    }
    if(sibling!=nullptr){
        delete sibling;
    }
    if(child!=nullptr){
        delete child;
    }

};
void TrieNode::insert(char* token, int id){
    if(value ==-1 || value ==token[0]){
        value = token[0];
        if(strlen(token)==1){
            if(list==nullptr)
                list=new listnode(id);
            list->add(id);
        }
        else{
            if(child == nullptr){
                child = new TrieNode();
            }
            child->insert(token+1,id);
        }
    }
    else{
        if(sibling==nullptr){
            sibling=new TrieNode();
        }
        sibling->insert(token,id);
    }
    return ;
}
int TrieNode::dfsearchword(char* word, int curr, int wordlen){
    if(word[curr]==value){
        if(curr==wordlen-1){
            if(list!=NULL){
                return list->volume();
            }
            else{
                return 0;
            }
        }else{
            if(child!=NULL){
                return child->dfsearchword(word, curr+1, wordlen);
            }else{
                return 0;
            }
        }
    }
    else{
        if(sibling!=NULL){
            return sibling->dfsearchword(word, curr, wordlen);
        }else{
            return 0;
        }
    }
};
int TrieNode::tfsearchword(int id, char* word, int curr, int wordlen){
    if(word[curr]==value){
        if(curr==wordlen-1){
            if(list!=NULL){
                return list->search(id);
            }else{
                return 0;
            }
        }
        else{
            if(child!=NULL){
                return child->tfsearchword(id, word, curr+1, wordlen);
            }else{
                return 0;
            }
        }
    }
    else{
        if(sibling!=NULL){
            return sibling->tfsearchword(id, word, curr, wordlen);
        }else{
            return 0;
        }
    }
}