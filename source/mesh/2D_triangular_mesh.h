#ifndef TRIANGULAR_MESH
#define TRIANGULAR_MESH


#include <armadillo>

class Mesh2DTriangular{
public:
    arma::mat _Triangles;
    arma::mat _Points;
    int _N,_M,_NumberOfTriangles,_NumberOfPoints;
    double Zmin,Zmax,_DomainArea,_Dx,_Dy;

    Mesh2DTriangular():_N(0),_M(0),_NumberOfTriangles(0),_NumberOfPoints(0){}
    Mesh2DTriangular(arma::vec const&,arma::vec const&,int const&,int const&);
    template<typename T2>
    void Fill(T2 const&);
    void Norm();
    void ToTXT(std::string const&);
};


#endif
