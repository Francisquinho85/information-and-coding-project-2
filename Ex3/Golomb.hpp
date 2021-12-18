class Golomb{
    public:
        int m;
        int q;
        int r;
        char* filename;
        Golomb(int m,char* filename);
        void encode(int n);
        int decode();
};
