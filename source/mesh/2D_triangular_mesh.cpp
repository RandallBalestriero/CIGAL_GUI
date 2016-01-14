#include "./2D_triangular_mesh.h"

Mesh2DTriangular::Mesh2DTriangular(arma::vec const& p1,arma::vec const& p2,int const& n,int const& m){
    _N=n;_M=m;_NumberOfTriangles=2*(n-1)*(m-1);_NumberOfPoints=n*m;
    _DomainArea=(p2[0]-p1[0])*(p2[1]-p1[1]);
    _Dy=(p2[1]-p1[1])/(float)m;
    _Dx=(p2[0]-p1[0])/(float)n;
    _Triangles=arma::mat (_NumberOfTriangles,4);
    _Points=arma::mat (_NumberOfPoints,4);
    float dx=(p2[0]-p1[0])/(n-1);
    float dy=(p2[1]-p1[1])/(m-1);
    //create the points
    for(int i=0;i<m;++i)
        for(int j=0;j<n;++j){
            _Points(i*n+j,0)=p1[0]+j*dx;
            _Points(i*n+j,1)=p1[1]+i*dy;
        }
    //set labels to the points
    for(int j=0;j<n;++j)
        _Points(j,3)=1;
    for(int j=_NumberOfPoints-n;j<_NumberOfPoints;++j)
        _Points(j,3)=1;
    for(int i=1;i<m;++i){
        _Points(i*n,3)=1;
        _Points(i*n-1,3)=1;}
    //TRIANGLES
    for(int i=0;i<_NumberOfTriangles;i+=2){
        _Triangles(i,0)=(i/2)%(_N-1)+_N*(i/(2*(_N-1)));
        _Triangles(i,1)=_Triangles(i,0)+1;
        _Triangles(i,2)=_Triangles(i,1)+_N;

        _Triangles(i+1,2)=_Triangles(i,0);
        _Triangles(i+1,0)=_Triangles(i,2);
        _Triangles(i+1,1)=_Triangles(i+1,0)-1;
    }
    //set labels to the triangles
    for(int i=0;i<_NumberOfTriangles;++i)
        _Triangles(i,3)=_Points(_Triangles(i,0),3)+_Points(_Triangles(i,1),3)+_Points(_Triangles(i,2),3);
}

//fill the z values of the mesh points using the given function f
template<typename O>
void Mesh2DTriangular::Fill(O const& f){
    for(int i=0;i<_NumberOfPoints;++i)
        _Points(i,2)=f(_Points(i,0),_Points(i,1));
    Zmax=_Points(0,2);
    Zmin=_Points(0,2);
    for(int i=1;i<_NumberOfPoints;++i){
        if(Zmax<_Points(i,2)) Zmax=_Points(i,2);
        else if(Zmin>_Points(i,2)) Zmin=_Points(i,2);
    }
}

//Compute the min and the max of the z values of the mesh points
void Mesh2DTriangular::Norm(){
    Zmax=_Points(0,2);
    Zmin=_Points(0,2);
    for(int i=1;i<_NumberOfPoints;++i){
        if(Zmax<_Points(i,2)) Zmax=_Points(i,2);
        else if(Zmin>_Points(i,2)) Zmin=_Points(i,2);
    }
}




