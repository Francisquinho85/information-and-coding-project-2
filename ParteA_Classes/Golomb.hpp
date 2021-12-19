/// A class to implement Golomb Code.

/// Golomb coding is a lossless data compression method using a family of data compression codes.
/// It relies on separating an integer into two parts:
///
/// One of those parts is represented by a unary code.
/// The other part is represented using a binary code.

/// ### Example
/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
/// int x = 2;
/// char* y = "input.txt";
/// Golomb* g = new Golomb(x,y);
/// ~~~~~~~~~~~~~~~~~~~~~~

class Golomb{
    public:
        /// An integer parameter, m > 0.
        int m;
        /// The quotient, q, can have the values 0, 1, 2, . . . , and is represented by unary code. 
        int q;
        /// The remainder of the division, r, can have the values 0, 1, 2, . . . , m−1, and is represented by binary code.
        int r;
        /// Name of the file to be read or written where the bits are stored.
        char* filename;

        /// Constructor of Golomb class
        /// @param m An integer parameter, m > 0.
        /// @param filename Name of the file to be read or written where the bits are stored.
        Golomb(int m,char* filename);

        /// The function writes to a text file the golomb code associated with its input sn by the value of m passed in the constructor. The contents of the file will be the quotient of the division of sn/m in unary, followed by the remainder of that division in binary.
        /// @param sn An input value to be divided.
        void encode(int sn);

        /// The function reads from a text file the bits associated with a goal code dividing it into unary (quotient) and binary (remainder) parts decoding their value and returning the value of the divisor according to the quotient and remainder discovered. 
        /// @return Value of the decoded golomb code divisor.
        int decode();
};
