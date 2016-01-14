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
#include <iostream>


inline double GetMetaXi(int const& Q){
    return (std::pow(2,-1.0/Q)+1)*M_PI/2;
}

inline double GetMetaSigma(int const& Q){
    return std::sqrt(3)*(1-pow(2,-1.0/Q));
}

inline double Lambda(int const& i,int const& Q){
    return pow(2,-(float)i/Q);
}

inline double f(double const& x,double const& m,double const& sigma){
    return std::exp(-std::pow(x-m,2)/(2.0*sigma*sigma));
}

inline arma::vec f(arma::vec const& x,double const& m,double const& sigma){
    arma::vec result(x.n_elem);
    result=arma::exp(-arma::pow(x-m,2)/(2*sigma*sigma));
    return result;
}

inline int StartSupport(arma::vec const& x,double const& m,double const& sigma,double const& eps){
    double dt=(x[x.n_elem-1]-x[0])/x.n_elem;
    return (-std::sqrt(-2*sigma*sigma*std::log(eps))+m)/dt;}
inline int StartSupport(int const& n,double const& m,double const& sigma,double const& eps){
    double dt=2*M_PI/n;
    return (-std::sqrt(-2*sigma*sigma*std::log(eps))+m)/dt;}


inline int EndSupport(arma::vec const& x,double const& m,double const& sigma,double const& eps){
    double dt=(x[x.n_elem-1]-x[0])/x.n_elem;
    return (std::sqrt(-2*sigma*sigma*std::log(eps))+m)/dt;}
inline int EndSupport(int const& n,double const& m,double const& sigma,double const& eps){
    double dt=2*M_PI/n;
    return (std::sqrt(-2*sigma*sigma*std::log(eps))+m)/dt;}

inline int TtoJ(int const& T,int const& Q){
    int M=(Q==1?2:1);
    return std::round(std::log2(T*M/(4.0*Q)));}


arma::vec Morlet(const std::size_t &,const float  &,const float &);

class MorletSparse{
private:
    arma::vec _Values;
public:
    int _Xi,_Sigma,_Start,_End,_Eps;
    MorletSparse(){}
    MorletSparse(int const&,double const&,double const&,double const& eps=0.0000001);
    inline double const& operator[](int const& i)const{return _Values.at(i);}
    inline double & operator[](int const& i){return _Values.at(i);}
    arma::vec & Values(){return _Values;}
    arma::vec const& Values()const{return _Values;}
};

inline std::ostream & operator<<(std::ostream & os, MorletSparse const& filter){
    os<<filter.Values();
    return os;
}


class GaborDense{
private:
    arma::vec _Values;
public:
    int _Xi=0,_Sigma;
    GaborDense(){}
    GaborDense(int const&,double const&,double const&);
    inline double const& operator[](int const& i)const{return _Values.at(i);}
    inline double & operator[](int const& i){return _Values.at(i);}
};

arma::vec MorletV2(const std::size_t &,const float  &,const float &,double const& eps);

arma::vec Gabor(const std::size_t &, const float &, const float &);




class PsisFilterBank{
private:
    std::vector<MorletSparse> _Psis;
public:
    int _NumberOfFilters;
    PsisFilterBank(){}
    inline PsisFilterBank(int const& N,int const& Q,int const& J,double const& eps=0.000001):_NumberOfFilters(Q*J){
        _Psis.resize(_NumberOfFilters);
        for(int i=0;i<_NumberOfFilters;++i){
            _Psis[i]=MorletSparse(N,GetMetaXi(Q)*Lambda(i,Q),GetMetaSigma(Q)*Lambda(i,Q),eps);
            std::cout<<"HERE1  "<<_Psis[i]._Start <<" "<<_Psis[i]._End<<std::endl;
        }
    }
    MorletSparse      & operator[](int const& i){return _Psis[i];}
    MorletSparse const& operator[](int const& i)const{return _Psis[i];}

};



