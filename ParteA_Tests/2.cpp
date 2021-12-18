#include "../ParteA_Classes/BitStream.hpp"

int main(int argc, char** argv) {
    BitStream bs(argv[1],'w');

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

    BitStream bs2(argv[1],'r');
    std::cout << bs2.readNBits(8);
    bs2.close();

    
}