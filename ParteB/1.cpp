#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include "AudioFile.h"
#include "../ParteA_Classes/Golomb.hpp"

using namespace std;

//g++ 1.cpp ../ParteA_Classes/Golomb.cpp ../ParteA_Classes/BitStream.cpp -o 1 

vector<double> residuals;

void encode(int m, char * residFile){
    Golomb golomb(m,residFile);
    golomb.openBsw();
    for(int i = 0; i < residuals.size(); i++){
        golomb.encode(residuals[i]);
    }
    golomb.closeBsw();
}

void decode(int m, char * residFile, vector<double> decodedValues){
    Golomb golomb(m,residFile);
    golomb.openBsr();
    for(int i = 0; i < residuals.size(); i++){
        char dec = golomb.decode();
        decodedValues.push_back(dec);
    }
    golomb.closeBsr();
}

void audioPredict(char * fileName) {

    AudioFile<double> audio;
    audio.load(fileName);
    int numChannels = audio.getNumChannels();
    int numSamples = audio.getNumSamplesPerChannel();
    double sample, aux1 = 0, aux2 = 0, aux3, pred = 0;
    int resid = 0;
    bool first = 1, second = 1, third = 1;

    for(int c = 0; c < numChannels; c++) {
        for(int s = 0; s < numSamples; s++) {
            sample = audio.samples[c][s] * 32768;
            if(!third) {
                pred = 3*aux1 - 3*aux2 + aux3; // pred = 3x[n-1] - 3x[n-2] + x[n-3]
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
            aux3 = aux2;
            aux2 = aux1;
            aux1 = sample;
            residuals.push_back(resid);
        }
    }
}

void audioDespredictor(vector<double> decodedValues,vector<double> finalResult){
    double originalVal = decodedValues[0];
    double previous = 0;
    bool first = 1, second = 1, third = 1;
    for (int i = 0; i < decodedValues.size(); i++)
    {
        
    }
    
}

int main(int argc, char** argv){
    
    audioPredict(argv[1]);
    encode(6,argv[2]);
    vector<double> decodedValues;
    decode(6,argv[2],decodedValues);
    vector<double> finalResult;
    audioDespredictor(decodedValues,finalResult);
}