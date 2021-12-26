#include <iostream>
#include <fstream>
#include "../ParteA_Classes/Golomb.hpp"
#include <inttypes.h>
#include "../ParteA_Classes/BitStream.hpp"


int main(int argc,char** argv){
    int t = 100;
    printf("t = %d\n",t);
    Golomb g(6,argv[1]);
    g.openBsw();
    g.encode(t);
    g.encode(20);
    g.closeBsw();
    t = g.decode();
    printf("%d \n",t);
    t = g.decode();
    printf("%d \n",t);
    printf("----------------------\n");   
}