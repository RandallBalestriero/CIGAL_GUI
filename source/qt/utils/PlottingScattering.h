#ifndef PlottingScattering_HEADER_FILE
#define PlottingScattering_HEADER_FILE

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QWheelEvent>

#include <armadillo>
#include "../../CIGAL.h"
#include <vector>


class PlottingScattering : public QWidget{
    Q_OBJECT

private :
    QPainter p;

    void paintEvent(QPaintEvent*);
    void wheelEvent(QWheelEvent*);

public:
    ScatteringLayer _Scattering;
    void Perform(arma::vec const& ,int const& ,int const& ,int const& J=0);
    int Xmin,Xmax;
    int SignalXmin,SignalXmax;//necessary to only draw part of the spectrogram column (first and end)
    int NyqFs,_Window; //necessary for axis labeling
    float _Noverlapp;
    PlottingScattering(QWidget *parent=0);//:QWidget(parent){}
    ~PlottingScattering(){}//delete[] _Scattering;}

signals:
    void XChanged();

};


#endif
