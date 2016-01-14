#include "./window.h"
#include "../geometry/geometry.h"
#include <QComboBox>

AudaWindow::AudaWindow(QMainWindow*){
    this->setWindowTitle("Auda !");

    this->_SubplotHelp = new QLabel(this);
    this->_SubplotHelp->setText("FOR NOW :\n    File -> Load wav ... \n    Spectrogram ? ->Transformations -> Spectrogram\n    FFT ? -> Transformations -> FFT\n    Partial FFT (given a time window) ? -> left click on signal to create selection then perform FFT\n    ZOOM with mouse wheel");
    //creating the menus and menu bar :
    this->_Menubar = new QMenuBar;
    this->_FileMenu = new QMenu("File",this);
    this->_TransformMenu = new QMenu("Transform",this);
    this->_OperationMenu = new QMenu("OPerations",this);

    this->_MenuActions.resize(8);

    this->_MenuActions[0]=new QAction("Load .wav",this);
    this->_MenuActions[1]=new QAction("Load .txt",this);
    this->_MenuActions[2]=new QAction("Save .wav",this);
    this->_MenuActions[3]=new QAction("Save .txt",this);
    this->_FileMenu->addAction(this->_MenuActions[0]);
    this->_FileMenu->addAction(this->_MenuActions[1]);
    this->_FileMenu->addAction(this->_MenuActions[2]);
    this->_FileMenu->addAction(this->_MenuActions[3]);


    this->_MenuActions[4]=new QAction("FFT",this);
    this->_MenuActions[5]=new QAction("Spectrogram",this);
    this->_MenuActions[6]=new QAction("Scattering",this);
    this->_TransformMenu->addAction(this->_MenuActions[4]);
    this->_TransformMenu->addAction(this->_MenuActions[5]);
    this->_TransformMenu->addAction(this->_MenuActions[6]);

    this->_MenuActions[7]=new QAction("Apply Operation",this);
    this->_OperationMenu->addAction(this->_MenuActions[7]);

    _Menubar->addMenu(this->_FileMenu);
    _Menubar->addMenu(this->_TransformMenu);
    _Menubar->addMenu(this->_OperationMenu);


    this->_MainLayout = new QVBoxLayout;
    this->_MainLayout->setMenuBar(this->_Menubar);
    this->_SubplotWav = new SubplotWav(this);
    QObject::connect(this->_SubplotWav->_RightWidget,SIGNAL(XChanged()),this,SLOT(ZoomSubplotWav()));


    this->_ScrollBar = new QScrollBar;
    this->_ScrollBar->setOrientation(Qt::Horizontal);
    this->_ScrollBar->setMinimum(0);
    QObject::connect(this->_ScrollBar,SIGNAL(sliderMoved(int)),this,SLOT(ScrollBarMoved()));

    this->_SubplotWav->hide();
    this->_ScrollBar->hide();

    this->_SubplotScattering = new SubplotScattering(this);
    this->_SubplotScattering->hide();
    QObject::connect(this->_SubplotScattering->_PushButtons[0],SIGNAL(clicked()),this,SLOT(RefreshScattering()));
    QObject::connect(this->_SubplotScattering->_RightWidget,SIGNAL(XChanged()),this,SLOT(ZoomSubplotScattering()));

    this->_SubplotSpectro = new SubplotSpectro(this);
    this->_SubplotSpectro->hide();
    QObject::connect(this->_SubplotSpectro->_RightWidget,SIGNAL(XChanged()),this,SLOT(ZoomSubplotSpectro()));
    QObject::connect(this->_SubplotSpectro->_RefreshButton,SIGNAL(clicked()),this,SLOT(RefreshSpectro()));


    this->_MainLayout->addWidget(this->_SubplotHelp);
    this->_MainLayout->addWidget(_SubplotWav);
    this->_MainLayout->addWidget(_ScrollBar);
    this->_MainLayout->addWidget(_SubplotSpectro);
    this->_MainLayout->addWidget(_SubplotScattering);


    QObject::connect(_MenuActions[4],SIGNAL(triggered()),this,SLOT(AddFFT()));
    QObject::connect(_MenuActions[5],SIGNAL(triggered()),this,SLOT(AddSpectro()));
    QObject::connect(_MenuActions[6],SIGNAL(triggered()),this,SLOT(AddScattering()));

    QObject::connect(_MenuActions[0],SIGNAL(triggered()),this,SLOT(LoadWav()));

    this->setLayout(_MainLayout);
    this->setGeometry(200,200,500,500);
}

