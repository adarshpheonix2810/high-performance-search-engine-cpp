#include <iostream>
#include <cstdlib>
#include "Score.hpp"
#include <cstring>
#ifndef LISTNODE_HPP
#define LISTNODE_HPP
class listnode
{
    listnode *next; //next list item
    int id;         //document id
    int times;      //how many times to the specific doc
    public:
        listnode(int docId):id(docId),times(1){next=NULL;}
        ~listnode();
        void add(int docId);
        int search(int docId);
        int volume();
        int passdocuments(Scorelist* scorelist);
};
#endif