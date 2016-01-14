#include "source/CIGAL.h"

#include <armadillo>
#include <string>
#include <vector>
/*
int main(int argc, char ** argv){
    int _T=4,_Q=8,_J=7;

    WAV wav(argv[argc-1]);//open wav
    std::vector<arma::vec> M(1);
    M[0]=wav._Values;

    ScatteringLayer scatt(M,_Q,_T,_J);//perform scattering network
    std::string s(argv[argc-1]);
    s+="4_16_5_scattering_layer_0.txt";
    scatt._U.save(s,arma::raw_ascii);
    return 0;
}

*/
