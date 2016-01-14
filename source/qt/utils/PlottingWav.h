#ifndef PlottingWav_HEADER_FILE
#define PlottingWav_HEADER_FILE

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QWheelEvent>
#include <QScrollBar>
#include <armadillo>


class PlottingWav : public QWidget{
    Q_OBJECT

private :
    QPainter p;


    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent*);


signals:
    void XChanged();

public:

    QPoint origin;
    QRubberBand * rubberband=0;
    int Xmin=0,Xmax=0,Fs=0;
    QPointF mouseposition;
    arma::vec * _Values;
    PlottingWav(QWidget *parent=0,arma::vec * p=0);
    ~PlottingWav(){if(rubberband) delete rubberband;}

};


#endif
