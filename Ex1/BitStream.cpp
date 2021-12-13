#include "BitStream.hpp"
#include <iostream>
#include <fstream>


BitStream::BitStream(const char* fileName, char m)
{
    mode = m;
    file.open(fileName);
    pos = 0;
    buf = 0;
}

int BitStream::readBit(){
    int bit;
    if(pos == 0) {
        buf = file.get();
    }
    bit = (buf >> (7-pos)) & 1;
    pos++;
    if(pos == 8) {
        pos = 0;
    }
    return bit;
}

int BitStream::readNBits(int bits){
    int number = 0;
    for(int i=0; i<bits; i++){
        std::cout << number << " ";
        number = (number << 1) | (readBit()&1);
    }
    return number;
}

void BitStream::writeBit(bool bit) {
    //std::cout << bit << " ";
    buf = ((buf << 1) | bit);
    //std::cout << static_cast<int>(buf)<< "\n";
    pos++;
    if(pos == 8) {
        file.put(buf);
        pos = 0;
        buf = 0;
    }
    
}

void BitStream::writeNBits(int number,int bits){
    int res;
    for(int i=bits-1; i>=0; i--){
        res = (number >> i) & 1;
        writeBit(res);
    }
}

void BitStream::close() {
    if(mode == 'w' && pos != 0) {
        buf = buf << (8 - pos);
        file.put(buf);
    }
    file.close();
}