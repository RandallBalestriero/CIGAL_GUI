#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include "1D_window.h"
#include "../main_window.h"
#include <QSizePolicy>
#include <QResizeEvent>
#include <QMainWindow>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include "1D_mesh_painter.h"
#include <iostream>
#include <QToolButton>
#include "../automaton/1D_function_parser.h"
#include <fstream>
#include "../CIGAL.h"
#include "../mesh/1D_mesh.h"
#include <QString>

using namespace std;



OneDWindow::OneDWindow(QMainWindow *parent):QDialog(parent),_Parent(parent){
        //create the two Communicate

    this->resize(800,600);
    this->move(parent->x(),parent->y());

    CreateGLWidget();

    CreateFunctionBox();
    CreateRangeBox();
    CreateMenu();
    CreateButtonBox();

    _FunctionBox->setMaximumHeight(100);
    _RangeBox->setMaximumHeight(90);
    _ButtonBox->setMaximumHeight(70);

    _MainLayout = new QVBoxLayout;
    _MainLayout->setMenuBar(_BarMenu);
    _MainLayout->addWidget(_FunctionBox);
    _MainLayout->addWidget(_RangeBox);
    _MainLayout->addWidget(_OpenGLWindow);
    _MainLayout->addWidget(_ButtonBox);

    setLayout(_MainLayout);
    setWindowTitle(tr("Plotting 1D"));
}

OneDWindow::~OneDWindow(){

    delete _UtilityButton[0];
    delete _UtilityButton[1];

    delete   _BarMenu;
    delete   _FileMenu;
    delete   _OpenGLWindow;
    delete   _ButtonBox;
    delete   _MainLayout;
    delete   _RangeBox;
    for(unsigned int i=0;i<_RangeBoxLineEdit.size();++i)
        delete _RangeBoxLineEdit[i];
    delete _FunctionBox;
}



void OneDWindow::CreateMenu(){
    _BarMenu=new QMenuBar;
    _FileMenu = new QMenu(tr("File"), this);
 //   QAction *exitAction = _FileMenu->addAction(tr("Export Mesh"));
 //   QAction *exitAction2 = _FileMenu->addAction(tr("Export Approx."));
 //   QAction *exitAction3 = _FileMenu->addAction(tr("Exit"));
    _BarMenu->addMenu(_FileMenu);
}

//create the two buttons
void OneDWindow::CreateButtonBox(){
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
void OneDWindow::CreateFunctionBox(){
    _FunctionBox = new QGroupBox(tr("Function"));
    _FunctionBoxLayout = new QGridLayout;
    _FunctionBoxLineEdit = new QLineEdit;

    _FunctionBoxCombo = new QComboBox;
    _FunctionBoxComboColor = new QComboBox;

    _FunctionBoxComboColor->addItem("Black");
    _FunctionBoxComboColor->addItem("Red");
    _FunctionBoxComboColor->addItem("Green");
    _FunctionBoxComboColor->addItem("Blue");

    _FunctionBoxCombo->addItem("New");

    _FunctionBoxLabel = new QLabel;
    _FunctionBoxLabel->setText("f(x)=");

    _FunctionBoxImportButton.push_back(new QPushButton);
    _FunctionBoxImportButton.push_back(new QPushButton);
    _FunctionBoxImportButton.push_back(new QPushButton);

    _FunctionBoxImportButton[0]->setText("From .txt");
    _FunctionBoxImportButton[1]->setText("From .wav");
    _FunctionBoxImportButton[2]->setText("Clear !");

    _FunctionBoxLayout->addWidget(_FunctionBoxCombo,0,0);
    _FunctionBoxLayout->addWidget(_FunctionBoxLabel,0,1);
    _FunctionBoxLayout->addWidget(_FunctionBoxLineEdit,0,2);
    _FunctionBoxLayout->addWidget(_FunctionBoxImportButton[0],0,3);
    _FunctionBoxLayout->addWidget(_FunctionBoxImportButton[1],0,4);

    _FunctionBoxLayout->addWidget(_FunctionBoxComboColor,1,3);
    _FunctionBoxLayout->addWidget(_FunctionBoxImportButton[2],1,4);

    _FunctionBox->setLayout(_FunctionBoxLayout);

    QObject::connect(_FunctionBoxImportButton[0],SIGNAL(clicked()),this,SLOT(FromTxt()));
    QObject::connect(_FunctionBoxImportButton[1],SIGNAL(clicked()),this,SLOT(FromWav()));
    QObject::connect(_FunctionBoxImportButton[2],SIGNAL(clicked()),this,SLOT(ClearPlot()));
    QObject::connect(_FunctionBoxCombo,SIGNAL(activated(int)),this,SLOT(FunctionIndexChanged()));

}



//create the range box
void OneDWindow::CreateRangeBox(){
    _RangeBox = new QGroupBox(tr("Ranges"));
    _RangeBoxLayout = new QGridLayout;

    _RangeBoxPlotButton = new QPushButton;
    _RangeBoxPlotButton->setText("Plot !");
    _RangeBoxLineEdit.resize(5);
    for(int i=0;i<5;++i){
        _RangeBoxLineEdit[i]=new QLineEdit;
        _RangeBoxLabel.push_back(new QLabel);
    }
    _RangeBoxLabel.push_back(new QLabel);

    _RangeBoxLabel[0]->setText("x0 :");
    _RangeBoxLabel[1]->setText("xn :");
    _RangeBoxLabel[2]->setText("N :");

    _RangeBoxLayout->addWidget(_RangeBoxLabel[0],0,0);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[0],0,1);
    _RangeBoxLayout->addWidget(_RangeBoxLabel[1],0,2);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[1],0,3);
    _RangeBoxLayout->addWidget(_RangeBoxLabel[2],0,4);
    _RangeBoxLayout->addWidget(_RangeBoxLineEdit[2],0,5);

    _RangeBoxLayout->addWidget(_RangeBoxPlotButton,1,5);


    _RangeBox->setLayout(_RangeBoxLayout);

    QObject::connect(_RangeBoxPlotButton,SIGNAL(clicked()),this,SLOT(Plot()));
}


