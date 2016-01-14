#include <QApplication>
#include "./source/qt/main_window.h"
#include "1D_function_parser.h"
#include "algebra/solve.h"
#include "./source/cigal/wav.h"
#include "./source/mesh/1D_mesh.h"
#include "./source/mesh/2D_rectangular_mesh.h"
#include "./source/mesh/2D_triangular_mesh.h"
#include "./source/fem/fem.h"

#include <fstream>

#include "./source/CIGAL.h"

using namespace std;



int main(int argc, char **argv ){

 //   FilterBank AA(1000,10,16,5);
  //  cout<<AA[0]<<endl;
   QApplication app( argc, argv );
   MainWindow window;
 //   FilterBank LL;
 //   std::cout<<TtoJ(8,16);

   // LL=FilterBank(18000,16,4);


    window.show();
    return app.exec();

 //   Filter=MorletV2(1024,2,0.3,0.00001);

    WAV wav("../CIGAL/wav/fast.wav");
    std::vector<arma::vec> Input(1);
   // cout<<wav._Values.n_rows<<endl;
   // int LL=1<<(int)std::ceil(std::log2(wav._Values.n_elem));


   // cout<<LL<<endl;
  //  Input[0]=arma::vec (LL);
  //  for(unsigned int i=0;i<wav._Values.n_elem;++i)
   //     Input[0].at(i)=wav._Values.at(i);

   // File<<S._FilterBank;
    //File<<S._Scattering<<"\n";

   // File<<L._Phi;
   // File.close();

/*
    arma::mat A(2,2);
    A(0,0)=2;
    A(1,1)=3;
    A(0,1)=-1;
    cout<<A<<endl;
    arma::vec X(2),B(2);
    B[0]=0.4;
    X=Jacobi(A,B,X);

    cout<<X<<endl;

    arma::vec a(2);
    a.fill(0);
    arma::vec b(2);
    b.fill(1);
    Mesh1D mesh(0,1,100);
    Mesh2DRectangular mesh2(a[0],a[1],b[0],b[1],4,4);
    Mesh2DTriangular mesh3(a,b,8,8);

    arma::sp_mat AA;
    arma::vec BB;
    g G;
    alpha _A;
    FillSystem(AA,BB,mesh3,_A,G);

    cout<<BB<<endl;
    cout<<AA<<endl;

    X.resize(BB.n_elem);
    X=CGD(AA,BB,X);*/
    return 0;
}