void AudaWindow::AddFFT(){
    if(this->_SubplotWav->_RightWidget->rubberband){
        float coeff=(this->_SubplotWav->_RightWidget->_Xlim[1]-this->_SubplotWav->_RightWidget->_Xlim[0])/(float)this->_SubplotWav->_RightWidget->width() ;
        int Start=this->_SubplotWav->_RightWidget->origin.x()*coeff;
        int End=Start+this->_SubplotWav->_RightWidget->rubberband->geometry().width() *coeff;
        tmp=arma::vec (End-Start);
        for(int i=Start;i<End;++i) tmp[i-Start]=this->_SubplotWav->_Wav._Values[i];
        this->_SubplotFFT = new SubplotFFT(this,tmp,this->_SubplotWav->_Wav._SampleRate);
        if(this->_SubplotWav->_RightWidget->rubberband){ delete this->_SubplotWav->_RightWidget->rubberband;this->_SubplotWav->_RightWidget->rubberband=0;}
    }
    else
        this->_SubplotFFT = new SubplotFFT(this,this->_SubplotWav->_Wav._Values,this->_SubplotWav->_Wav._SampleRate);
    this->_SubplotFFT->show();
}

void AudaWindow::AddSpectro(){
    _Spectro.Perform(this->_SubplotWav->_Wav._Values);
    Normalize(this->_Spectro._Coefficients);
    SignalSpectrogramRatio=(double)this->_SubplotWav->_Wav._Values.n_elem/this->_Spectro._Coefficients.n_cols;
    this->_SubplotSpectro->_RightWidget->_Values=_Spectro._Coefficients;

    this->_SubplotSpectro->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalSpectrogramRatio;
    this->_SubplotSpectro->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalSpectrogramRatio;
    this->_SubplotSpectro->_RightWidget->_Ylimit[0]=0;
    this->_SubplotSpectro->_RightWidget->_Ylimit[1]=this->_Spectro._Coefficients.n_rows;
    this->_SubplotSpectro->_RightWidget->_XaxisScaling=this->_SubplotWav->_Wav._Values.n_elem/(this->_SubplotWav->_Wav._SampleRate*(double)this->_Spectro._Coefficients.n_cols );
    this->_SubplotSpectro->_RightWidget->_XaxisLabel="Time in sec.";

    this->_SubplotSpectro->show();
}


void AudaWindow::AddScattering(){
    std::vector<arma::vec> input(1);input[0]=this->_SubplotWav->_Wav._Values;
    this->_ScatteringLayer=ScatteringLayer(input,64,8,4);
    this->_SubplotScattering->_RightWidget->_Values=this->_ScatteringLayer._Scattering;
    SignalScatteringRatio=this->_SubplotWav->_Wav._Values.n_elem/(double)this->_ScatteringLayer._Scattering.n_cols;
    this->_SubplotScattering->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalScatteringRatio;
    this->_SubplotScattering->_RightWidget->_Ylimit[0]=0;
    this->_SubplotScattering->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalScatteringRatio;
    this->_SubplotScattering->_RightWidget->_Ylimit[1]=this->_ScatteringLayer._Scattering.n_rows;
    this->_SubplotScattering->_RightWidget->_XaxisLabel="Time in sec.";
    this->_SubplotScattering->_RightWidget->_XaxisScaling=this->_SubplotWav->_RightWidget->_XaxisScaling/this->_ScatteringLayer._Scattering.n_cols;
    this->_SubplotScattering->show();
}

void AudaWindow::ZoomSubplotWav(){
    if((int)this->_SubplotWav->_Wav._Values.n_elem==this->_SubplotWav->_RightWidget->_Xlim[1] && this->_SubplotWav->_RightWidget->_Xlim[0]==0)
        this->_ScrollBar->hide();
    else{
        this->_ScrollBar->show();
        this->_ScrollBar->setMaximum(10*this->_SubplotWav->_Wav._Values.n_elem/(_SubplotWav->_RightWidget->_Xlim[1]-_SubplotWav->_RightWidget->_Xlim[0]));
        this->_ScrollBar->setSliderPosition(this->_ScrollBar->maximum()*(_SubplotWav->_RightWidget->_Xlim[0])/((float)this->_SubplotWav->_Wav._Values.n_elem)+1);
    }
    this->_SubplotSpectro->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalSpectrogramRatio;
    this->_SubplotSpectro->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalSpectrogramRatio;
    this->_SubplotScattering->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalScatteringRatio;
    this->_SubplotScattering->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalScatteringRatio;
    this->_SubplotScattering->update();
    this->_SubplotSpectro->update();
}


