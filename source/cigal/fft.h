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

#ifndef _FFT_HEADER_FILE
#define _FFT_HEADER_FILE
#include <cmath>
#include <complex>
#include <fstream>
#include <armadillo>

using namespace std;

inline complex<double> W(double const& coeff){
    complex<double> b(std::cos(coeff),-std::sin(coeff));
    return b;
}


class fft{
    public:
        arma::cx_vec _Coefficients;
        fft(){}
        fft(arma::vec const& signal,const bool pad=1){
            ComputeFFT(signal,pad);
        }
        template<typename T>
         void Set_Coefficients(T const& signal,bool pad=1);
         void PerformBitReversal();
         void PerformDecomposition();
         void ComputeFFT(arma::vec const& signal,const bool pad=1);
         void ComputeIFFT(arma::cx_vec const& signal,const bool pad=1);
        complex<double> & operator[](int const i){return _Coefficients[i];}
        complex<double> const& operator[](int const i)const{return _Coefficients[i];}
        ~fft(){}
};


// SET COEFFICIENTS METHOD WHICH WILL INITiALIZE ALL THE VARIABLES FOR THE PERFORM METHOD
// THIS MEANS PADDING IF NECESSARY
template<typename T>
 void fft::Set_Coefficients(T const& signal,bool pad){
    if(pad){
        int k=1;
        while(pad){//search the number of 0 to add
            k<<=1;
            if(signal.n_elem<=k)
            pad=0;
        }
        _Coefficients=arma::cx_vec(k);//allocate memory
        for(int i=0;i<signal.n_elem;++i) _Coefficients[i]=signal[i];//copy data
        for(int i=signal.n_elem;i<k;++i) _Coefficients[i]=0;//padded part
    }
    else{//if no padding direct copy
        _Coefficients=arma::cx_vec (signal.n_elem);
        for(int i=0;i<_Coefficients.n_elem;++i)
        _Coefficients[i]=signal[i];
    }
}
void InplaceFFT(arma::cx_vec & _Coefficients);

void InplaceIFFT(arma::cx_vec & _Coefficients);


#endif
