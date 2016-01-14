#ifndef SUBPLOT_FFT
#define SUBPLOT_FFT


#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <vector>
#include <armadillo>
#include "../../cigal/wav.h"
#include <QCheckBox>

#include "../utils/PlottingFFTWidget.h"

class SubplotFFT:public QDialog{
    Q_OBJECT
public:
    PlottingFFTWidget * _RightWidget;
    std::vector<arma::vec> _FFT;

    void Compute(arma::vec &);
    SubplotFFT(QDialog * parent, arma::vec &,int const&);
    ~SubplotFFT();
private:
    QHBoxLayout * _MainLayout;
    QCheckBox *   _CheckBoxImag;
    QLabel  *     _CheckLabel3;
    QCheckBox *   _CheckBoxReal;
    QLabel  *     _CheckLabel2;
    QCheckBox *   _CheckBoxMod;
    QLabel  *     _CheckLabel1;


    QGridLayout * _LeftWidgetLayout;
    QPushButton * _LeftWidgetPushButton;
    QWidget * _LeftWidget;
    QLabel * _LeftWidgetLabelFFTInfo[3];
    QFileDialog * _LeftWidgetQFileDialog;

private slots:
    void ExportFFT();
    void CheckedChanged();
};











#endif
