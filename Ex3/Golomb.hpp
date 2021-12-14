class Golomb{
    public:
        u_int32_t m;
        u_int32_t q;
        u_int32_t r;
        char* filename;
        Golomb(u_int32_t m,char* filename);
        void encode(u_int32_t n);
        u_int32_t decode();
};
