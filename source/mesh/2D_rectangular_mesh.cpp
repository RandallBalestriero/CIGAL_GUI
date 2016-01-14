#include "./2D_rectangular_mesh.h"

Mesh2DRectangular::Mesh2DRectangular(const float & x0, const float & y0, const float & x1, const float & y1, const int & n, const int &m){
    _NumberOfPoints=n*m;
    _N=n;_M=m;
    _NumberOfRectangles=(n-1)*(m-1);
    _Points=arma::mat (_NumberOfPoints,4);
    _Rectangles=arma::mat (_NumberOfRectangles,4);
    float dx=(x1-x0)/(n-1);
    float dy=(y1-y0)/(m-1);
    for(int i=0;i<n;++i)
        for(int j=0;j<m;++j){
            _Points(i*m+j,0)=x0+j*dx;
            _Points(i*m+j,1)=y0+i*dy;
        }
    //set the limit point flag
    for(int i=0;i<n;++i)//firt line
        _Points(i,3)=1;
    for(unsigned int i=_NumberOfPoints-n;i<_NumberOfPoints;++i)//last line
        _Points(i,3)=1;
    //sides
    for(int i=1;i<m;++i){
        _Points(i*n,3)=1;
        _Points(i*n-1,3)=1;
    }
    for(int i=0;i<n-1;++i)
        for(int j=0;j<m-1;++j){
            _Rectangles(i*(m-1)+j,0)=i*m+j;
            _Rectangles(i*(m-1)+j,1)=i*m+j+1;
            _Rectangles(i*(m-1)+j,2)=(i+1)*m+j+1;
            _Rectangles(i*(m-1)+j,3)=(i+1)*m+j;
        }
}

template<typename O>
void Mesh2DRectangular::Fill(O const& f){
    for(int i=0;i<_NumberOfPoints;++i)
        _Points(i,2)=f(_Points(i,0),_Points(i,1));
    Zmax=_Points(0,2);
    Zmin=_Points(0,2);
    for(int i=1;i<_NumberOfPoints;++i){
        if(Zmax<_Points(i,2)) Zmax=_Points(i,2);
        else if(Zmin>_Points(i,2)) Zmin=_Points(i,2);
    }
}
