#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include "AudioFile.h"
#include "../ParteA_Classes/Golomb.hpp"
#include <map>

using namespace std;

//g++ 1.cpp ../ParteA_Classes/Golomb.cpp ../ParteA_Classes/BitStream.cpp -o 1 

vector<int> residuals;
map <double, int> histo_r;
int numChannels;
int numSamplesPerChannel;
double entropy;
double pR = 0;


void encode(int m, char * residFile){
    Golomb golomb(m,residFile);
    golomb.openBsw();
    for(int i = 0; i < residuals.size(); i++){
        golomb.encode(residuals[i]);
    }
    golomb.closeBsw();
}

void decode(int m, char * residFile, vector<int> &decodedValues){
    Golomb golomb(m,residFile);
    golomb.openBsr();
    for(int i = 0; i < residuals.size(); i++){
        int dec = golomb.decode();
        decodedValues.push_back(dec);
    }
    golomb.closeBsr();
}

void audioPredict(char * fileName, char * dropBits) {

    AudioFile<double> audio;
    audio.load(fileName);
    numChannels = audio.getNumChannels();
    numSamplesPerChannel = audio.getNumSamplesPerChannel();
    int sample, aux1 = 0, aux2 = 0, aux3 = 0, pred = 0;
    int resid = 0;
    bool first = 1, second = 1, third = 1;
    int bitsToDrop = atoi(dropBits);

    for(int c = 0; c < numChannels; c++) {
        for(int s = 0; s < numSamplesPerChannel; s++) {
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

            /*for(int j = 0; j < bitsToDrop; j++)     //
			{                                       //
				resid = resid / 2;                  //
			}                                       // Residual
			for(int k = 0; k < bitsToDrop; k++)     // Quantization
			{                                       //
				resid = resid * 2;                  //
			}                                       //*/

            aux3 = aux2;
            aux2 = aux1;
            aux1 = sample;
            residuals.push_back(resid);

            if (histo_r.find(resid)!= histo_r.end()){
                histo_r[resid]++;
            }else{
                histo_r[resid]=1;
            }
        }
    }
    std:: ofstream ofsC1("hist_data.txt");
    for(std::map<double,int>::iterator it = histo_r.begin(); it != histo_r.end(); ++it) {
        pR = (double)it->second/(numChannels*numSamplesPerChannel);
        if(pR > 0)
            entropy -= (log(pR)/log(16)) *pR;
        ofsC1 << it->first << "=>" << it->second << '\n';
    }
    ofsC1.close();
    
}

void audioDespredictor(vector<int> decodedValues,vector<int> &finalResult){
    int originalVal = 0, aux1 = 0, aux2 = 0, aux3 = 0, decoded = 0, pred = 0;
    bool first = 1, second = 1, third = 1;
    for (int i = 0; i < decodedValues.size(); i++)
    {
        decoded = decodedValues[i];
        
        if(!third) {
            pred = (3*aux1) - (3*aux2) + aux3; // pred = 3x[n-1] - 3x[n-2] + x[n-3]
            originalVal = pred + decoded;
        } else if(!second) {
            pred = 2*aux1 - aux2;
            originalVal = pred + decoded;
            third = 0;
        } else if(!first){
            pred = aux1;
            originalVal = pred + decoded;
            second = 0;
        } else {
            pred = 0;
            originalVal = decoded;
            first = 0;
        }
        aux3 = aux2;
        aux2 = aux1;
        aux1 = originalVal;
        finalResult.push_back(originalVal);
    }
}

void saveToWavFile(vector<int> finalResult, char * outFileName){

    AudioFile<double> audioOut;
    audioOut.setNumChannels(numChannels);
    audioOut.setNumSamplesPerChannel(numSamplesPerChannel);
    
    for(int c = 0; c < numChannels; c++) {
        for(int s = 0; s < numSamplesPerChannel; s++){
            if(c == 0){
                audioOut.samples[c][s] = (double) finalResult[s] / 32768.0;
            } else {
                audioOut.samples[c][s] = (double) finalResult[s+numSamplesPerChannel] / 32768.0;
            }
        }
    }
    audioOut.save(outFileName, AudioFileFormat::Wave);
}

int main(int argc, char** argv){

    audioPredict(argv[1],argv[4]);
    encode(6,argv[2]);
    vector<int> decodedValues;
    decode(6,argv[2],decodedValues);
    vector<int> finalResult;
    audioDespredictor(decodedValues,finalResult);
    saveToWavFile(finalResult, argv[3]);
    printf("Entropy of file %s: %f \n",argv[1],entropy);

}