void AudaWindow::ZoomSubplotSpectro(){
    this->_SubplotWav->_RightWidget->_Xlim[0]=this->_SubplotSpectro->_RightWidget->_Xlimit[0]*SignalSpectrogramRatio;
    if(this->_SubplotSpectro->_RightWidget->_Xlimit[1]*SignalSpectrogramRatio>(int)this->_SubplotWav->_RightWidget->_Values->n_elem){
        this->_SubplotWav->_RightWidget->_Xlim[1]=this->_SubplotWav->_Wav._Values.n_elem;
        this->_SubplotSpectro->_RightWidget->_Xlimit[1]=this->_Spectro._Coefficients.n_cols;}
    else this->_SubplotWav->_RightWidget->_Xlim[1]=this->_SubplotSpectro->_RightWidget->_Xlimit[1]*SignalSpectrogramRatio;

    if((int)this->_SubplotSpectro->_RightWidget->_Values.n_cols==this->_SubplotSpectro->_RightWidget->_Xlimit[1]  && this->_SubplotSpectro->_RightWidget->_Xlimit[0]==0)
        this->_ScrollBar->hide();
    else{
        this->_ScrollBar->show();
        this->_ScrollBar->setMaximum(10*this->_SubplotWav->_Wav._Values.n_elem/(_SubplotWav->_RightWidget->_Xlim[1]-_SubplotWav->_RightWidget->_Xlim[0]));
        this->_ScrollBar->setSliderPosition(this->_ScrollBar->maximum()*(_SubplotWav->_RightWidget->_Xlim[0])/((float)this->_SubplotWav->_Wav._Values.n_elem));
    }
    this->_SubplotScattering->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalScatteringRatio;
    this->_SubplotScattering->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalScatteringRatio;
    this->_SubplotWav->update();
    this->_SubplotScattering->update();
}


void AudaWindow::ZoomSubplotScattering(){
    this->_SubplotWav->_RightWidget->_Xlim[0]=this->_SubplotScattering->_RightWidget->_Xlimit[0]*SignalScatteringRatio;

    if(this->_SubplotScattering->_RightWidget->_Xlimit[1]*SignalScatteringRatio>(int)this->_SubplotWav->_RightWidget->_Values->n_elem){
        this->_SubplotWav->_RightWidget->_Xlim[1]=this->_SubplotWav->_Wav._Values.n_elem;
        this->_SubplotScattering->_RightWidget->_Xlimit[1]=this->_SubplotScattering->_RightWidget->_Values.n_cols;}
    else this->_SubplotWav->_RightWidget->_Xlim[1]=this->_SubplotScattering->_RightWidget->_Xlimit[1]*SignalScatteringRatio;

    if((int)this->_SubplotScattering->_RightWidget->_Values.n_cols==this->_SubplotScattering->_RightWidget->_Xlimit[1]  && this->_SubplotScattering->_RightWidget->_Xlimit[0]==0)
        this->_ScrollBar->hide();
    else{
        this->_ScrollBar->show();
        this->_ScrollBar->setMaximum(10*this->_SubplotWav->_Wav._Values.n_elem/(_SubplotWav->_RightWidget->_Xlim[1]-_SubplotWav->_RightWidget->_Xlim[0]));
        this->_ScrollBar->setSliderPosition(this->_ScrollBar->maximum()*(_SubplotWav->_RightWidget->_Xlim[0])/((float)this->_SubplotWav->_Wav._Values.n_elem));
    }
    this->_SubplotSpectro->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalSpectrogramRatio;
    this->_SubplotSpectro->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalSpectrogramRatio;
    this->_SubplotSpectro->update();
    this->_SubplotWav->update();
}

void AudaWindow::ScrollBarMoved(){
    int Range=this->_SubplotWav->_RightWidget->_Xlim[1]-this->_SubplotWav->_RightWidget->_Xlim[0];
    this->_SubplotWav->_RightWidget->_Xlim[0]=(this->_ScrollBar->value())*0.1*Range;
    this->_SubplotWav->_RightWidget->_Xlim[1]=(this->_ScrollBar->value())*0.1*Range+Range;

    if(this->_SubplotWav->_RightWidget->_Xlim[1]>(int)this->_SubplotWav->_Wav._Values.n_elem) {
        this->_SubplotWav->_RightWidget->_Xlim[0]=this->_SubplotWav->_Wav._Values.n_elem-Range;
        this->_SubplotWav->_RightWidget->_Xlim[1]=this->_SubplotWav->_Wav._Values.n_elem;
    }
    this->_SubplotSpectro->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalSpectrogramRatio;
    this->_SubplotSpectro->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalSpectrogramRatio;
    if(this->_SubplotScattering){
    this->_SubplotScattering->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalScatteringRatio;
    this->_SubplotScattering->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalScatteringRatio;    }

    this->_SubplotWav->_RightWidget->update();
    this->_SubplotSpectro->_RightWidget->update();
    this->_SubplotScattering->update();

}

void AudaWindow::LoadWav(){
    this->_SubplotWav->LoadWav();
    this->_SubplotSpectro->_RightWidget->_Values.clear();
    this->_SubplotHelp->hide();
    this->_SubplotSpectro->hide();
    this->_SubplotScattering->hide();
}

