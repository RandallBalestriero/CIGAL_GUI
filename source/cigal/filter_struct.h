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

#ifndef filter_struct_header_file
#define filter_struct_header_file

#include "meta_param.h"
#include <cmath>
#include <armadillo>

//----------------------------------------------------------//
//      PARENT CLASS WHICH CAN HOLD ANY KIND OF 1D FILER    //
//      EVEN USER DEFINED ONES                              //
//----------------------------------------------------------//
/*

class Filter1D{

    protected:
        float  _MotherSigma,_Sigma;
        float  _MotherXi,_Xi;
        float  _Scale,_Lambda;
        float  _Log2Bandwidth;
        int    _Gamma,_PE;
    public:
        arma::vec _Coefficients;
        Filter1D(){};
        virtual void ComputeCoefficients(){};//specialized method dependant on the filter type
        Filter1D(Filter1D && other){
            _Coefficients=other._Coefficients;
            _Gamma=other._Gamma;
            _PE=other._PE;
            _MotherXi=other._MotherXi;
            _MotherSigma=other._MotherSigma;
            _Lambda=other._Lambda;
            _Scale=other._Scale;
            _Xi=other._Xi;
            _Sigma=other._Sigma;
            _Log2Bandwidth=other._Log2Bandwidth;
        }
        ~Filter1D(){};

        float const& operator[](const std::size_t& i)const{return _Coefficients[i];}
        float & operator[](const std::size_t& i){return _Coefficients[i];}
        void operator>>(const char* name){
            ofstream file;
            file.open(name);
            for(std::size_t i=0;i<_Coefficients._N;++i){
                file<<_Coefficients[i]<<" ";
            }
            file.close();
        }

        Filter1D& operator=(Filter1D const & other){
            _Coefficients=other._Coefficients;
            _Gamma=other._Gamma;
            _PE=other._PE;
            _MotherXi=other._MotherXi;
            _MotherSigma=other._MotherSigma;
            _Lambda=other._Lambda;
            _Scale=other._Scale;
            _Xi=other._Xi;
            _Sigma=other._Sigma;
            _Log2Bandwidth=other._Log2Bandwidth;
            return *this;
        }
        Filter1D& operator=(Filter1D && other){
            _Coefficients=other._Coefficients;
            _Gamma=other._Gamma;
            _PE=other._PE;
            _MotherXi=other._MotherXi;
            _MotherSigma=other._MotherSigma;
            _Lambda=other._Lambda;
            _Scale=other._Scale;
            _Xi=other._Xi;
            _Sigma=other._Sigma;
            _Log2Bandwidth=other._Log2Bandwidth;
            return *this;
        }

};

//  DEFAULT DESTRUCTOR USING THE Clean UTILITY



//------------------------------------------------------//
//          SPECIALIZED CLASS                           //
//          ONE PER FILTER TYPE                         //
//------------------------------------------------------//

//MORLET FILTER
/*
class MorletFilter1D: public Filter1D{
    public:
        MorletFilter1D(){}//default constructor
        MorletFilter1D(const std::size_t signal_size,const int gamma,MetaParam const& opt){//constructor
                                                                                           //using MetaParam
            _Gamma=gamma;
            _Coefficients=arma::vec (signal_size);
            _PE=opt._PE;
            _MotherXi=(M_PI/2)*(pow(2,-1./opt._Q)+1);//(M_PI/2)*(pow(2,-1./opt._Q)+1);
            _MotherSigma=sqrt(3)*(1-pow(2,-1./opt._Q));
            _Lambda=pow(2,(float)-_Gamma/opt._Q);
            _Scale=4.*opt._Q/_Lambda;
            _Xi=_MotherXi*_Lambda;
            _Sigma=_MotherSigma*_Lambda;
            _Log2Bandwidth=ceil(log2(_Lambda/opt._Q));
            ComputeCoefficients();
        }

        MorletFilter1D(const std::size_t signal_size,const float xi,const float sigma,const int pe=1){//constructor
                                                                                                //using sigma xi
            _Gamma=0;
            _PE=pe;
            _MotherXi=0;
            _MotherSigma=0;
            _Lambda=0;
            _Scale=0;
            _Xi=xi;
            _Sigma=sigma;
            _Log2Bandwidth=0;
            _Coefficients=arma::vec (signal_size);
            ComputeCoefficients();
        }

        void ComputeCoefficients(){//utility for creating the filter, lowpass is substracted to the computed
                                    //filter to guarantee wavelet condition which means that two Gabors have
                                    //to be computed to end up with one Morlet
            const float DoublePi=2*M_PI;
            const float step=DoublePi/_Coefficients._N;
            float x=0;
            const float ExpSigma=2.*pow(_Sigma,2);
            arma::vec lowpasses(_Coefficients._N);
            for(std::size_t i=0;i<_Coefficients._N;++i){
                for(int k=-_PE;k<_PE+1;++k){
                    _Coefficients[i]+=exp(-pow(x+k*DoublePi-_Xi,2)/ExpSigma);
                    lowpasses[i]+=exp(-pow(x+k*DoublePi,2)/ExpSigma);
                }
                x+=step;
            }
            lowpasses*=_Coefficients[0];
            _Coefficients-=lowpasses;
        }
};


//  GABOR CLASS

class GaborFilter1D: public Filter1D{
    public:
        GaborFilter1D(){_Coefficients=0;}//default constructor
        GaborFilter1D(const std::size_t signal_size,const int gamma,MetaParam const& opt){//constructor
                                                                                        //using MetaParam
            _Gamma=gamma;
            _PE=opt._PE;
            _MotherXi=(M_PI/2)*(pow(2,-1./opt._Q)+1);
            _MotherSigma=sqrt(3)*(1-pow(2,-1./opt._Q));
            _Lambda=pow(2,(float)-_Gamma/opt._Q);
            _Scale=4.*opt._Q/_Lambda;
            _Xi=_MotherXi*_Lambda;
            _Sigma=_MotherSigma*_Lambda;
            _Log2Bandwidth=ceil(log2(_Lambda/opt._Q));
            _Coefficients=arma::vec (signal_size);
            ComputeCoefficients();
        }

        GaborFilter1D(const std::size_t signal_size,const float xi,const float sigma,const int pe=1){//constructor
                                                                                                //using sigma xi
            _Gamma=0;
            _PE=pe;
            _MotherXi=0;
            _MotherSigma=0;
            _Lambda=0;
            _Scale=0;
            _Xi=xi;
            _Sigma=sigma;
            _Log2Bandwidth=0;
            _Coefficients=Vector <float> (signal_size);
            ComputeCoefficients();
        }

        void ComputeCoefficients(){//filter creation algorithm
            const float step=2.*M_PI/_Coefficients._N;
            float x=0,PEPosition;
            const float ExpSigma=2.*pow(_Sigma,2);
            for(std::size_t i=0;i<_Coefficients._N;++i){
                for(short int k=-_PE;k<_PE+1;++k){
                    PEPosition=x+(int)k*2*M_PI;
                    _Coefficients[i]+=exp(-pow(PEPosition-_Xi,2)/ExpSigma);
                }
                x+=step;
            }
        }
};
*/
//If one wants to add a new wavelet it can be done by simple adding another class and inherit from Filter1D

