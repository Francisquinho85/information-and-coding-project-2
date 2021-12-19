#include "predictor.hpp"

predictor::predictor(){
    in = 0;
    n = 0;
}

short predictor::predict()
{
    if(in == 0){
        return 0;
    } else if (in == 1) {
        return x[n];
    } else if (in == 2) {
        return 2*x[n] - 2*x[n-1];
    } 
    return 3*x[n] - 3*x[n-1] + x[n-2];
}

int predictor::residual(short sample)
{
    short val = predict(); // x[n]
    updateBuffer(sample);
    return sample-val;   
}

short predictor::reconstruct(int residual)
{
    int val_res = predict() + residual;
    updateBuffer(val_res);
    return val_res;
}

void predictor::updateBufferConst(short quant)
{
    updateBuffer(predict()+quant);
}

void predictor::updateBuffer(short sample) {
    n = (n+1)&0x03;
    x[n] = sample;
    in++;
}

void predictor::showBuffer()
{
    cout << x[0] << ',' << x[1] << ',' <<  x[2] << endl;
}