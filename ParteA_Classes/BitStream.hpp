#include <fstream>
#include <iostream>

/// A class to read/write bits from/to a file.

/// ### Example
/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
/// char* x = "input.txt";
/// char y = "w";
/// BitStream* g = new BitStream(x,y);
/// ~~~~~~~~~~~~~~~~~~~~~~
class BitStream{
    public:
        /// Constructor of BitStream class
        /// @param fileName Name of the file to be read or written where the bits are stored.
        /// @param m Char that will indicate if the file is to be read("r") or written("w").
        BitStream(const char* fileName, char m);

        /// Read a single bit from a file.
        /// @returns Returns the bit that was read from the file.
        int readBit();

        /// Read N bits from a file.
        /// @param bits description
        /// @returns description of the return value
        int readNBits(int bits);

        /// Write a single bit to a file.
        /// @param bit Input bit to be written to the file.
        void writeBit(bool bit);

        /// Write N bits to a file.
        /// @param number Input int to be written to the file in bits.
        /// @param bits Number of bits to which the input number value will be written.
        void writeNBits(int number,int bits);

        /// Close the writing and reading of a file.
        void close();
    
    private:
        /// Buffer of the file content.
        char buf;
        /// Position where the bit is in the buffer.
        int pos;
        /// Char that will indicate if the file is to be read("r") or written("w").
        char mode;
        /// Name of the file to be read or written where the bits are stored.
        std::fstream file; 
};