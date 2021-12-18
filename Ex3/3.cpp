#include <iostream>
#include <fstream>
#include "Golomb.hpp"
#include <inttypes.h>
#include "../Ex1/BitStream.hpp"


int main(int argc,char** argv){
    int t = -8;
    printf("t = %d\n",t);
    Golomb g(5,argv[1]);

    g.encode(t);
    t = g.decode();
    printf("%" PRIu8 "\n",t);
    printf("----------------------\n");
    
}