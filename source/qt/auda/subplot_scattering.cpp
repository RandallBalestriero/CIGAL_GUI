#include "./subplot_scattering.h"

SubplotScattering::SubplotScattering(QDialog *){
    this->setWindowTitle("Scattering");
    this->setMinimumWidth(500);


    this->_LeftLayout= new QGridLayout(this);
    this->_Labels[0]=new QLabel("Normalization : ",this);
    this->_Labels[1]=new QLabel("T : ",this);
    this->_Labels[2]=new QLabel("Q : ",this);
    this->_Labels[3]=new QLabel("J : ",this);

    this->_LineEdits[0]=new QLineEdit("32",this);
    this->_LineEdits[1]=new QLineEdit("8",this);
    this->_LineEdits[2]=new QLineEdit("5",this);


    this->_PushButtons[0]=new QPushButton(this);
    this->_PushButtons[0]->setText("Refresh");
    this->_PushButtons[1]=new QPushButton(this);
    this->_PushButtons[1]->setText("Export");

    this->_ComboBox = new QComboBox(this);
    this->_ComboBox->addItem("log(x+eps)");
    this->_ComboBox->addItem("log(1+x)");
    this->_ComboBox->addItem("sqrt(x,3)");
    this->_ComboBox->addItem("x");
    this->_ComboBox->setCurrentIndex(3);


    this->_LeftLayout->addWidget(this->_Labels[0],0,0);
    this->_LeftLayout->addWidget(this->_ComboBox,0,1);
    this->_LeftLayout->addWidget(this->_Labels[1],1,0);
    this->_LeftLayout->addWidget(this->_LineEdits[0],1,2);
    this->_LeftLayout->addWidget(this->_Labels[2],2,0);
    this->_LeftLayout->addWidget(this->_LineEdits[1],2,1);
    this->_LeftLayout->addWidget(this->_Labels[3],3,0);
    this->_LeftLayout->addWidget(this->_LineEdits[2],3,1);
    this->_LeftLayout->addWidget(this->_PushButtons[0],4,0,1,2);
    this->_LeftLayout->addWidget(this->_PushButtons[1],5,0,1,2);


    this->_RightWidget = new ImagescWidget(this);
    this->_MainLayout = new QHBoxLayout;

    this->_LeftWidget = new QWidget(this);
    this->_LeftWidget->setLayout(this->_LeftLayout);


    this->_LeftWidget->setMaximumWidth(150);

    this->_MainLayout->addWidget(this->_LeftWidget);
    this->_MainLayout->addWidget(this->_RightWidget);

    this->setLayout(this->_MainLayout);
}
