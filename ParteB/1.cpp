#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include "AudioFile.h"

using namespace std;

int main(int argc, char** argv){

    ofstream sampleOfstream("samp.txt");
    ofstream errorOfstream("resid.txt");
    AudioFile<double> audio;
    audio.load(argv[1]);
    int numChannels = audio.getNumChannels();
    int numSamples = audio.getNumSamplesPerChannel();
    double sample;
    double aux1 = 0, aux2 = 0, aux3, pred = 0;
    int resid = 0;
    bool first = 1, second = 1, third = 1;
    double sampleEntropy, errorEntropy;

    for(int c = 0; c < numChannels; c++) {
        for(int s = 0; s < numSamples; s++) {
            sample = audio.samples[c][s] * 32768;
            if(!third) {
                pred = 3*aux1 - 3*aux2 + aux3;
                resid = sample - pred;
            } else if(!second) {
                pred = 2*aux1 - aux2;
                resid = sample - pred; // r[n] = x[n] - x'[n]
                third = 0;
            } else if(!first){
                pred = aux1;
                resid = sample - pred;
                second = 0;
            } else {
                pred = 0;
                resid = sample;
                first = 0;
            }
            //sampleOfstream << sample << "\n";
            //errorOfstream << resid << "\n";
            cout << sample << "\t";
            cout << pred << "\t";
            cout << resid << "\n";
            aux3 = aux2;
            aux2 = aux1;
            aux1 = sample;
        }
    }
}