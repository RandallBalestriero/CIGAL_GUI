#ifndef TD_MESH_RECTANGULAR
#define TD_MESH_RECTANGULAR

#include <armadillo>

class Mesh2DRectangular{
public:
    double Zmax,Zmin;
    arma::mat _Points;
    arma::mat _Rectangles;
    unsigned int _NumberOfPoints,_N,_M;
    unsigned int _NumberOfRectangles;
    Mesh2DRectangular():_NumberOfPoints(0),_NumberOfRectangles(0){};
    Mesh2DRectangular(float const&, float const&,float const&,float const&,int const&,int const&);
    template<typename O>
    void Fill(O const& f);
};


#endif
