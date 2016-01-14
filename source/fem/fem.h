#ifndef FEM_HEADER_FILE
#define FEM_HEADER_FILE

#include "../mesh/2D_rectangular_mesh.h"
#include "../mesh/2D_triangular_mesh.h"

//##########################################
//
//      UTILITY
//
//############################################


//see if a point is in a square domain where domain is a vector of two points so a
//vector of 4 floats corresponding to the lower left and upper right points
//point is a vector of size at leat two
inline bool InSquareDomain(arma::vec const& point,arma::mat const& domain,int const& i){
    if(point[0]>=domain(i,0) && point[0]<= domain(i,2))
        if(point[1]>=domain(i,1) && point[1]<=domain(i,3))
            return 1;
    return 0;}

inline bool InSquareDomain(float const& x,float const& y,arma::mat const& domain,int const& i){
    if(x>=domain(i,0) && x<= domain(i,2))
        if(y>=domain(i,1) && y<=domain(i,3))
            return 1;
    return 0;}



//#########################################################
//
//      PIECEWISE DEFINED FUNCTION
//
//#########################################################


//alpha class which is the function used in the integral
class alpha{
public:
    arma::mat _Domains;//domains with two points for a square
    arma::vec _Values;
    alpha();

    double const& operator()(arma::vec const& Point)const;
    double const& operator()(float const& x,float const& y)const;
};


// boundary function g defined on each of the 3 boundaries
class g{
public:
    double values[3];
    g();
    double operator()(arma::vec const& p)const;
    double operator()(double const&,double const&)const;

};



//      FOR THE BILINEAR FORM
arma::vec grad(arma::vec const& a,arma::vec const& b,arma::vec const& c);

//compute a given the mesh, i, j and alpha
inline float a(Mesh2DTriangular const& mesh,int const& TriangleNumber,int const& i,int const& j,alpha const& _Alpha){
    const float area=(mesh._DomainArea)/mesh._NumberOfTriangles;
    const float alphaquadrature=(_Alpha(mesh._Points(mesh._Triangles(TriangleNumber,0),0),mesh._Points(mesh._Triangles(TriangleNumber,0),1))
                                 +_Alpha(mesh._Points(mesh._Triangles(TriangleNumber,1),0),mesh._Points(mesh._Triangles(TriangleNumber,1),1))
                                 +_Alpha(mesh._Points(mesh._Triangles(TriangleNumber,2),0),mesh._Points(mesh._Triangles(TriangleNumber,2),1)))/3.0;
    arma::vec triangle10(3),triangle11(3),triangle12(3);
    arma::vec triangle20,triangle21,triangle22;
    triangle10[0]=mesh._Points(mesh._Triangles(TriangleNumber,0),0);
    triangle10[1]=mesh._Points(mesh._Triangles(TriangleNumber,0),1);
    triangle10[2]=0;
    triangle11[0]=mesh._Points(mesh._Triangles(TriangleNumber,1),0);
    triangle11[1]=mesh._Points(mesh._Triangles(TriangleNumber,1),1);
    triangle11[2]=0;
    triangle12[0]=mesh._Points(mesh._Triangles(TriangleNumber,2),0);
    triangle12[1]=mesh._Points(mesh._Triangles(TriangleNumber,2),1);
    triangle12[2]=0;

    triangle20=triangle10;
    triangle21=triangle11;
    triangle22=triangle12;
    if(i==0) triangle10[2]=1;
    else if(i==1) triangle11[2]=1;
    else triangle12[2]=1;
    if(j==0) triangle20[2]=1;
    else if(j==1) triangle21[2]=1;
    else triangle22[2]=1;

    return area*alphaquadrature*arma::sum(grad(triangle10,triangle11,triangle12)%grad(triangle20,triangle21,triangle22));
}

//          FOR THE LINEAR FORM

//compute b given the mesh, i, alpha and g
inline float b(Mesh2DTriangular const& mesh,int const& i, alpha const& A, g const& G){
    if(mesh._Points(i,3)==0) return 0;//not on the boundary
    if(mesh._Points(i,0)==0 || mesh._Points(i,0)==1)//if on S_1 or S_3
        return mesh._Dy*A(mesh._Points(i,0),mesh._Points(i,1))*G(mesh._Points(i,0),mesh._Points(i,1));
    else//on S_2
        return mesh._Dx*A(mesh._Points(i,0),mesh._Points(i,1))*G(mesh._Points(i,0),mesh._Points(i,1));
}


//############################################
//
//      FUNCTION TO FILL THE SYSTEM
//
//##############################################




inline void FillSystem(arma::sp_mat & A,arma::vec & B,Mesh2DTriangular const& mesh,alpha const& al,g const& G){
    B=arma::vec (mesh._NumberOfPoints-mesh._N);
    A=arma::sp_mat (mesh._NumberOfPoints-mesh._N,mesh._NumberOfPoints-mesh._N);
    for(int i=mesh._N;i<mesh._NumberOfPoints;++i) B[i-mesh._N]=b(mesh,i,al,G);
    for(int t=0;t<mesh._NumberOfTriangles;++t)
        for(int i=0;i<3;++i){
            int S1(mesh._Triangles(t,i)-mesh._N);
            if(S1>=0){//in the domain and a ddl
                for(int j=0;j<3;++j){
                    int S2(mesh._Triangles(t,j)-mesh._N);
                    if(S2>=0)
                        A(S1,S2)+=a(mesh,t,i,j,al);
                }
            }
        }
}


#endif
