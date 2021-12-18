#include <math.h> 
#include "Golomb.hpp"
#include "../Ex1/BitStream.cpp"
#include <iostream>
#include <string> 
using namespace std;

Golomb::Golomb(int m,char* filename){
    this->filename = filename;
    this->m = m;
}

void Golomb::encode(int n){
    
    int q = n/(this->m);
    //this->q = q;
    int r = n-q*(this->m);

    // write in BitStream
    BitStream bs(this->filename,'w');
    for(int i=0;i < q;i++){
        bs.writeBit(1);

    }
    bs.writeBit(0);
    double log = log2(this->m);
    int b = ceil(log);
    int aux = pow(2,b)-m;
    int res = r + aux;
    if(r < aux){
        bs.writeNBits(r,b-1);
    }
    else bs.writeNBits(res,b);
    bs.close();     
}

int Golomb::decode(){
    BitStream bs(this->filename,'r');
    while(bs.readBit()==1){
        q++;
    }

    double log = log2(this->m);
    int b = ceil(log);
    int aux = pow(2,b)-m;
    int res = 0;
    for (int i = 0; i < b-1; i++)
    {
        res = (res << 1) | bs.readBit();
    }
    bs.close();
    if (res < aux) return q*m+res;
    else  res = (res << 1) | bs.readBit();
    return (q * m) + (res - aux);
}
