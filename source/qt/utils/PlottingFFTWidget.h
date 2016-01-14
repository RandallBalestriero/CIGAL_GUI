#ifndef PlottingFFTWIDGET_HEADER_FILE
#define PlottingFFTWIDGET_HEADER_FILE

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


class PlottingFFTWidget : public QWidget{
    Q_OBJECT

private :
    QPainter p;

    void paintEvent(QPaintEvent*);

public:
    int NyqFs;
    int Checked[3];
    std::vector<arma::vec *> _Values;
    PlottingFFTWidget(QWidget *parent=0);
    ~PlottingFFTWidget(){};

};


#endif
