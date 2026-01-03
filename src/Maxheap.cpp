#include "Maxheap.hpp"
using namespace std;

Maxheap::Maxheap(int k):
    curnumofscores(0),
    maxnumofscores(k)
{
    heap=(double*)malloc(maxnumofscores*sizeof(double));
    ids=(int*)malloc(maxnumofscores*sizeof(int));
    // Initialize IDs to -1 to mark as invalid
    for(int i=0; i<maxnumofscores; i++){
        ids[i] = -1;
        heap[i] = 0.0;
    }
}
int Maxheap::minindex(int low,int high){
    int min=low;
    double minscore=heap[low];
    for(int i=low;i<high;i++){
        if(heap[i]<minscore){
            min=i;
            minscore=heap[i];
        }
    }
    return min;
}
void Maxheap::swapscore(int index1,int index2){
    double temp=0.0;
    temp=heap[index1];
    int tempid=ids[index1];
    heap[index1]=heap[index2];
    heap[index2]=temp;
    ids[index1]=ids[index2];
    ids[index2]=tempid;

}
void Maxheap::insert(double score,int id){
    int index;
    if(curnumofscores<maxnumofscores){
        index=curnumofscores;
        curnumofscores++;
    }
    else{
        int tempindex=minindex(maxnumofscores/2,maxnumofscores);
        if(score>heap[tempindex]){
            index=tempindex;
        }
        else{
            return ;
        }
    }
    heap[index]=score;
    ids[index]=id;
    while(index > 0 && heap[index]>heap[(index-1)/2]){
        swapscore(index,(index-1)/2);
        index=(index-1)/2;
    }
}
int Maxheap::MaxChild(int number1,int number2){
    if(number1<curnumofscores && number2<curnumofscores){
        if(heap[number1]>heap[number2]){
            return number1;
        }
        else{
            return number2;
        }
    }
    else if(number1<curnumofscores){
        return number1;
    }
    else if(number2<curnumofscores){
        return number2;
    }
    else{
        return -1;
    }
    
}
double Maxheap::remove(){
    int index=0, chosenchild;
    double Returnvalue=0.0;
    if(curnumofscores>0){
        Returnvalue=heap[0];
        curnumofscores--;
        heap[0]=heap[curnumofscores];
        ids[0]=ids[curnumofscores];
        while(1){
            chosenchild=MaxChild(2*index+1,2*index+2);
            if(chosenchild!=-1 && heap[chosenchild]>heap[index]){
                swapscore(chosenchild,index);
                index=chosenchild;
            }
            else{
                break;
            }
        }
    }
    return Returnvalue;
}