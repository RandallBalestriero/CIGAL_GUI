#include "./1D_mesh.h"

Mesh1D::Mesh1D(const double &a, const double &b, const unsigned int & n){
    _X=arma::vec(n);
    _X[0]=a;
    _N=n;
    _Deltax=((b-a)/(n-1));
    for(unsigned int i=1;i<n;++i)
        _X[i]=_X[i-1]+_Deltax;
    _Y=arma::vec(n);
}

Mesh1D& Mesh1D::b(double const& B){
    _Deltax=(B-_X[0])/(_N-1);
    for(unsigned int i=1;i<_N;++i)
        _X[i]=_X[i-1]+_Deltax;
    return *this;
}

Mesh1D& Mesh1D::a(double const& A){
    _Deltax=(_X[_N-1]-A)/(_N-1);
    _X[0]=A;
    for(unsigned int i=1;i<_N;++i)
        _X[i]=_X[i-1]+_Deltax;
    return *this;
}

Mesh1D& Mesh1D::n(unsigned int const& N){
    if(N>1){
        double const a=_X[0];
        double const b=_X[_N-1];
        _X=arma::vec (N);
        _Deltax=(b-a)/(N-1);
        _N=N;
        for(unsigned int i=1;i<_N;++i)
            _X[i]=_X[i-1]+_Deltax;
    }
    return *this;
}


