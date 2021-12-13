#include <fstream>
#include <iostream>

class BitStream{
    public:
        BitStream(const char* fileName, char m);
        int readBit();
        int readNBits(int bits);
        void writeBit(bool bit);
        void writeNBits(int number,int bits);
        void close();
    
    private:
        char buf;
        int pos;
        char mode;
        std::fstream file;
};