#include "./filters.h"

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
