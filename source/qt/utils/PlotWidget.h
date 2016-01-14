#ifndef PlottingWidget
#define PlottingWidget

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QWheelEvent>
#include <QScrollBar>
#include <string>
#include <armadillo>


class PlotWidget : public QWidget{
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

    double linewidth=16;
    QPoint origin;
    QRubberBand * rubberband=0;
    int _Xlim[2];
    std::string _XaxisLabel="x";double _XaxisScaling=1;
    std::string _YaxisLabel="y";

    QPointF mouseposition;
    arma::vec * _Values;
    PlotWidget(QWidget *parent=0,arma::vec * p=0);
    ~PlotWidget(){if(rubberband) delete rubberband;}

};


#endif
