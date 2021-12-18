#include <iostream>
#include <fstream>
#include "../ParteA_Classes/Golomb.hpp"
#include <inttypes.h>
#include "../ParteA_Classes/BitStream.hpp"


int main(int argc,char** argv){
    int t = 15;
    printf("t = %d\n",t);
    Golomb g(6,argv[1]);

    g.encode(t);
    t = g.decode();
    printf("%d \n",t);
    printf("----------------------\n");   
}