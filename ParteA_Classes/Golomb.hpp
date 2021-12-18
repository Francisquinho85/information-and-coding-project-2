/// This is the brief description for this class.
///
/// This is the detailed description. More stuff here.

/// ### Example
/// ~~~~~~~~~~~~~~~~~~~~~~.cpp
/// int x = 2;
/// char* y = "input.txt";
/// Golomb* g = new Golomb(x,y);
/// ~~~~~~~~~~~~~~~~~~~~~~

class Golomb{
    public:
        /// Atribute detail
        int m;
        /// Atribute detail
        int q;
        /// Atribute detail
        int r;
        /// Atribute detail
        char* filename;

        /// Function detail
        /// @param m description
        /// @param filename description
        Golomb(int m,char* filename);

        /// Function detail
        /// @param n description
        void encode(int n);

        /// Function detail
        /// @return description of the return value
        int decode();
};
