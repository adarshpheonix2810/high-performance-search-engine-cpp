#include "Listnode.hpp"

listnode::~listnode()
{
    if(next!=NULL)
        delete(next);
}
void listnode::add(int did)
{
    if(did==id)
        times++;
    else
    {
        if(next==NULL)
            next=new listnode(did);
        next->add(did);
    }
    
}
int listnode::search(int did)
{
    if(did==id)
        return times;
    else
    {
        if(next==NULL)
            return 0;
        return next->search(did);
    }
}