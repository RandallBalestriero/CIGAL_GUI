#include "./subplot_spectro.h"
#include "../../cigal/wav.h"


SubplotSpectro::SubplotSpectro(QDialog *){
    this->setWindowTitle("Fast Fourier Transform");
    this->setMinimumWidth(500);
    _Font = QFont();
    _Font.setPointSize(7);

    this->_RightWidget = new ImagescWidget(this);
    this->_MainLayout = new QHBoxLayout;

    this->_LeftWidget = new QWidget(this);

    this->_LeftWidgetLayout = new QGridLayout;
    this->_LeftWidgetPushButton = new QPushButton(this);
    this->_LeftWidgetPushButton->setText("Export");
    this->_RefreshButton = new QPushButton(this);
    this->_RefreshButton->setText("Refresh");

    this->_CheckLabel3 = new QLabel(this);
    this->_CheckLabel3->setText("Normalization :");
    this->_CheckLabel3->setFont(_Font);

    this->_CheckLabel1 = new QLabel(this);
    this->_CheckLabel1->setFont(_Font);
    this->_CheckLabel1->setText("Overlapp :");

    this->_CheckLabel2 = new QLabel(this);
    this->_CheckLabel2->setFont(_Font);
    this->_CheckLabel2->setText("Window Size :");


    this->_ComboBox = new QComboBox(this);
    this->_ComboBox->addItem("log(x+eps)");
    this->_ComboBox->addItem("log(1+x)");
    this->_ComboBox->addItem("sqrt(x,3)");
    this->_ComboBox->addItem("x");
    this->_ComboBox->setCurrentIndex(3);

    this->_LineEditOverlapp = new QLineEdit(this);
    this->_LineEditWindow = new QLineEdit(this);
    this->_LineEditOverlapp->setText("0.5");
    this->_LineEditWindow->setText("512");

    this->_LeftWidgetLayout->addWidget(this->_LeftWidgetPushButton,4,0,1,2);
    this->_LeftWidgetLayout->addWidget(this->_RefreshButton,3,0,1,2);
    this->_LeftWidgetLayout->addWidget(this->_LineEditOverlapp,2,1);
    this->_LeftWidgetLayout->addWidget(this->_CheckLabel1,2,0);
    this->_LeftWidgetLayout->addWidget(this->_LineEditWindow,1,1);
    this->_LeftWidgetLayout->addWidget(this->_CheckLabel2,1,0);
    this->_LeftWidgetLayout->addWidget(this->_ComboBox,0,1);
    this->_LeftWidgetLayout->addWidget(this->_CheckLabel3,0,0);

    this->_LeftWidget->setLayout(this->_LeftWidgetLayout);

    this->_LeftWidget->setMaximumWidth(150);

    this->_MainLayout->addWidget(this->_LeftWidget);
    this->_MainLayout->addWidget(this->_RightWidget);

    this->setLayout(this->_MainLayout);

    QObject::connect(this->_LeftWidgetPushButton,SIGNAL(clicked()),this,SLOT(ExportFFT()));
}


void SubplotSpectro::ExportFFT(){
    this->_LeftWidgetQFileDialog = new QFileDialog(this,tr("Open FFT"),"./",tr("FFT files (*.FFT)"));
    this->_LeftWidgetQFileDialog->show();
    QStringList files;
    if(this->_LeftWidgetQFileDialog->exec()){
        files = this->_LeftWidgetQFileDialog->selectedFiles();
       // _FFT<<(files.at(0).toStdString());
    }
}


