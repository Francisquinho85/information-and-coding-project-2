#include <math.h> 
#include "Golomb.hpp"
#include "../Ex1/BitStream.cpp"
#include <iostream>
#include <string> 

Golomb::Golomb(u_int32_t m,char* filename){
    this->filename = filename;
    this->m = m;
}

void Golomb::encode(u_int32_t n){
    
    u_int32_t q = n/(this->m);
    this->q = q;
    this->r = n-q*(this->m);

    // write in BitStream
    BitStream bs(this->filename,'w');
    for(int i=0;i < q;i++){
        bs.writeBit(1);

    }
    bs.writeBit(0);
    double log = log2(this->m);
    int b = ceil(log);
    int res = r + pow(2,b)-m;
    bs.writeNBits(res,b);
    bs.close();     
}

u_int32_t Golomb::decode(){
    BitStream bs(this->filename,'r');
    std::string a= "0";
    while(bs.readBit()==1){
        q++;
        a= "1"+a;
    }
    
    double log = log2(this->m);
    int b = ceil(log);
    int res = pow(2,b)-m;
    for (int i = 0; i < b; i++)
    {
        a = a + std::to_string(bs.readBit());
    }
    std::cout << a<<"\n";

    int result = stoi(a);
    bs.close();
    return result;

}
