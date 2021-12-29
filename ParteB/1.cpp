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
vector<int> residuals2;
vector<int> samples;
map <double, int> histo_r;
map <double, int> histo_s;
int numChannels;
int numSamplesPerChannel;
int bitsToDrop;
double probR[65536];
double probS[65536];
double entropyR = 0;
double entropyS = 0;


void encode(int m, char * residFile, vector<int> r){
    Golomb golomb(m,residFile);
    golomb.openBsw();
    for(int i = 0; i < r.size(); i++){
        golomb.encode(r[i]);
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

void audioPredict(char * fileName) {

    AudioFile<double> audio;
    audio.load(fileName);
    numChannels = audio.getNumChannels();
    numSamplesPerChannel = audio.getNumSamplesPerChannel();
    int sample, aux1 = 0, aux2 = 0, aux3 = 0, pred = 0;
    int resid = 0;
    bool first = 1, second = 1, third = 1;

    for (int i=-32768; i < 32768;i++){
        histo_r[i]=0;
        histo_s[i]=0;
    }

    for(int c = 0; c < numChannels; c++) {
        for(int s = 0; s < numSamplesPerChannel; s++) {
            sample = audio.samples[c][s] * 32768;
            samples.push_back(sample);

            for(int j = 0; j < bitsToDrop; j++)     //
			{                                       // Residual
				sample = sample / 2;                // Quantization 
			}                                       // 
			
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
            
            if (histo_r.find(resid)== histo_r.end()){
                histo_r[resid]=1;
            } else{
                histo_r[resid]++;
            }

            if (histo_s.find(sample)== histo_s.end()){
                histo_s[sample]=1;
            } else{
                histo_s[sample]++;
            }
        }
    }
    
    for (int i=0; i < 65536;i++){
        if (histo_r[i-3278] != 0)
            probR[i] = histo_r[i-3278]*1.0/numSamplesPerChannel;
        else
            probR[i] = 0;
        if (histo_s[i-3278] != 0)
            probS[i] = histo_s[i-3278]*1.0/numSamplesPerChannel;
        else
            probS[i] = 0;
    }
     
    for (int i=0;i<65536;i++){
        if (probR[i] != 0)
            entropyR = entropyR + (probR[i]*log(probR[i]/log(2)));
        if (probS[i] != 0)
            entropyS = entropyS + (probS[i]*log(probS[i]/log(2)));
    }

    std:: ofstream ofsR("hist_data_r.txt");
    for(std::map<double,int>::iterator it = histo_r.begin(); it != histo_r.end(); ++it) {
        ofsR << it->first << "=>" << it->second << '\n';
    }
    ofsR.close();

    std:: ofstream ofsS("hist_data_s.txt");
    for(std::map<double,int>::iterator it = histo_s.begin(); it != histo_s.end(); ++it) {
        ofsS << it->first << "=>" << it->second << '\n';
    }
    ofsS.close();
}

void audioPredictor2(){

    int sample,resid = 0,pred = 0;

    for(int i = 0; i < samples.size(); i++){

        sample = samples[i];

        for(int j = 0; j < bitsToDrop; j++)
        {                                       
            sample = sample / 2;                
        }                                       

        resid = sample - pred;
        pred = sample;
        residuals2.push_back(resid);
    }
}

void audioDespredictor(vector<int> decodedValues, vector<int> &finalResult){
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

        for(int k = 0; k < bitsToDrop; k++)     
        {                                       
            originalVal = originalVal * 2;                
        } 

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

    string file1 = "resid.bits";
    string file2 = "resid2.bits";
    char bitsFile1[file1.length() + 1];
    char bitsFile2[file2.length() + 1];
    strcpy(bitsFile1,file1.c_str());
    strcpy(bitsFile2,file2.c_str());

    std::ofstream clear_f;
    clear_f.open(bitsFile1, std::ofstream::out | std::ofstream::trunc);
    clear_f.close();
    clear_f.open(bitsFile2, std::ofstream::out | std::ofstream::trunc);
    clear_f.close();

    int m = atoi(argv[3]);
    if(argc > 4){
        bitsToDrop = atoi(argv[4]);
    } else {
        bitsToDrop = 0;
    }

    audioPredict(argv[1]);
    encode(m,bitsFile1,residuals);
    vector<int> decodedValues;
    decode(m,bitsFile1,decodedValues);
    vector<int> finalResult;
    audioDespredictor(decodedValues,finalResult);
    saveToWavFile(finalResult, argv[2]);
    
    audioPredictor2();
    encode(m,bitsFile2,residuals2);

    printf("Entropy of residuals in file %s: %f \n",argv[1],-entropyR);
    printf("Entropy of samples in file  %s: %f \n",argv[1],-entropyS);


    ifstream size_o(argv[1], ios::binary);
    size_o.seekg(0, ios::end);
    int original_size = size_o.tellg();
    cout << "Size of the original file: " << original_size << " bytes\n";
    size_o.close();
    ifstream size_f1(bitsFile1, ios::binary);
    size_f1.seekg(0, ios::end);
    int file1_size = size_f1.tellg();
    cout << "Size of the result file of first predictor: " << file1_size << " bytes\n";
    size_f1.close();
    ifstream size_f2(bitsFile2, ios::binary);
    size_f2.seekg(0, ios::end);
    int file2_size = size_f2.tellg();
    cout << "Size of the result file of second predictor: " << file2_size << " bytes\n";
    size_f2.close();
}