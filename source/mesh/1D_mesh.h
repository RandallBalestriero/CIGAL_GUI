#ifndef ODMESH_HEADER_FILE
#define ODMESH_HEADER_FILE

#include <armadillo>

struct Mesh1D{
    arma::vec _X;
    arma::vec _Y;
    unsigned int _N;
    double _Deltax;
    Mesh1D(){};
    Mesh1D(double const&,double const&,unsigned int const&);
    Mesh1D& a(double const&);//change a the starting value
    Mesh1D& b(double const&);//change b the ending value
    Mesh1D& n(unsigned int const&);//change the number of points
};



#endif
