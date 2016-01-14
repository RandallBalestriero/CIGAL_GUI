#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include "2D_window.h"
#include "../main_window.h"
#include <QSizePolicy>
#include <QResizeEvent>
#include <QMainWindow>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include "2D_mesh_painter.h"
#include <iostream>
#include <QToolButton>
#include "../../automaton/2D_function_parser.h"
#include <fstream>
#include "../CIGAL.h"
#include "../../mesh/2D_rectangular_mesh.h"
#include <QString>

using namespace std;



TwoDWindow::TwoDWindow(QMainWindow *parent):QDialog(parent),_Parent(parent){
        //create the two Communicate

    this->resize(800,600);
    this->move(parent->x(),parent->y());

    CreateGLWidget();

    CreateFunctionBox();
    CreateRangeBox();
    CreateMenu();
    CreateButtonBox();

    _FunctionBox->setMaximumHeight(85);
    _RangeBox->setMaximumHeight(110);
    _ButtonBox->setMaximumHeight(70);

    _MainLayout = new QVBoxLayout;
    _MainLayout->setMenuBar(_BarMenu);
    _MainLayout->addWidget(_FunctionBox);
    _MainLayout->addWidget(_RangeBox);
    _MainLayout->addWidget(_OpenGLWindow);
    _MainLayout->addWidget(_ButtonBox);

    setLayout(_MainLayout);
    setWindowTitle(tr("Plotting 2D"));
}

TwoDWindow::~TwoDWindow(){

    delete _UtilityButton[0];
    delete _UtilityButton[1];

    delete   _BarMenu;
    delete   _FileMenu;
    delete   _OpenGLWindow;
    delete   _ButtonBox;
    delete   _MainLayout;
}



void TwoDWindow::CreateMenu(){
    _BarMenu=new QMenuBar;
  //  _FileMenu = new QMenu(tr("File"), this);
 //   QAction *exitAction = _FileMenu->addAction(tr("Export Mesh"));
 //   QAction *exitAction2 = _FileMenu->addAction(tr("Export Approx."));
 //   QAction *exitAction3 = _FileMenu->addAction(tr("Exit"));
  //  _BarMenu->addMenu(_FileMenu);
}

//create the two buttons
void TwoDWindow::CreateButtonBox(){
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
void TwoDWindow::CreateFunctionBox(){
    _FunctionBox = new QGroupBox(tr("Function"));
    _FunctionBoxLayout = new QGridLayout;
    _FunctionBoxLineEdit = new QLineEdit;

    _FunctionBoxLabel = new QLabel;
    _FunctionBoxLabel->setText("f(x,y)=");

    _FunctionBoxImportButton=new QPushButton("From txt");

    _FunctionBoxLayout->addWidget(_FunctionBoxLabel,0,0);
    _FunctionBoxLayout->addWidget(_FunctionBoxLineEdit,0,1);
    _FunctionBoxLayout->addWidget(_FunctionBoxImportButton,0,2);
    _FunctionBox->setLayout(_FunctionBoxLayout);

    QObject::connect(_FunctionBoxImportButton,SIGNAL(clicked()),this,SLOT(FromTxt()));
}



//create the range box
void TwoDWindow::CreateRangeBox(){
    _RangeBox = new QGroupBox(tr("Ranges"));
    _RangeBoxLayout = new QGridLayout;

    _RangeBoxPlotButton = new QPushButton("Plot !");
    _RangeBoxLineEdit.resize(6);
    _RangeBoxLabel.resize(6);
    for(int i=0;i<6;++i){
        _RangeBoxLineEdit[i]=new QLineEdit;
        _RangeBoxLabel[i]=new QLabel;
    }

    _RangeBoxLabel[0]->setText("x0 :");
    _RangeBoxLabel[1]->setText("xn :");
    _RangeBoxLabel[2]->setText("N :");
    _RangeBoxLabel[3]->setText("y0 :");
    _RangeBoxLabel[4]->setText("yn :");
    _RangeBoxLabel[5]->setText("M :");

    _RangeBoxLayout->addWidget(_RangeBoxLabel[0],0,0);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[0],0,1);
    _RangeBoxLayout->addWidget(_RangeBoxLabel[1],0,2);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[1],0,3);
    _RangeBoxLayout->addWidget(_RangeBoxLabel[2],0,4);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[2],0,5);

    _RangeBoxLayout->addWidget(_RangeBoxLabel[3],1,0);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[3],1,1);
    _RangeBoxLayout->addWidget(_RangeBoxLabel[4],1,2);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[4],1,3);
    _RangeBoxLayout->addWidget(_RangeBoxLabel[5],1,4);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[5],1,5);

    _RangeBoxLayout->addWidget(_RangeBoxPlotButton,2,5);

    _RangeBox->setLayout(_RangeBoxLayout);

    QObject::connect(_RangeBoxPlotButton,SIGNAL(clicked()),this,SLOT(Plot()));
}