/*
inline arma::vec Morlet(const std::size_t & signal_size,const int & gamma,MetaParam const& opt){

            arma::vec _Coefficients(signal_size);

            float _MotherXi=(M_PI/2)*(pow(2,-1./opt._Q)+1);
            float _MotherSigma=sqrt(3)*(1-pow(2,-1./opt._Q));
            float _Lambda=pow(2,(float)-gamma/opt._Q);

            float _Xi=_MotherXi*_Lambda;
            float _Sigma=_MotherSigma*_Lambda;

            const float DoublePi=2*M_PI;
            const float step=DoublePi/_Coefficients.n_elem;
            float x=0;
            const float ExpSigma=2.*pow(_Sigma,2);
            arma::vec lowpasses(_Coefficients.n_elem);
            for(std::size_t i=0;i<_Coefficients.n_elem;++i){
                _Coefficients[i]+=exp(-pow(x+1*DoublePi-_Xi,2)/ExpSigma)+exp(-pow(x+-1*DoublePi-_Xi,2)/ExpSigma)+exp(-pow(x-_Xi,2)/ExpSigma);
                lowpasses[i]+=exp(-pow(x+1*DoublePi,2)/ExpSigma)+exp(-pow(x+-1*DoublePi,2)/ExpSigma)+exp(-pow(x,2)/ExpSigma);
                x+=step;
            }
            lowpasses*=_Coefficients[0];
            _Coefficients-=lowpasses;
            return _Coefficients;
 }
*/
inline void InplaceMorlet(arma::vec & _Coefficients,const int& gamma,MetaParam const& opt){

            const float _MotherXi=(M_PI/2)*(pow(2,-1./opt._Q)+1);
            const float _MotherSigma=sqrt(3)*(1-pow(2,-1./opt._Q));
            const float _Lambda=pow(2,(float)-gamma/opt._Q);

            const float _Xi=_MotherXi*_Lambda;
            const float _Sigma=_MotherSigma*_Lambda;

            const float DoublePi=2*M_PI;
            const float step=DoublePi/_Coefficients.n_elem;
            float x=0;
            const float ExpSigma=2.*pow(_Sigma,2);
            arma::vec lowpasses(_Coefficients.n_elem);
            for(std::size_t i=0;i<_Coefficients.n_elem;++i){
                _Coefficients[i]+=exp(-pow(x+1*DoublePi-_Xi,2)/ExpSigma)+exp(-pow(x+-1*DoublePi-_Xi,2)/ExpSigma)+exp(-pow(x-_Xi,2)/ExpSigma);
                lowpasses[i]+=exp(-pow(x+1*DoublePi,2)/ExpSigma)+exp(-pow(x+-1*DoublePi,2)/ExpSigma)+exp(-pow(x,2)/ExpSigma);
                x+=step;
            }
            lowpasses*=_Coefficients[0];
            _Coefficients-=lowpasses;
 }