void OneDWindow::FromTxt(){
    _FunctionBoxDialogTxt = new QFileDialog(this,tr("Open txt"),"./",tr("Text files (*.txt)"));
    _FunctionBoxDialogTxt->show();
    if(_FunctionBoxDialogTxt->exec()){
        QStringList files = _FunctionBoxDialogTxt->selectedFiles();
        std::ifstream f(files.at(0).toStdString());
        //float v;
        if(_FunctionBoxCombo->currentIndex()==0){
            _FunctionBoxCombo->addItem(QString::number((int)_FunctionBoxCombo->count()));
            _FunctionBoxCombo->setCurrentIndex(_FunctionBoxCombo->count()-1);
            _OpenGLWindow->_MeshVector.push_back(Mesh1D ());
            _OpenGLWindow->_MeshColor.push_back(_FunctionBoxComboColor->currentText().toStdString());
            _FunctionBoxSource.push_back(QString::fromStdString("From "+files.at(0).toStdString()));
        }
        else{
            _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]=Mesh1D ();
            _FunctionBoxSource[_FunctionBoxCombo->currentIndex()-1]=QString::fromStdString("From "+files.at(0).toStdString());
        }
  //      while(f>>v){
  //          _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._Y.push_back(v);
  //      }
        _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X=arma::vec (_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._Y.n_elem);
        for(unsigned int i=0;i<_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._Y.n_elem;++i)
            _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X[i]=i;
        //update info on mesh painter
 //       _OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._N=_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector._N-1]._Y._N;
        _OpenGLWindow->Xmax=_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X.max();
        _OpenGLWindow->Xmin=_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X.min();
        _OpenGLWindow->Ymax=_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._Y.max();
        _OpenGLWindow->Ymin=_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._Y.min();
        _OpenGLWindow->update();
        f.close();
    }
    delete _FunctionBoxDialogTxt;
}

void OneDWindow::FromWav(){/*
    _FunctionBoxDialogTxt = new QFileDialog(this,tr("Open wav"),"./",tr("Wav files (*.wav)"));
    _FunctionBoxDialogTxt->show();
    if(_FunctionBoxDialogTxt->exec()){
        QStringList files = _FunctionBoxDialogTxt->selectedFiles();
        WAV wav(files.at(0).toStdString());
        if(_FunctionBoxCombo->currentIndex()==0){
            _FunctionBoxCombo->addItem(QString::number((int)_FunctionBoxCombo->count()));
            _FunctionBoxCombo->setCurrentIndex(_FunctionBoxCombo->count()-1);
            _OpenGLWindow->_MeshVector.push_back(Mesh1D ());
            //_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._Y=wav._Values;
            _OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._X=Varma::vec (wav._Values._N);
            for(unsigned int i=0;i<wav._Values._N;++i)
                _OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector._N-1]._X[i]=i;
            _OpenGLWindow->Xmin=0;
            _OpenGLWindow->Xmax=wav._Values._N;
            _OpenGLWindow->Ymin=wav._Values.min();
            _OpenGLWindow->Ymax=wav._Values.max();
            _OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector._N-1]._N=wav._Values._N;
            _OpenGLWindow->_MeshColor.append(_FunctionBoxComboColor->currentText().toStdString());
            _FunctionBoxSource.append(QString::fromStdString("From "+files.at(0).toStdString()));
        }
        _OpenGLWindow->update();
    }*/
}

