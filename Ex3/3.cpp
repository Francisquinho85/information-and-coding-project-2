#include <iostream>
#include <fstream>
#include "Golomb.h"
#include <inttypes.h>


int main(int argc,char* argv[]){
    uint8_t t = 28;
    printf("t = %d\n",t);
    Golomb g(6);
    g.encode(t);
    printf("r = %d\n",g.r);
    printf("q = %d\n",g.q);
    t = g.decode(t);
    printf("%" PRIu8 "\n",t);
    printf("----------------------\n");

}