class FilterBank{
public:
    PsisFilterBank _Psis;
    GaborDense _Phi;
    int _N,_T,_Q,_J;
    FilterBank(){}
    inline FilterBank(int const& N,int const& T,int const& Q,int const& J=0,double const& eps=0.000001):_N(N),_T(T),_Q(Q),_J(J){
        if(J==0)_J=TtoJ(T,Q);
        if(J<=0) std::cout<<"Error , negative J :"<<J<<std::endl;
        std::cout<<"HERE1"<<std::endl;
        _Psis=PsisFilterBank(N,Q,J,eps);
        std::cout<<"HERE2"<<std::endl;
        _Phi=GaborDense(N,0,GetMetaSigma(Q));
        std::cout<<"Filter Bank : T="<<_T <<", Q="<<_Q<<", J="<<_J<<std::endl;
    }
    inline MorletSparse & operator[](int const& i){return _Psis[i];}
    inline MorletSparse const& operator[](int const& i)const{return _Psis[i];}

};

/*
arma::vec Morlet(const std::size_t & signal_size,const float  & xi,const float & sigma){
    arma::vec _Coefficients=arma::linspace(0,2*M_PI,signal_size);
    _Coefficients=f(_Coefficients-2*M_PI,xi,sigma)+f(_Coefficients,xi,sigma);
    return _Coefficients;
}

arma::vec MorletV2(const std::size_t & signal_size, const float  & xi, const float & sigma, const double & eps){
    const double dt=2*M_PI/signal_size,Coeff=1.0/(2*sigma*sigma);
    const int A(StartSupport(signal_size,xi,sigma,eps)),B(EndSupport(signal_size,xi,sigma,eps));
    arma::vec x=arma::linspace(A*dt,(B+2)*dt,B-A+2);
    for(int i=0;i<B-A;++i)
        x[i]=std::exp(-std::pow(x[i]-xi,2)*Coeff);
    x[x.n_elem-2]=A;
    x[x.n_elem-1]=B;
    return x;
}


MorletSparse::MorletSparse(const int & N, const double & xi, const double & sig,const double& eps):_Xi(xi),_Sigma(sig){
    _Start=StartSupport(N,xi,sig,eps);
    _End=EndSupport(N,xi,sig,eps);
    const double dt=2*M_PI/N;
    const double Coeff=1.0/(2*sig*sig);

    _Values=arma::exp(-arma::pow(arma::linspace(_Start*dt,_End*dt,_End-_Start)-xi,2)*Coeff);
}

GaborDense::GaborDense(const int & n, const double & xi, const double & sig):_Xi(xi),_Sigma(sig){
    _Values=arma::linspace(0,2*M_PI,n);
    double const Coeff=1.0/(2*sig*sig),Coeff2=2*M_PI;
    for(unsigned int i=0;i<_Values.n_elem;++i)
        _Values.at(i)=std::exp(-std::pow(_Values.at(i)-xi,2)*Coeff)+std::exp(-std::pow(_Values.at(i)-Coeff2-xi,2)*Coeff);
}

arma::vec Gabor(const std::size_t & signal_size,const float& xi,const float & sigma){//constructor
    arma::vec _Coefficients=arma::linspace(0,2*M_PI,signal_size);
    double const Coeff=1.0/(2*sigma*sigma);
    return arma::exp(-arma::pow(_Coefficients-xi,2)*Coeff)+arma::exp(-arma::pow(_Coefficients-2*M_PI-xi,2)*Coeff);
}
*/
/*
inline std::ostream& operator<<(std::ostream& os,FilterBank const& filters){
    arma::vec z(filters._N);
    for(int i=0;i<filters._Q*filters._J;++i){
        z.fill(0);
        int start=filters._Psis[i].at(filters._Psis[i].n_elem-2);
        for(int j=start;j<filters._Psis[i].at(filters._Psis[i].n_elem-1);++j)
            z.at(j)=filters._Psis[i].at(j-start);
        for(int j=0;j<filters._N;++j)
            os<<z.at(j)<<" ";
        os<<std::endl;
    }
    return os;
}*/




#endif

