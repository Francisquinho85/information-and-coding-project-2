#include <math.h> 
#include "../Ex1/BitStream.cpp"

class Golomb{
    public:
    uint32_t m;
    uint32_t q;
    uint32_t r;
    BitStream bs;
    Golomb(uint32_t m){
        this->m = m;
    }
    ~Golomb(){}
    void encode(uint32_t n);
    uint32_t decode(uint32_t t);
};
    
void Golomb::encode(uint32_t n){

    uint32_t q = n/(this->m);
    // write in BitStream
    for(int i=0;i < q;i++){
        bs.writeBit(0);
    }
    bs.writeBit(1);
    bs.writeNBits(r,ceil(log2(this->m)));

    this->q = q;
    this->r = n-q*(this->m);

    
}
uint32_t Golomb::decode(uint32_t t){
    // int cnt=0;
    // int i;
    // for(i=0;i < sizeof(t)*8;i++){
    //     if (this->bs.readBit(this->q,i))
    //     cnt++;
    // }
    // return cnt*(this->m)+(this->r);
    
    
    
}
