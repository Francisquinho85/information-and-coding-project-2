#include "../Ex1/BitStream.cpp"

int main() {
    BitStream bs("file.bits","w");

    bs.writeBit('0');
    bs.writeBit('0');
    bs.writeBit('1');
    bs.writeBit('0');
    bs.writeBit('0');
    bs.writeBit('0');
    bs.writeBit('1');
    bs.writeBit('1');
}