#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>
#include "./main_window.h"
#include "./plotting2/2D_window.h"
#include "./fem2/fem_window.h"
#include "./qt/auda/window.h"

MainWindow::MainWindow(QMainWindow *parent):QMainWindow(parent){
        //create the two Communicate
    if(parent){
        this->resize(parent->width(),parent->height());
        this->move(parent->x(),parent->y());
    }
    else{
        QDesktopWidget *desktop = QApplication::desktop();//retrieve screen size
        this->resize(250,600);
        this->move(desktop->width()/2-this->width()/2,desktop->height()/2-this->height()/2);
    }
    CreateLayout();
    CreateConnect();
}

MainWindow::~MainWindow(){
    for(unsigned int i=0;i<_QHBoxLayoutVector.size();++i)
        delete _QHBoxLayoutVector[i];
    for(unsigned int i=0;i<_ButtonVector.size();++i)
        delete _ButtonVector[i];

    }

void MainWindow::CreateLayout(){
    _MainLayout=new QVBoxLayout;
    _ButtonVector.resize(6);
    _QHBoxLayoutVector.resize(4);

    _QHBoxLayoutVector[0]=new QHBoxLayout;
    _ButtonVector[0]=new QPushButton("Plotting 1D",this);
    _ButtonVector[1]=new QPushButton("Plotting 2D",this);
    _ButtonVector[0]->setMaximumWidth(200);
    _ButtonVector[1]->setMaximumWidth(200);
    _QHBoxLayoutVector[0]->addWidget(_ButtonVector[0]);
    _QHBoxLayoutVector[0]->addWidget(_ButtonVector[1]);

    _ButtonVector[2]=new QPushButton("Signal Analysis",this);
    _ButtonVector[2]->setMaximumWidth(200);
    _QHBoxLayoutVector[1]=new QHBoxLayout;
    _QHBoxLayoutVector[1]->addWidget(_ButtonVector[2]);


    _QHBoxLayoutVector[2]=new QHBoxLayout;
    _ButtonVector[3]=new QPushButton(" PROJECT : MEF 2D",this);
    _ButtonVector[3]->setMaximumWidth(200);
    _QHBoxLayoutVector[2]->addWidget(_ButtonVector[3]);

    _QHBoxLayoutVector[3]=new QHBoxLayout;
    _ButtonVector[4]=new QPushButton("Help",this);
    _ButtonVector[5]=new QPushButton("Quit",this);
    _ButtonVector[4]->setMaximumWidth(100);
    _ButtonVector[5]->setMaximumWidth(100);
    _QHBoxLayoutVector[3]->addWidget(_ButtonVector[4]);
    _QHBoxLayoutVector[3]->addWidget(_ButtonVector[5]);

    _MainLayout->addLayout(_QHBoxLayoutVector[0]);
    _MainLayout->addLayout(_QHBoxLayoutVector[1]);
    _MainLayout->addLayout(_QHBoxLayoutVector[2]);
    _MainLayout->addLayout(_QHBoxLayoutVector[3]);
    _CentralWidget = new QWidget();
    _CentralWidget->setLayout(_MainLayout);
    setCentralWidget(_CentralWidget);

}

void MainWindow::CreateConnect(){
    QObject::connect(_ButtonVector[0],SIGNAL(clicked()),this,SLOT(PopUp1D()));
    QObject::connect(_ButtonVector[1],SIGNAL(clicked()),this,SLOT(PopUp2D()));
    QObject::connect(_ButtonVector[3],SIGNAL(clicked()),this,SLOT(PopUpFEM2D()));
    QObject::connect(_ButtonVector[2],SIGNAL(clicked()),this,SLOT(PopUpAUDA()));
    QObject::connect(_ButtonVector[4],SIGNAL(clicked()),this,SLOT(PopUpHelp()));
}


void MainWindow::PopUp1D(){
    _Pop=new OneDWindow(this);
    _Pop->show();
    this->hide();
}

void MainWindow::PopUp2D(){
    _Pop=new TwoDWindow(this);
    _Pop->show();
    this->hide();
}

void MainWindow::PopUpFEM2D(){
    _Pop=new FEMTwoDWindow(this);
    _Pop->show();
    this->hide();
}
void MainWindow::PopUpAUDA(){
    _Pop=new AudaWindow(this);
    _Pop->show();
    this->hide();
}

void MainWindow::PopUpHelp(){
    QDialog l(this);
    QGridLayout layout;
    std::string s("This GUI aims to use all the key features of this library.\n");
    s+="The first two sections are here to draw/plot any 1D or 2D functions with\n";
    s+="lots of possibilities.\n";
    s+="The Signal Analysis part provides real time fft, spectrogram and scattering network.\n";
    s+="Finally, the FEM section shows the resolution of the Project problem.";
    QLabel text(s.c_str());
    layout.addWidget(&text);
    l.setLayout(&layout);
    l.resize(300,200);
    l.exec();
    }

