#include "../Ex1/BitStream.hpp"

int main() {
    BitStream bs("file.txt",'w');

    bs.writeNBits(100,8);

    /* bs.writeBit(0);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(0);
    bs.writeBit(0);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(1);
    bs.writeBit(1); */
    bs.close();

    BitStream bs2("file.txt",'r');
    std::cout << bs2.readNBits(8) << "\n";
    bs2.close();

    
}