#ifndef QT_UTILS_HEADER_FILE
#define QT_UTILS_HEADER_FILE

#include <QWidget>
#include <QPainterPath>
#include <QPainter>
#include "../fem/fem.h"
#include "../mesh/2D_triangular_mesh.h"
#include "../geometry/geometry.h"

inline void PlotLineWidget(QWidget & w,arma::vec const& X,arma::vec const& Y,arma::vec const& Range,std::string const& color="black",int const& lw=2){
    QPainter p;
    double XRange=Range[2]-Range[0];
    double YRange=Range[3]-Range[1];

    p.begin(&w);
    if(color=="Black")
        p.setPen(QPen(Qt::black, lw));
    if(color=="Blue")
        p.setPen(QPen(Qt::blue, lw));
    if(color=="Red")
        p.setPen(QPen(Qt::red, lw));
    if(color=="Green")
        p.setPen(QPen(Qt::green, lw));
    for(unsigned int i=0;i<X.n_elem-1;++i){
        if(X[i]>=Range[0] && X[i]<=Range[2] && Y[i]>= Range[1] && Y[i]<=Range[3] && X[i+1]<=Range[2] && X[i+1]>=Range[0] && Y[i+1]<= Range[3] && Y[i+1]>= Range[1]){
            p.drawLine(w.width()*abs(X[i]-Range[0])/(XRange),
                    w.height()*abs(Y[i]-Range[3])/YRange,
                    w.width()*abs(X[i+1]-Range[0])/XRange,
                    w.height()*abs(Y[i+1]-Range[3])/YRange);
        }
    }
    p.end();
}


class ColorMap:public QWidget{
    Q_OBJECT
public:
    double _Min,_Max;
    ColorMap(){_Min=0;_Max=1;}
private:
    void paintEvent(QPaintEvent *){
        QPainter p(this);
        for(int x=0;x<std::min(20,this->width()/4);++x)
            for(int y=0;y<this->height();++y){
                p.setPen(QColor(R(1-(float)y/this->height()),G(1-(float)y/this->height()),B(1-(float)y/this->height())));
                p.drawPoint(x,y);
            }
        float d=(_Max-_Min)/9;
        p.setPen(Qt::black);
        for(int y=0;y<10;y++)
            p.drawText(QRectF(std::min(20,this->width()/4)+5,y*((float)this->height()/10),60,30),QString::number(_Max-y*d));
        p.end();
    }
};








inline void PlotAxisWidget(QWidget & w,arma::vec const& Range){
    //plot Y axis
    QPainter p;
    p.begin(&w);
    p.setPen(QPen(Qt::black, 2));
    float Xmin=Range[0],Xmax=Range[2],Ymin=Range[1],Ymax=Range[3];

    float YStep=(w.height())/10;
    QString b;
    if(Xmax>0 && Xmin<0){
        int T=w.width()*(-Xmin/(Xmax-Xmin));
        for(int i=1;i<10;++i){
            p.drawPoint(QPointF(T,i*YStep));
            b.sprintf("%.2f",Ymax-i*(Ymax-Ymin)/10.);
            p.drawText(T-10,i*YStep,35,20,Qt::AlignRight,b);
        }
    }
    else if(Xmin>=0){
        for(int i=1;i<10;++i){
            p.drawPoint(QPointF(40,i*YStep));
            b.sprintf("%.2f",Ymax-i*(Ymax-Ymin)/10.);
            p.drawText(2,i*YStep,35,20,Qt::AlignRight,b);
        }
    }
    else{
        int T=w.width();
        for(int i=1;i<10;++i){
            p.drawPoint(QPointF(T,i*YStep));
            b.sprintf("%.2f",Ymax-i*(Ymax-Ymin)/10.);
            p.drawText(T+10,i*YStep,35,20,Qt::AlignRight,b);
        }
    }
    //Plot X axis
    float XStep=(w.width())/10;
    if(Ymax>0 && Ymin<0){
        int T=w.height()*(Ymax/(Ymax-Ymin));
        for(int i=1;i<10;++i){
            p.drawPoint(QPointF(i*XStep,T));
            b.sprintf("%.2f",Xmin+i*(Xmax-Xmin)/10.);
            p.drawText(i*XStep,T,35,20,Qt::AlignRight,b);
        }
    }
    else if(Ymin>=0){
        int T=w.height();
        for(int i=1;i<10;++i){
            p.drawPoint(QPointF(i*XStep,T));
            b.sprintf("%.2f",Xmin+i*(Xmax-Xmin)/10.);
            p.drawText(i*XStep,T+4,35,20,Qt::AlignRight,b);
        }
    }
    else{
        int T=w.width();
        for(int i=1;i<10;++i){
            p.drawPoint(QPointF(i*XStep,T));
            b.sprintf("%.2f",Ymax-i*(Ymax-Ymin)/10.);
            p.drawText(i*XStep,T-4,35,20,Qt::AlignRight,b);
        }
    }
}







class PlotMeshWidget : public QWidget{
    Q_OBJECT
public:
    PlotMeshWidget(QWidget *);
    void SetMesh(Mesh2DTriangular *);
    Mesh2DTriangular * mesh;
    ~PlotMeshWidget();
private:
    void paintEvent(QPaintEvent*);
};

inline void PlotTriangle(arma::vec const& a,arma::vec const& b,arma::vec const& c,QPainter & p){
    float a1=(c[2]-a[2])*(b[1]-a[1])-(c[1]-a[1])*(b[2]-a[2]);
    float a2=-(c[2]-a[2])*(b[0]-a[0])+(b[2]-a[2])*(c[0]-a[0]);
    float a3=(b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);

    float C=a1*a[0]+a2*a[1]+a3*a[2];
    C/=a3;
    a1/=(-a3);
    a2/=(-a3);
    if(a[0]<b[0]){
        float coeff=(b[0]-a[0])/(b[1]-c[1]);
        for(int i=c[1];i<b[1];++i){
            for(int j=a[0]+coeff*(b[1]-i);j<b[0];++j){
                float PP=a1*(j)+a2*(i)+C;
                p.setPen(QColor(R(PP),G(PP),B(PP)));
                p.drawPoint(j,i);
            }
        }
    }
    else{
        float coeff=(a[0]-b[0])/(c[1]-b[1]);
        for(int i=b[1];i<c[1];++i){
            for(int j=b[0];j<std::min(b[0]+coeff*(c[1]-i),a[0]);++j){
                float PP=a1*(j)+a2*(i)+C;
                p.setPen(QColor(R(PP),G(PP),B(PP)));
                p.drawPoint(j,i);
            }
        }
    }
}


#endif
