
#include <iostream>

using namespace std;

class predictor {
    public:
        predictor();
        short predict();
        int residual(short sample);
        short reconstruct(int residual);
        void updateBufferConst(short quant);
        void showBuffer();
        void  updateBuffer(short sample);
        
    private:
        int in;
        short n;
        short x[3] = {0,0,0};
};