inline arma::vec Morlet(const std::size_t & signal_size,const float  & xi,const float & sigma){//constructor
                                                                                                //using sigma xi
    arma::vec _Coefficients(signal_size);

    const float DoublePi=2*M_PI;
    const float step=DoublePi/_Coefficients.n_elem;
    float x=0;
    const float ExpSigma=2.*pow(sigma,2);
    arma::vec lowpasses(_Coefficients.n_elem);
    for(std::size_t i=0;i<_Coefficients.n_elem;++i){
        _Coefficients[i]+=exp(-pow(x+1*DoublePi-xi,2)/ExpSigma)+exp(-pow(x+-1*DoublePi-xi,2)/ExpSigma)+exp(-pow(x-xi,2)/ExpSigma);
        lowpasses[i]+=exp(-pow(x+1*DoublePi,2)/ExpSigma)+exp(-pow(x+-1*DoublePi,2)/ExpSigma)+exp(-pow(x,2)/ExpSigma);
        x+=step;
    }
    lowpasses*=_Coefficients[0];
    _Coefficients-=lowpasses;
    return _Coefficients;
}

inline void InplaceMorlet(arma::vec & _Coefficients,const float  & xi,const float & sigma){//constructor
                                                                                                //using sigma xi
    const float DoublePi=2*M_PI;
    const float step=DoublePi/_Coefficients.n_elem;
    float x=0;
    const float ExpSigma=2.*pow(sigma,2);
    arma::vec lowpasses(_Coefficients.n_elem);
    for(std::size_t i=0;i<_Coefficients.n_elem;++i){
        _Coefficients[i]+=exp(-pow(x+1*DoublePi-xi,2)/ExpSigma)+exp(-pow(x+-1*DoublePi-xi,2)/ExpSigma)+exp(-pow(x-xi,2)/ExpSigma);
        lowpasses[i]+=exp(-pow(x+1*DoublePi,2)/ExpSigma)+exp(-pow(x+-1*DoublePi,2)/ExpSigma)+exp(-pow(x,2)/ExpSigma);
        x+=step;
    }
    lowpasses*=_Coefficients[0];
    _Coefficients-=lowpasses;
}






inline arma::vec Gabor(const std::size_t& signal_size,const int& gamma,MetaParam const& opt){

    arma::vec _Coefficients(signal_size);

    float _MotherXi=(M_PI/2)*(pow(2,-1./opt._Q)+1);
    float _MotherSigma=sqrt(3)*(1-pow(2,-1./opt._Q));
    float _Lambda=pow(2,(float)-gamma/opt._Q);
    float _Xi=_MotherXi*_Lambda;
    float _Sigma=_MotherSigma*_Lambda;

    const float DoublePi(2*M_PI);
    const float step=DoublePi/_Coefficients.n_elem;
    float x=0,PEPosition;
    const float ExpSigma=2.*pow(_Sigma,2);
    for(std::size_t i=0;i<_Coefficients.n_elem;++i){
        for(int k=-opt._PE;k<opt._PE+1;++k){
            PEPosition=x+k*DoublePi;
            _Coefficients[i]+=exp(-pow(PEPosition-_Xi,2)/ExpSigma);
        }
        x+=step;
    }
    return _Coefficients;
 }

inline arma::vec Gabor(const std::size_t & signal_size,const float  & xi,const float & sigma,const int & pe=1){//constructor
                                                                                                //using sigma xi
    arma::vec _Coefficients(signal_size);
    const float DoublePi(2*M_PI);
    const float step=DoublePi/_Coefficients.n_elem;
    float x=0,PEPosition;
    const float ExpSigma=2.*pow(sigma,2);
    for(std::size_t i=0;i<_Coefficients.n_elem;++i){
        for(int k=-pe;k<pe+1;++k){
            PEPosition=x+k*DoublePi;
            _Coefficients[i]+=exp(-pow(PEPosition-xi,2)/ExpSigma);
        }
        x+=step;
    }
    return _Coefficients;
}


inline void InplaceGabor(arma::vec & _Coefficients,const float  & xi,const float & sigma){//constructor
    const float DoublePi(2*M_PI);
    const float step=DoublePi/_Coefficients.n_elem;
    float x=0;
    const float ExpSigma=2.*pow(sigma,2);
    for(unsigned int i=0;i<_Coefficients.n_elem;++i){
        _Coefficients[i]=exp(-pow(x-DoublePi-xi,2)/ExpSigma)+exp(-pow(x-xi,2)/ExpSigma)+exp(-pow(x+DoublePi-xi,2)/ExpSigma);
        x+=step;
    }
}



#endif

