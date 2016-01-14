#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include "fem_window.h"
#include "../main_window.h"
#include <QSizePolicy>
#include <QResizeEvent>
#include <QMainWindow>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include "fem_plotting.h"
#include <iostream>
#include <QToolButton>
#include "../../automaton/2D_function_parser.h"
#include <fstream>
#include "../CIGAL.h"
#include "../../mesh/2D_triangular_mesh.h"
#include <QString>
#include "../utils.h"
#include "../algebra/solve.h"
#include <QThread>
#include <QPixmap>

using namespace std;



FEMTwoDWindow::FEMTwoDWindow(QMainWindow *parent):QDialog(parent),_Parent(parent){
        //create the two Communicate

    this->resize(parent->width(),parent->height());
    this->move(parent->x(),parent->y());

    CreateGLWidget();
    CreateButtonBox();
    CreateMeshBox();
    CreateProblemBox();
    CreateSolveBox();
    CreateMenu();


  //  _FunctionBox->setMaximumHeight(85);
  //  _RangeBox->setMaximumHeight(110);
 //   _ButtonBox->setMaximumHeight(70);

    _MainLayout = new QVBoxLayout;
    _ButtonBox->setMaximumHeight(70);
    _MeshBox->setMaximumHeight(250);
    _SolveBox->setMaximumHeight(70);
    _ProblemBox->setMaximumHeight(150);
    _MainLayout->setMenuBar(_BarMenu);
    _MainLayout->addWidget(_ProblemBox);
    _MainLayout->addWidget(_MeshBox);
    _MainLayout->addWidget(_SolveBox);
    _MainLayout->addWidget(_PlotMeshBox);
    _MainLayout->addWidget(_ButtonBox);


    setLayout(_MainLayout);
    setWindowTitle(tr("FEM 2D"));
}

FEMTwoDWindow::~FEMTwoDWindow(){

    delete _UtilityButton[0];
    delete _UtilityButton[1];

    delete   _BarMenu;
    delete   _FileMenu;
    delete   _OpenGLWindow;
    delete   _MeshBox;
    delete   _MainLayout;
    //delete   _MeshBoxLayout;
    //delete   _MeshPlotButton;
}



void FEMTwoDWindow::CreateMenu(){
    _BarMenu=new QMenuBar;
    _FileMenu = new QMenu(tr("File"), this);
 //   QAction *exitAction = _FileMenu->addAction(tr("Export Mesh"));
 //   QAction *exitAction2 = _FileMenu->addAction(tr("Export Approx."));
 //   QAction *exitAction3 = _FileMenu->addAction(tr("Exit"));
    _BarMenu->addMenu(_FileMenu);
}

//create the two buttons
void FEMTwoDWindow::CreateButtonBox(){
    _ButtonBox = new QGroupBox;
    _ButtonBoxLayout = new QHBoxLayout;
    _UtilityButton[0]=new QPushButton("Main Menu");
    _UtilityButton[1]=new QPushButton("Quit");
    _ButtonBoxLayout->addWidget(_UtilityButton[0]);
    _ButtonBoxLayout->addWidget(_UtilityButton[1]);
    _ButtonBox->setLayout(_ButtonBoxLayout);

    QObject::connect(_UtilityButton[0],SIGNAL(clicked()),this,SLOT(PopUpMenu()));
    QObject::connect(_UtilityButton[1],SIGNAL(clicked()),QApplication::instance(),SLOT(quit()));
}

