#include <fstream>
#include <iostream>

/// This is the brief description for this class.
///
/// This is the detailed description. More stuff here.

/// ### Example
/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
/// char* x = "input.txt";
/// char y = "w";
/// BitStream* g = new BitStream(x,y);
/// ~~~~~~~~~~~~~~~~~~~~~~
class BitStream{
    public:
        /// Function detail
        /// @param fileName description
        /// @param m description
        BitStream(const char* fileName, char m);

        /// Function detail
        /// @returns description of the return value
        int readBit();

        /// Function detail
        /// @param bits description
        /// @returns description of the return value
        int readNBits(int bits);

        /// Function detail
        /// @param bit description
        void writeBit(bool bit);

        /// Function detail
        /// @param number description
        /// @param bits description
        void writeNBits(int number,int bits);

        /// Function detail
        void close();
    
    private:
        /// Atribute detail
        char buf;
        /// Atribute detail
        int pos;
        /// Atribute detail
        char mode;
        /// Atribute detail
        std::fstream file; 
};