void OneDWindow::FunctionIndexChanged(){
    int i=_FunctionBoxCombo->currentIndex();
    if(i==0)
        _FunctionBoxLineEdit->setText("");
    else{
    if(_OpenGLWindow->_MeshColor[i-1]=="Black")
        _FunctionBoxComboColor->setCurrentIndex(0);
    else if(_OpenGLWindow->_MeshColor[i-1]=="Red")
        _FunctionBoxComboColor->setCurrentIndex(1);
    else if(_OpenGLWindow->_MeshColor[i-1]=="Green")
        _FunctionBoxComboColor->setCurrentIndex(2);
    else
        _FunctionBoxComboColor->setCurrentIndex(3);
    _FunctionBoxLineEdit->setText(_FunctionBoxSource[i-1]);
    }
}

void OneDWindow::Plot(){
    if(_FunctionBoxCombo->currentIndex()==0){
        _OpenGLWindow->_MeshColor.push_back(_FunctionBoxComboColor->currentText().toStdString());
        int a=_FunctionBoxCombo->count();
        std::string F=_FunctionBoxLineEdit->text().toStdString();
        Tree T(F);
        _FunctionBoxSource.push_back(QString::fromStdString(T.toString()));
        _FunctionBoxCombo->addItem(QString::number(a));
        _FunctionBoxCombo->setCurrentIndex(a);
        _OpenGLWindow->AddMesh(_RangeBoxLineEdit[0]->text().toFloat(),_RangeBoxLineEdit[1]->text().toFloat(),_RangeBoxLineEdit[2]->text().toUInt());
        _OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._Y=T(_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._X);
        _OpenGLWindow->Ymin=_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._Y.min();
        _OpenGLWindow->Ymax=_OpenGLWindow->_MeshVector[_OpenGLWindow->_MeshVector.size()-1]._Y.max();
        _OpenGLWindow->update();
    }
    else{
        std::string F=_FunctionBoxLineEdit->text().toStdString();
        Tree T(F);
        _FunctionBoxSource[_FunctionBoxCombo->currentIndex()-1]=QString::fromStdString(T.toString());
        _OpenGLWindow->_MeshColor[_FunctionBoxCombo->currentIndex()-1]=_FunctionBoxComboColor->currentText().toStdString();
        _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]=Mesh1D (_RangeBoxLineEdit[0]->text().toFloat(),_RangeBoxLineEdit[1]->text().toFloat(),_RangeBoxLineEdit[2]->text().toUInt());
        _OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._Y=T(_OpenGLWindow->_MeshVector[_FunctionBoxCombo->currentIndex()-1]._X);
        _OpenGLWindow->update();
    }
}

void OneDWindow::Trig(){
    cout<<"TRIG "<<_FunctionNumberMenu->currentIndex()<<endl;
}

void OneDWindow::ClearPlot(){
    unsigned int CI=_FunctionBoxCombo->currentIndex();
    if(CI!=0){
        //_OpenGLWindow->_MeshVector.erase(CI-1);
       // _OpenGLWindow->_MeshColor.erase(CI-1);
        _FunctionBoxCombo->clear();
        _FunctionBoxCombo->addItem("NEW");
        for(unsigned int i=0;i<_OpenGLWindow->_MeshVector.size();++i){
            if(CI-1==i) i++;
            _FunctionBoxCombo->addItem(QString::number(i));
        }
    }
    else{
        _OpenGLWindow->_MeshVector.clear();
        _OpenGLWindow->_MeshColor.clear();
        _FunctionBoxCombo->clear();
        _FunctionBoxCombo->addItem("NEW");
    }
    _OpenGLWindow->update();
}

void OneDWindow::PopUpMenu(){
    QMainWindow* menu=new MainWindow(_Parent);
    menu->show();
    this->hide();    
}

void OneDWindow::CreateGLWidget(){
    _OpenGLWindow=new OneDMeshPainter;
    _OpenGLWindow->setStyleSheet("background-color:black;");
}

