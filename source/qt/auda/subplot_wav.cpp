#include "./subplot_wav.h"
SubplotWav::SubplotWav(QWidget *){
 //   this->setMinimumHeight(300);
    this->_MainLayout = new QHBoxLayout;

    this->_LeftWidget = new QWidget(this);
    this->_LeftWidgetLayout = new QGridLayout;

    this->_LeftWidgetLabelWavInfo[0] = new QLabel(this->_LeftWidget);
    this->_LeftWidgetLabelWavInfo[0]->setText("No wav loaded");
    this->_LeftWidgetLabelWavInfo[1] = new QLabel(this->_LeftWidget);
    this->_LeftWidgetLabelWavInfo[1]->setText("Fs : ");
    this->_LeftWidgetLabelWavInfo[2] = new QLabel(this->_LeftWidget);
    this->_LeftWidgetLabelWavInfo[2]->setText("Dur : ");

    this->_LeftWidgetLayout->addWidget(this->_LeftWidgetLabelWavInfo[0],0,0);
    this->_LeftWidgetLayout->addWidget(this->_LeftWidgetLabelWavInfo[1],1,0);
    this->_LeftWidgetLayout->addWidget(this->_LeftWidgetLabelWavInfo[2],2,0);
    this->_LeftWidget->setLayout(this->_LeftWidgetLayout);

    this->_RightWidget = new PlotWidget(this);
    this->_LeftWidget->setMaximumWidth(150);

    this->_MainLayout->addWidget(this->_LeftWidget);
    this->_MainLayout->addWidget(this->_RightWidget);

    this->setLayout(this->_MainLayout);
}


void SubplotWav::LoadWav(){
    this->_LeftWidgetQFileDialog = new QFileDialog(this,tr("Open wav"),"./",tr("Wav files (*.wav)"));
    this->_LeftWidgetQFileDialog->show();
    QStringList files;
    if(this->_LeftWidgetQFileDialog->exec()){
        files = this->_LeftWidgetQFileDialog->selectedFiles();
        _Wav<<(files.at(0).toStdString());

    this->_LeftWidgetLabelWavInfo[0]->setText(QString::fromStdString(files.at(0).toStdString()));
    this->_LeftWidgetLabelWavInfo[2]->setText("Dur. : "+QString::number(_Wav._Values.n_elem/(float)_Wav._SampleRate)+" sec.");
    this->_LeftWidgetLabelWavInfo[1]->setText("Fs : "+QString::number(_Wav._SampleRate));
//    this->_Wav._Values-=arma::min(this->_Wav._Values);
//    this->_Wav._Values/=arma::max(this->_Wav._Values);
    this->_RightWidget->_Xlim[0]=0;
    this->_RightWidget->_Xlim[1]=_Wav._Values.n_elem;
    this->_RightWidget->_XaxisScaling=1.0/this->_Wav._SampleRate;
    this->_RightWidget->_XaxisLabel="Times in sec.";

 //   this->_RightWidget->Fs=this->_Wav._SampleRate;
    this->_RightWidget->_Values=&_Wav._Values;
    this->show();
    this->_RightWidget->update();
    }
}