void TwoDWindow::FromTxt(){/*
    _FunctionBoxDialogTxt = new QFileDialog(this,tr("Open txt"),"./",tr("Text files (*.txt)"));
    _FunctionBoxDialogTxt->show();
    if(_FunctionBoxDialogTxt->exec()){
        QStringList files = _FunctionBoxDialogTxt->selectedFiles();
        std::ifstream f(files.at(0).toStdString());
        float v;
        if(_FunctionBoxCombo->currentIndex()==0){
            _FunctionBoxCombo->addItem(QString::number((int)_FunctionBoxCombo->count()));
            _FunctionBoxCombo->setCurrentIndex(_FunctionBoxCombo->count()-1);
            _OpenGLWindow->_MeshVector.append(Mesh1D<> ());
            _OpenGLWindow->_MeshColor.append(_FunctionBoxComboColor->currentText().toStdString());
            _FunctionBoxSource.append(QString::fromStdString("From "+files.at(0).toStdString()));
        }
        else{
            _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]=Mesh1D<> ();
            _FunctionBoxSource[_FunctionBoxCombo->currentIndex()-1]=QString::fromStdString("From "+files.at(0).toStdString());
        }
        while(f>>v){
            _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._Y.append(v);
        }
        _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X=PlainVector<> (_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector._N-1]._Y._N);
        for(int i=0;i<_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector._N-1]._Y._N;++i)
            _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X[i]=i;
        //update info on mesh painter
        _OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector._N-1]._N=_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector._N-1]._Y._N;
        _OpenGLWindow->Xmax=_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X.max();
        _OpenGLWindow->Xmin=_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X.min();
        _OpenGLWindow->Ymax=_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._Y.max();
        _OpenGLWindow->Ymin=_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._Y.min();
        _OpenGLWindow->update();
        f.close();
    }
    delete _FunctionBoxDialogTxt;*/
}


void TwoDWindow::Plot(){
        _OpenGLWindow->AddMesh(_RangeBoxLineEdit[0]->text().toFloat(),_RangeBoxLineEdit[3]->text().toFloat(),_RangeBoxLineEdit[1]->text().toFloat(),_RangeBoxLineEdit[4]->text().toFloat(),_RangeBoxLineEdit[2]->text().toUInt(),_RangeBoxLineEdit[5]->text().toUInt());
        std::string F=_FunctionBoxLineEdit->text().toStdString();
        Tree2D T(F);
  //      _OpenGLWindow->_Mesh.Fill(T);
        _OpenGLWindow->update();
}

void TwoDWindow::Trig(){
    cout<<"TRIG "<<_FunctionNumberMenu->currentIndex()<<endl;
}

void TwoDWindow::PopUpMenu(){
    QMainWindow* menu=new MainWindow(_Parent);
    menu->show();
    this->hide();    
}

void TwoDWindow::CreateGLWidget(){
    _OpenGLWindow=new TwoDMeshPainter;
    _OpenGLWindow->setStyleSheet("background-color:black;");
}

