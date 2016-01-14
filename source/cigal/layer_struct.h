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


#ifndef Layer_struct_header_file
#define Layer_struct_header_file

#include <cmath>
#include <complex>
#include <string>
#include <armadillo>
#include "filters.h"
#include <fftw3.h>


class ScatteringLayer{
public:
        FilterBank _FilterBank;
        arma::mat _Scattering;
        arma::mat _U;

        template<typename M>
        inline ScatteringLayer(std::vector<M> &, int const& Q, int const& T, const int &J=0);
        inline ScatteringLayer(){}
        template<typename T>
        inline void PerformDecomposition(std::vector<T> &);
        inline void LittlewoodPaleyNormalisation();
};


template<typename Ty>
inline ScatteringLayer::ScatteringLayer(std::vector<Ty> & input,int const& T,int const& Q,int const& J){
    //PRINT LAYER INFORMATIONS
    std::cout<<"\t\tScattering Layer :"<<std::endl;
    std::cout<<"\t\t\t\tCreating Filter Bank"<<std::endl;
    _FilterBank=FilterBank(input[0].n_elem,T,Q,J,0.000000001);
    std::cout<<"\t\t\tInput size = ("<<input[0].n_elem<<","<<input.size()<<") "<<std::endl;

    std::cout<<"\t\t\t\tDecomposition"<<std::endl;


    PerformDecomposition(input);
    std::cout<<"\t\t\t\tDecomposition Done"<<std::endl;
}



/*

inline void ScatteringLayer::LittlewoodPaleyNormalisation(){
    arma::vec sum(_FilterBank._Psis[0].n_elem);
    for(std::size_t i=0;i<_FilterBank._Psis[0].n_elem;++i)
        for(int j=0;j<_FilterBank._Psis._Q*_FilterBank._Psis._J;++j){
            sum[i]+=pow(_FilterBank._Psis[j].at(i),2);}
    //ajoute le symm
    for(std::size_t i=0;i<sum.n_elem;++i)
        sum[i]+=sum[sum.n_elem-1-i];
    //cherche le max
    float sum_max=arma::max(sum);
    for(int i=0;i<_FilterBank._Psis._Q*_FilterBank._Psis._J;++i)
    _FilterBank._Psis[i]/=sum_max;
}
*/

template<typename T>
inline void ScatteringLayer::PerformDecomposition(std::vector<T> &_Input){
    const int coeff=2;//1<<(int)std::floor(std::log2(coeff));
    const int SizeAfter=_Input[0].n_elem/coeff;
    const int SizeOfScatt=(SizeAfter)*2/_FilterBank._T;
    _Scattering=arma::mat(this->_FilterBank._Q*_FilterBank._J*_Input.size(),SizeOfScatt);
    _U         =arma::mat(this->_FilterBank._Q*_FilterBank._J*_Input.size(),SizeAfter);

    arma::cx_vec InputFFT(_Input[0].n_elem);
    arma::cx_vec high(_Input[0].n_elem);//will hold line result
    arma::cx_vec line_perio(SizeAfter);
    arma::cx_vec low(SizeOfScatt);



    fftw_plan P1=fftw_plan_dft_1d(_Input[0].n_elem,reinterpret_cast<fftw_complex*>(InputFFT.memptr()),reinterpret_cast<fftw_complex*>(InputFFT.memptr()),FFTW_FORWARD,FFTW_ESTIMATE);
    fftw_plan P2=fftw_plan_dft_1d(SizeAfter,reinterpret_cast<fftw_complex*>(line_perio.memptr()),reinterpret_cast<fftw_complex*>(line_perio.memptr()),FFTW_BACKWARD,FFTW_ESTIMATE);
    fftw_plan P3=fftw_plan_dft_1d(SizeAfter,reinterpret_cast<fftw_complex*>(line_perio.memptr()),reinterpret_cast<fftw_complex*>(line_perio.memptr()),FFTW_FORWARD,FFTW_ESTIMATE);
    fftw_plan P4=fftw_plan_dft_1d(SizeOfScatt,reinterpret_cast<fftw_complex*>(low.memptr()),reinterpret_cast<fftw_complex*>(low.memptr()),FFTW_BACKWARD,FFTW_ESTIMATE);



    for(unsigned int i=0;i<_Input.size();++i){//for each line of the input (1 for the first layer)

        for(int I=0;I<_Input[i].n_elem;++I)
            InputFFT.at(I)=_Input[i].at(I);
        fftw_execute(P1);

        //HIGH DECOMPOSITION

        for(int filter_i=0;filter_i<_FilterBank._J*_FilterBank._Q;++filter_i){//for each highpass filter

            //get the filter support
            const int start=_FilterBank[filter_i]._Start;
            const int end=_FilterBank[filter_i]._End;
            std::cout<<"Start is at "<<start<<" "<<end<<std::endl;
            high.fill(0);
            line_perio.fill(0);
            low.fill(0);

            //apply the element wise product
            for(int j=start;j<end;++j)
                high.at(j)=_FilterBank[filter_i][j-start]*InputFFT.at(j);

            //PERIODIZE
            for(int P=std::floor(start/SizeAfter);P<std::floor(end/SizeAfter)+1;++P)//here
                for(int j=0;j<SizeAfter;++j)
                        line_perio.at(j)+=high.at(P*SizeAfter+j);

            //IFFT
            fftw_execute(P2);

            for(int I=0;I<line_perio.n_elem;++I){
                line_perio.at(I)=std::abs(line_perio.at(I));
                _U.at(i+_Input.size()*filter_i,I)=std::real(line_perio.at(I));
            }

            //WE APPLY PHI
            fftw_execute(P3);
            _FilterBank._Phi=GaborDense(line_perio.n_elem,0,GetMetaSigma(_FilterBank._Q));
            for(unsigned int j=0;j<line_perio.n_elem;++j)
              line_perio.at(j)*=_FilterBank._Phi[j];
            //PERIODIZE
            for(int I=0;I<_FilterBank._T/2;++I)
                for(int j=0;j<SizeOfScatt;++j)
                    low.at(j)+=line_perio.at(j+I*SizeOfScatt);
            //IFFT
            fftw_execute(P4);
            for(int I=0;I<SizeOfScatt;++I)
                _Scattering.at(i+_Input.size()*filter_i,I)=std::abs(low.at(I));
            std::cout<<"DONE"<<std::endl;
        }
    }
}

#endif
