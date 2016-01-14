/*
 This file is part of CIGAL.
 CIGAL version 0.1, revised jan 2015  from Randall Balestriero
 randallbalestriero@gmail.com
 CIGAL is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 CIGAL is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with CIGAL.  If not, see <http://www.gnu.org/licenses/>.
 
 We thank SABIOD.ORG project MI CNRS MASTODONS who supported this work,
 Pr. Hervé Glotin for his support, Vincent LOSTA... and Pr Stéphane
 Mallat for their precious discussions and help on SCANET from DATA ENS
 Paris.*/


#ifndef SPECTROGRAM_HEADER_FILE
#define SPECTROGRAM_HEADER_FILE

#include "./wav.h"
#include <fftw3.h>
#include <armadillo>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//                                                                              //
//                       SPECTROGRAM PART                                       //
//                                                                              //
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//                       




///////////////////////////////////////////////////////
//				WINDOW FUNCTIONS					///
///////////////////////////////////////////////////////

inline void hamming(arma::vec & array){
    float Coeff = 2*M_PI/(array.n_elem-1);
    for(unsigned int i=0;i<array.n_elem;++i){
        array[i]=0.54-0.46*cos(Coeff*i);
    }
}
inline void hanning(arma::vec & array){
    float Coeff=2*M_PI/(array.n_elem-1);
    for(unsigned int i=0;i<array.n_elem;++i){
        array[i]=0.5-0.5*cos(Coeff*i);
    }
}
inline void triangular(arma::vec & array){
    int Coeff=(array.n_elem-1)/2;
    for(int i=0;i<Coeff;++i){
        array[i]=i*1./array.n_elem;
        array[Coeff-i-1]=i*1./array.n_elem;
    }
}
inline void hann_poisson(arma::vec & array,int alpha=2){
    for(unsigned int i=0;i<array.n_elem;++i){
        array[i]=exp(-alpha*abs(array.n_elem-1-2*i)/(array.n_elem-1))*(1-cos(2*M_PI*i/(array.n_elem-1)))/2;
    }
}


class spectrogram{
    public:
    arma::mat _Coefficients;
    int _window;
    float _overlapp;
    spectrogram(){}
    spectrogram(std::string const& Name,const int window=512,const float overlap=0.5,void(*WindowFunction)(arma::vec &)=hamming);
    double & operator()(const std::size_t i,const std::size_t j);
    double const& operator()(const std::size_t i,const std::size_t j)const;
    ~spectrogram(){}//default destructor

    //PERFORM METHOD WHICH CAN BE CALLED AFTER THE DECLARATION AND MULTIPLE TIMES FOR THE SAME VARIABLE
    void Perform(arma::vec const& signal,const int window=512,const float overlap=0.5,void(*WindowFunction)(arma::vec &)=hamming);
};

#endif

