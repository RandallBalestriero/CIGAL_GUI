#include "./subplot_fft.h"
#include "../../cigal/wav.h"
#include "../../cigal/fft.h"
#include <complex.h>
#include <fftw3.h>



SubplotFFT::SubplotFFT(QDialog * parent, arma::vec & wav, const int & Fs){
    this->setWindowTitle("Fast Fourier Transform");
    this->setMinimumWidth(500);
    this->_RightWidget = new PlottingFFTWidget(this);
    this->Compute(wav);
    this->_RightWidget->NyqFs=Fs/2;
    this->_MainLayout = new QHBoxLayout;

    this->_LeftWidget = new QWidget(this);
    this->_LeftWidgetLayout = new QGridLayout;
    this->_LeftWidgetPushButton = new QPushButton(parent);
    this->_LeftWidgetPushButton->setText("Export");

    this->_LeftWidgetLabelFFTInfo[0] = new QLabel(this->_LeftWidget);
    this->_LeftWidgetLabelFFTInfo[0]->setText("FFT :");
    this->_LeftWidgetLabelFFTInfo[1] = new QLabel(this->_LeftWidget);
    this->_LeftWidgetLabelFFTInfo[1]->setText("Nyquist Freq :"+QString::number(Fs/2));
    this->_LeftWidgetLabelFFTInfo[2] = new QLabel(this->_LeftWidget);
    this->_LeftWidgetLabelFFTInfo[2]->setText("in "+QString::number(wav.n_elem/2)+" points");

    this->_CheckBoxImag = new QCheckBox(this);
    this->_CheckBoxImag->setCheckState(Qt::Unchecked);
    this->_CheckLabel3 = new QLabel(this);
    this->_CheckLabel3->setText("Imaginary part");
    this->_CheckBoxMod = new QCheckBox(this);
    this->_CheckBoxMod->setCheckState(Qt::Checked);
    this->_CheckLabel1 = new QLabel(this);
    this->_CheckLabel1->setText("Modulus");
    this->_CheckBoxReal = new QCheckBox(this);
    this->_CheckBoxReal->setCheckState(Qt::Unchecked);
    this->_CheckLabel2 = new QLabel(this);
    this->_CheckLabel2->setText("Real Part");
    QObject::connect(this->_CheckBoxImag,SIGNAL(stateChanged(int)),this,SLOT(CheckedChanged()));
    QObject::connect(this->_CheckBoxReal,SIGNAL(stateChanged(int)),this,SLOT(CheckedChanged()));
    QObject::connect(this->_CheckBoxMod,SIGNAL(stateChanged(int)),this,SLOT(CheckedChanged()));



    this->_LeftWidgetLayout->addWidget(this->_LeftWidgetPushButton,0,0,1,2);
    this->_LeftWidgetLayout->addWidget(this->_LeftWidgetLabelFFTInfo[0],1,0,1,2);
    this->_LeftWidgetLayout->addWidget(this->_LeftWidgetLabelFFTInfo[1],2,0,1,2);
    this->_LeftWidgetLayout->addWidget(this->_LeftWidgetLabelFFTInfo[2],3,0,1,2);
    this->_LeftWidgetLayout->addWidget(this->_CheckBoxMod,4,0);
    this->_LeftWidgetLayout->addWidget(this->_CheckLabel1,4,1);
    this->_LeftWidgetLayout->addWidget(this->_CheckBoxReal,5,0);
    this->_LeftWidgetLayout->addWidget(this->_CheckLabel2,5,1);
    this->_LeftWidgetLayout->addWidget(this->_CheckBoxImag,6,0);
    this->_LeftWidgetLayout->addWidget(this->_CheckLabel3,6,1);

    this->_LeftWidget->setLayout(this->_LeftWidgetLayout);

    this->_LeftWidget->setMaximumWidth(150);

    this->_MainLayout->addWidget(this->_LeftWidget);
    this->_MainLayout->addWidget(this->_RightWidget);

    this->setLayout(this->_MainLayout);

    QObject::connect(this->_LeftWidgetPushButton,SIGNAL(clicked()),this,SLOT(ExportFFT()));

}

SubplotFFT::~SubplotFFT(){
    delete _MainLayout;
}

void SubplotFFT::Compute(arma::vec & wav){
    this->_FFT.resize(3);
    fftw_complex*  in=new fftw_complex[wav.n_elem/2+1];
    fftw_plan PP;
    PP=fftw_plan_dft_r2c_1d(wav.n_elem, wav.memptr(),in,FFTW_ESTIMATE);
    fftw_execute(PP);
    fftw_destroy_plan(PP);

    this->_FFT[0]=arma::vec(wav.n_elem/2+1);
    this->_FFT[1]=arma::vec(wav.n_elem/2+1);
    this->_FFT[2]=arma::vec(wav.n_elem/2+1);
    for(unsigned int i=0;i<wav.n_elem/2+1;++i){
        _FFT[1][i]=in[i][0]/wav.n_elem;
        _FFT[2][i]=in[i][1]/wav.n_elem;
        _FFT[0][i]=std::sqrt(_FFT[1][i]*_FFT[1][i]+_FFT[2][i]*_FFT[2][i]);
    }

    delete[] in;

    this->_RightWidget->_Values.resize(3);
    this->_RightWidget->_Values[0]=&this->_FFT[0];
    this->_RightWidget->_Values[1]=&this->_FFT[1];
    this->_RightWidget->_Values[2]=&this->_FFT[2];
    this->_RightWidget->update();
}

void SubplotFFT::ExportFFT(){
    this->_LeftWidgetQFileDialog = new QFileDialog(this,tr("Open FFT"),"./",tr("FFT files (*.FFT)"));
    this->_LeftWidgetQFileDialog->show();
    QStringList files;
    if(this->_LeftWidgetQFileDialog->exec()){
        files = this->_LeftWidgetQFileDialog->selectedFiles();
       // _FFT<<(files.at(0).toStdString());
    }
}

void SubplotFFT::CheckedChanged(){
    this->_RightWidget->Checked[0]=this->_CheckBoxMod->checkState()==Qt::Checked?1:0;
    this->_RightWidget->Checked[1]=this->_CheckBoxReal->checkState()==Qt::Checked?1:0;
    this->_RightWidget->Checked[2]=this->_CheckBoxImag->checkState()==Qt::Checked?1:0;
    this->_RightWidget->update();
}