void AudaWindow::RefreshSpectro(){
    if(this->_SubplotSpectro->_LineEditOverlapp->text().toFloat()!=this->_Spectro._overlapp || this->_SubplotSpectro->_LineEditWindow->text().toFloat()!=this->_Spectro._window){
        _Spectro.Perform(this->_SubplotWav->_Wav._Values,this->_SubplotSpectro->_LineEditWindow->text().toFloat(),this->_SubplotSpectro->_LineEditOverlapp->text().toFloat());
        SignalSpectrogramRatio=(double)this->_SubplotWav->_Wav._Values.n_elem/this->_Spectro._Coefficients.n_cols;
        this->_SubplotSpectro->_RightWidget->_XaxisScaling=this->_SubplotWav->_Wav._Values.n_elem/(this->_SubplotWav->_Wav._SampleRate*(double)this->_Spectro._Coefficients.n_cols );
        Normalize(this->_Spectro._Coefficients);
        }
    if(this->_SubplotSpectro->_ComboBox->currentIndex()==0){
        this->_SubplotSpectro->_RightWidget->_Values=arma::log(0.00000001+this->_Spectro._Coefficients);}
    else if(this->_SubplotSpectro->_ComboBox->currentIndex()==1){
        this->_SubplotSpectro->_RightWidget->_Values=arma::log(1+this->_Spectro._Coefficients);}
    else if(this->_SubplotSpectro->_ComboBox->currentIndex()==2){
        this->_SubplotSpectro->_RightWidget->_Values=arma::sqrt(this->_Spectro._Coefficients);}
    else{this->_SubplotSpectro->_RightWidget->_Values=this->_Spectro._Coefficients;}

    this->_SubplotSpectro->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalSpectrogramRatio;
    this->_SubplotSpectro->_RightWidget->_Xlimit[0]=this->_SubplotWav->_RightWidget->_Xlim[0]/SignalSpectrogramRatio;
    this->_SubplotSpectro->_RightWidget->_Ylimit[1]=this->_Spectro._Coefficients.n_rows;
    this->_SubplotSpectro->_RightWidget->update();
}

void AudaWindow::RefreshScattering(){
    if(this->_SubplotScattering->_LineEdits[0]->text().toInt()!=this->_ScatteringLayer._FilterBank._T ||
            this->_SubplotScattering->_LineEdits[1]->text().toInt()!=this->_ScatteringLayer._FilterBank._Q ||
            this->_SubplotScattering->_LineEdits[2]->text().toInt()!=this->_ScatteringLayer._FilterBank._J){
        std::vector<arma::vec> input(1);input[0]=this->_SubplotWav->_Wav._Values;
        this->_ScatteringLayer=ScatteringLayer(input,this->_SubplotScattering->_LineEdits[0]->text().toFloat(),this->_SubplotScattering->_LineEdits[1]->text().toFloat(),this->_SubplotScattering->_LineEdits[2]->text().toFloat());
        Normalize(this->_ScatteringLayer._Scattering);
        SignalScatteringRatio=this->_SubplotWav->_Wav._Values.n_elem/(double)this->_ScatteringLayer._Scattering.n_cols;
        this->_SubplotScattering->_RightWidget->_Xlimit[0]=std::floor(this->_SubplotWav->_RightWidget->_Xlim[0]/SignalScatteringRatio);
        this->_SubplotScattering->_RightWidget->_Ylimit[0]=0;
        this->_SubplotScattering->_RightWidget->_Xlimit[1]=this->_SubplotWav->_RightWidget->_Xlim[1]/SignalScatteringRatio;
        this->_SubplotScattering->_RightWidget->_Ylimit[1]=this->_ScatteringLayer._Scattering.n_rows;
        this->_SubplotScattering->_RightWidget->_XaxisLabel="Time in sec.";
        this->_SubplotScattering->_RightWidget->_XaxisScaling=this->_SubplotWav->_RightWidget->_XaxisScaling/this->_ScatteringLayer._Scattering.n_cols;
    }
    if(this->_SubplotScattering->_ComboBox->currentIndex()==0)
        this->_SubplotScattering->_RightWidget->_Values=arma::log(0.000000001+this->_ScatteringLayer._Scattering);
    else if(this->_SubplotScattering->_ComboBox->currentIndex()==1)
        this->_SubplotScattering->_RightWidget->_Values=arma::log(1+this->_ScatteringLayer._Scattering);
    else if(this->_SubplotScattering->_ComboBox->currentIndex()==2)
        this->_SubplotScattering->_RightWidget->_Values=arma::sqrt(this->_ScatteringLayer._Scattering);
    else
        this->_SubplotScattering->_RightWidget->_Values=this->_ScatteringLayer._Scattering;

    this->_SubplotScattering->_RightWidget->update();
}
