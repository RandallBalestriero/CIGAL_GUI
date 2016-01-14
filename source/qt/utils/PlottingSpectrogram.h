#ifndef PlottingSpectro_HEADER_FILE
#define PlottingSpectro_HEADER_FILE

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QWheelEvent>

#include <armadillo>
#include <vector>


class PlottingSpectrogram : public QWidget{
    Q_OBJECT

private :
    QPainter p;

    void paintEvent(QPaintEvent*);
    void wheelEvent(QWheelEvent*);

public:
    int Xmin,Xmax;
    int SignalXmin,SignalXmax;//necessary to only draw part of the spectrogram column (first and end)
    int NyqFs,_Window; //necessary for axis labeling
    float _Noverlapp;
    arma::mat _Matrix=arma::mat(1,1);
    PlottingSpectrogram(QWidget *parent=0);
    ~PlottingSpectrogram(){}

signals:
    void XChanged();

};


#endif
