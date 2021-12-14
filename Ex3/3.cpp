#include <iostream>
#include <fstream>
#include "Golomb.hpp"
#include <inttypes.h>
#include "../Ex1/BitStream.hpp"


int main(int argc,char** argv){
    uint8_t t = 28;
    printf("t = %d\n",t);
    Golomb g(6,argv[1]);
    g.encode(t);
    printf("r = %d\n",g.r);
    printf("q = %d\n",g.q);
    t = g.decode();
    printf("%" PRIu8 "\n",t);
    printf("----------------------\n");
    
}