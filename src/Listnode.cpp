#include "Listnode.hpp"

listnode::~listnode()
{
    if(next!=NULL)
        delete(next);
}
void listnode::add(int docId)
{
    if(docId==id)
        times++;
    else
    {
        if(next==NULL)
            next=new listnode(docId);
        else
            next->add(docId);
    }
    
}
int listnode::search(int docId)
{
    if(docId==id)
        return times;
    else
    {
        if(next==NULL)
            return 0;
        return next->search(docId);
    }
}

int listnode::volume()
{
    if(next != NULL) 
        return 1 + next->volume();
    else 
        return 1;
}
int listnode::passdocuments(Scorelist* scorelist){
    scorelist->insert(id);
    if(next != NULL){
        return next->passdocuments(scorelist);
    }
    return 0;
}