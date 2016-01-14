#ifndef Imagesc
#define Imagesc

#include <QWidget>
#include <vector>
#include <armadillo>
#include <string>
#include <QWheelEvent>
#include <QPainter>


class ImagescWidget:public QWidget{
    Q_OBJECT
private:
    QPainter p;
    void paintEvent(QPaintEvent*);
    void wheelEvent(QWheelEvent*);
public:
    arma::mat _Values;
    int _Xlimit[2],_Ylimit[2];
    double _XaxisScaling=1;
    std::vector<double> _Xlabels,_Ylabels;
    std::string _XaxisLabel="x",_YaxisLabel="y";
    ~ImagescWidget(){}

    ImagescWidget(QWidget *);

signals:
    void XChanged();
};




#endif
