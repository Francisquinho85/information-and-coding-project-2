#include <iostream>
#include <fstream>
#include <string>

class BitStream {
    private:
        char buf;
        int pos;
        char mode;

    public:
        ifstream file;        
        BitStream(string fileName, char m){
            mode = m;
            file.open(fileName, ios::in | ios::out);
        }

        char readBit(){
            char bit;
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

        void readNBits(int n){
            
        }

        void writeBit(char bit) {
            if(pos == 8) {
                file.put(buf);
                pos = 0;
                buf = 0;
            }
            buf = (buf << 1) | bit;
            pos++;
        }
        
        void writeNBits(int n){

        }

        void close() {
            if(mode == "w" && pos != 0) {
                buf = buf << (8 - pos);
                file.put(buf);
            }
            file.close();
        }
};