//create the range box
void FEMTwoDWindow::CreateMeshBox(){
    _MeshBox = new QGroupBox;
    _MeshBoxLayout = new QGridLayout;
    _MeshLabel.resize(9);
    for(int i=0;i<8;++i) _MeshLabel[i]=new QLabel;
    _MeshLabel[0]->setText("a:=(");
    _MeshLabel[1]->setText(",");
    _MeshLabel[2]->setText("),");
    _MeshLabel[3]->setText("      N = ");
    _MeshLabel[4]->setText("b:=(");
    _MeshLabel[5]->setText(",");
    _MeshLabel[6]->setText("),");
    _MeshLabel[7]->setText("      M = ");

    QPixmap pix("./source/qt/fem2/square.png");
    _MeshLabel[8]=new QLabel;
    _MeshLabel[8]->setPixmap(pix.scaled(230,230,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    _MeshLabel[8]->show();

    _MeshLineEdit.resize(6);
    for(int i=0;i<6;++i) _MeshLineEdit[i]=new QLineEdit;
    _MeshLineEdit[0]->setText("0");
    _MeshLineEdit[1]->setText("0");
    _MeshLineEdit[2]->setText("10");
    _MeshLineEdit[3]->setText("1");
    _MeshLineEdit[4]->setText("1");
    _MeshLineEdit[5]->setText("10");




    _MeshPlotButton = new QPushButton("Create Mesh");

    _MeshBoxLayout->addWidget(_MeshLabel[8],0,0,2,1);
    _MeshBoxLayout->addWidget(_MeshLabel[0],0,1);
    _MeshBoxLayout->addWidget(_MeshLineEdit[0],0,2);
    _MeshBoxLayout->addWidget(_MeshLabel[1],0,3);
    _MeshBoxLayout->addWidget(_MeshLineEdit[1],0,4);
    _MeshBoxLayout->addWidget(_MeshLabel[2],0,5);
    _MeshBoxLayout->addWidget(_MeshLabel[3],0,6);
    _MeshBoxLayout->addWidget(_MeshLineEdit[2],0,7);

    _MeshBoxLayout->addWidget(_MeshLabel[4],1,1);
    _MeshBoxLayout->addWidget(_MeshLineEdit[3],1,2);
    _MeshBoxLayout->addWidget(_MeshLabel[5],1,3);
    _MeshBoxLayout->addWidget(_MeshLineEdit[4],1,4);
    _MeshBoxLayout->addWidget(_MeshLabel[6],1,5);
    _MeshBoxLayout->addWidget(_MeshLabel[7],1,6);
    _MeshBoxLayout->addWidget(_MeshLineEdit[5],1,7);

    _MeshBoxLayout->addWidget(_MeshPlotButton,1,8);

    _MeshBox->setLayout(_MeshBoxLayout);
    _MeshBox->setTitle("Mesh");
    QObject::connect(_MeshPlotButton,SIGNAL(clicked()),this,SLOT(PlotMesh()));
}



//create the range box
void FEMTwoDWindow::CreateProblemBox(){
    _ProblemBox=new QGroupBox;
    _ProblemBoxLayout= new QHBoxLayout;
    _ProblemBoxSubLayoutLeft = new QGridLayout;
    _ProblemBoxSubLayoutRight = new QGridLayout;

    _ProblemBoxLabel.resize(9);
    _ProblemBoxLabel[0]=new QLabel("Initial Problem : ");
    QPixmap pix("./source/qt/fem2/problem.png");
    _ProblemBoxLabel[1]=new QLabel;
    _ProblemBoxLabel[1]->setPixmap(pix);
    _ProblemBoxLabel[1]->show();
    //labels for the set g :
    _ProblemBoxLabel[4]=new QLabel("Value of g(x,y) on ");
    _ProblemBoxLabel[5]=new QLabel("{a0}x]a1,b1[ : ");
    _ProblemBoxLabel[6]=new QLabel("]a0,b0[x{b1} : ");
    _ProblemBoxLabel[7]=new QLabel("{b0}x]a1,b1[ : ");
    //label for alpha
    _ProblemBoxLabel[8]=new QLabel("Value of alpha on each sub-domain ");



    _ProblemBoxSubLayoutLeft->addWidget(_ProblemBoxLabel[0],0,0);
    _ProblemBoxSubLayoutLeft->addWidget(_ProblemBoxLabel[1],0,1);

    _ProblemBoxButton.resize(2);
    _ProblemBoxButton[0]=new QPushButton("Set alpha");
    _ProblemBoxButton[1]=new QPushButton("Set g");

    _ProblemBoxSubLayoutRight->addWidget(_ProblemBoxButton[0]);
    _ProblemBoxSubLayoutRight->addWidget(_ProblemBoxButton[1]);

    _ProblemBoxLayout->addLayout(_ProblemBoxSubLayoutLeft);
    _ProblemBoxLayout->addLayout(_ProblemBoxSubLayoutRight);
    _ProblemBox->setLayout(_ProblemBoxLayout);
    _ProblemBox->setTitle("Problem");
    QObject::connect(_ProblemBoxButton[0],SIGNAL(clicked()),this,SLOT(CreateAlpha()));
    QObject::connect(_ProblemBoxButton[1],SIGNAL(clicked()),this,SLOT(CreateG()));


}
//create the solver box
void FEMTwoDWindow::CreateSolveBox(){
    _SolveBox=new QGroupBox;
    _SolveBoxLayout= new QGridLayout;

    _SolveLabel.resize(4);
    _SolveLabel[0]=new QLabel("Solver");
    _SolveLabel[1]=new QLabel("Criteria");
    _SolveLabel[2]=new QLabel("Precondi.");
    _SolveLabel[3]=new QLabel("Waiting ...");
    _SolveLabel[3]->setMinimumWidth(200);


    _SolveMenu = new QComboBox;
    _SolveMenu->addItem("CGD");
    _SolveMenu->addItem("GD");

    _SolvePrecon = new QCheckBox;

    _SolveCriteria = new QLineEdit;
    _SolveCriteria->setText("0.0000001");

    _SolveButton = new QPushButton("Solve !");
    QObject::connect(_SolveButton,SIGNAL(clicked()),this,SLOT(Solve()));


    _SolveBoxLayout->addWidget(_SolveLabel[0],0,0);
    _SolveBoxLayout->addWidget(_SolveMenu,0,1);
    _SolveBoxLayout->addWidget(_SolveLabel[1],0,2);
    _SolveBoxLayout->addWidget(_SolveCriteria,0,3);
    _SolveBoxLayout->addWidget(_SolveLabel[2],0,4);
    _SolveBoxLayout->addWidget(_SolvePrecon,0,5);
    _SolveBoxLayout->addWidget(_SolveButton,0,6);
    _SolveBoxLayout->addWidget(_SolveLabel[3],0,7);


    _SolveBox->setLayout(_SolveBoxLayout);
    _SolveBox->setTitle("Solver Selection");
}

void FEMTwoDWindow::FromTxt(){
}


void FEMTwoDWindow::PlotMesh(){
    arma::vec s(2),e(2);e=1;
    s[0]=_MeshLineEdit[0]->text().toDouble();
    s[1]=_MeshLineEdit[1]->text().toDouble();
    e[0]=_MeshLineEdit[3]->text().toDouble();
    e[1]=_MeshLineEdit[4]->text().toDouble();
    int n,m;
    n=_MeshLineEdit[2]->text().toInt();
    m=_MeshLineEdit[5]->text().toInt();
    if(_Mesh){ delete _Mesh;_Mesh=0;}
    _Mesh=new Mesh2DTriangular(s,e,n,m);
    PopUpMesh= new QDialog(this);
    PopUpMeshLayout = new QHBoxLayout;
    if(plottingmesh) delete plottingmesh;
    plottingmesh= new PlotMeshWidget(PopUpMesh);
    plottingmesh->SetMesh(_Mesh);
    PopUpMeshLayout->addWidget(plottingmesh);
    PopUpMesh->setLayout(PopUpMeshLayout);
    PopUpMesh->resize(min(n*20,500),min(m*20,500));
    PopUpMesh->exec();
}


void FEMTwoDWindow::CreateAlpha(){
    PopAlpha= new QDialog(this);

    alphagrid = new QGridLayout;
    alphagrid->addWidget(_ProblemBoxLabel[8],0,0,1,3);

    if(!_Alpha) _Alpha=new alpha;
    _ProblemBoxAlphaGridLineEdit.resize(9);
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j){
            _ProblemBoxAlphaGridLineEdit[i*3+j]=new QLineEdit;
            alphagrid->addWidget(_ProblemBoxAlphaGridLineEdit[i*3+j],i+1,j);
            _ProblemBoxAlphaGridLineEdit[i*3+j]->setText(QString::fromStdString(std::to_string(_Alpha->_Values[i*3+j])));
        }

    PopAlpha->setLayout(alphagrid);
    PopAlpha->resize(300,140);
    PopAlpha->exec();

}

void FEMTwoDWindow::CreateG(){
    PopG= new QDialog(this);

    ggrid = new QGridLayout;
    if(!_G) _G = new g;

    _ProblemBoxGGridLineEdit.resize(3);
    ggrid->addWidget(_ProblemBoxLabel[4],0,0);
    ggrid->addWidget(_ProblemBoxLabel[5],1,0);
    ggrid->addWidget(_ProblemBoxLabel[6],2,0);
    ggrid->addWidget(_ProblemBoxLabel[7],3,0);
    for(int i=0;i<3;++i){
        _ProblemBoxGGridLineEdit[i]=new QLineEdit;
        _ProblemBoxGGridLineEdit[i]->setText(QString::fromStdString(std::to_string(_G->values[i])));
        ggrid->addWidget(_ProblemBoxGGridLineEdit[i],i+1,1);}
    PopG->setLayout(ggrid);
    PopG->resize(200,100);
    PopG->exec();
}



void FEMTwoDWindow::Solve(){
    _SolveLabel[3]->setText("Filling the system");
    for(int i=0;i<_Mesh->_NumberOfPoints;++i) _Mesh->_Points(i,2)=0;
    for(int i=0;i<9;++i) _Alpha->_Values[i]=_ProblemBoxAlphaGridLineEdit[i]->text().toFloat();
    for(int i=0;i<3;++i) _G->values[i]=_ProblemBoxGGridLineEdit[i]->text().toFloat();

    FillSystem(_A,_B,*_Mesh,*_Alpha,*_G);
    arma::vec X0(_B.n_elem);
    //prconditionning :
//    if(_SolvePrecon->isChecked()){
 //       _SolveLabel[3]->setText("Preconditionning");
  //      for(unsigned int i=0;i<_B.n_elem;++i){
  //          float tmp=_A(i,i);
  //          for(unsigned int j=0;j<(_A._R[i+1]-_A._R[i]);++j){
  //              _A._V[_A._R[i]+j]/=tmp;
  //          }
  //          _B[i]/=tmp;
  //      }
   // }
    if(_SolveMenu->currentIndex()==0){
        _SolveLabel[3]->setText("Solving the system with CGD");
        _SolveLabel[3]->update();
        CGD(_A,_B,X0,_SolveCriteria->text().toDouble(),10);
    }
    else{
        _SolveLabel[3]->setText("Solving the system with GD");
        GD(_A,_B,X0,_SolveCriteria->text().toDouble(),10);
    }
    _SolveLabel[3]->setText("Done !");
    for(unsigned int i=_Mesh->_N;i<_B.n_elem+_Mesh->_N;++i)
        _Mesh->_Points(i,2)=X0[i-_Mesh->_N];
    _Mesh->Norm();
    _MeshColorMap->_Max=_Mesh->Zmax;
    _MeshColorMap->_Min=_Mesh->Zmin;
    _MeshColorMap->update();
    _OpenGLWindow->mesh=_Mesh;
    _OpenGLWindow->update();
    this->update();
}


void FEMTwoDWindow::PopUpMenu(){
    QMainWindow* menu=new MainWindow(_Parent);
    menu->show();
    this->hide();
}

void FEMTwoDWindow::CreateGLWidget(){
    _MeshLayout = new QHBoxLayout;
    _MeshColorMap = new ColorMap;
    _OpenGLWindow=new FEMTwoDMeshPainter;
    _OpenGLWindow->setMinimumHeight(100);
    _MeshColorMap->setMaximumWidth(100);
    _MeshLayout->addWidget(_OpenGLWindow);
    _MeshLayout->addWidget(_MeshColorMap);
    _PlotMeshBox = new QGroupBox;
    _PlotMeshBox->setLayout(_MeshLayout);
}

