#include "./2D_mesh_painter.h"
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygon>
#include <string>
#include <cmath>
#include <iostream>

TwoDMeshPainter::TwoDMeshPainter(QWidget *parent):QWidget(parent){}

void TwoDMeshPainter::AddMesh(float const& a,float const& b,float const & c,float const& d,unsigned int const& n, unsigned int const& m){
    _Mesh=Mesh2DRectangular (a,b,c,d,n,m);
    Xmin=a;
    Ymin=b;
    Xmax=c;
    Ymax=d;
}

float TwoDMeshPainter::XtoPixel(float const& x){
    return this->width()*abs(x-Xmin)/(Xmax-Xmin);
}

float TwoDMeshPainter::YtoPixel(float const& y){
    return this->height()*(1-abs(y-Ymin)/(Ymax-Ymin));
}

void TwoDMeshPainter::paintEvent(QPaintEvent*){
    p.begin(this);
    p.fillRect(QRectF(0,0,this->width(),this->height()),Qt::white);
    p.end();
    if(this->_Mesh._NumberOfRectangles!=0){
        //plot Y axis
        p.begin(this);
        p.setPen(QPen(Qt::black, 2));
        float YStep=(this->height())/10;
        QString b;
        if(Xmax>0 && Xmin<0){
            int T=this->width()*(-Xmin/(Xmax-Xmin));
            for(int i=1;i<10;++i){
                p.drawPoint(QPointF(T,i*YStep));
                b.sprintf("%.2f",Ymax-i*(Ymax-Ymin)/10.);
                p.drawText(T-40,i*YStep,35,20,Qt::AlignRight,b);
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
            int T=this->width()-50;
            for(int i=1;i<10;++i){
                p.drawPoint(QPointF(T,i*YStep));
                b.sprintf("%.2f",Ymax-i*(Ymax-Ymin)/10.);
                p.drawText(T+10,i*YStep,35,20,Qt::AlignRight,b);
            }
        }
        //Plot X axis
        float XStep=(this->width())/10;
        if(Ymax>0 && Ymin<0){
            int T=this->height()*(Ymax/(Ymax-Ymin));
            for(int i=1;i<10;++i){
                p.drawPoint(QPointF(i*XStep,T));
                b.sprintf("%.2f",Xmin+i*(Xmax-Xmin)/10.);
                p.drawText(i*XStep,T,35,20,Qt::AlignRight,b);
            }
        }
        else if(Ymin>=0){
            int T=this->height()-20;
            for(int i=1;i<10;++i){
                p.drawPoint(QPointF(i*XStep,T));
                b.sprintf("%.2f",Xmin+i*(Xmax-Xmin)/10.);
                p.drawText(i*XStep,T+4,35,20,Qt::AlignRight,b);
            }
        }
        else{
            int T=this->width()-50;
            for(int i=1;i<10;++i){
                p.drawPoint(QPointF(i*XStep,T));
                b.sprintf("%.2f",Ymax-i*(Ymax-Ymin)/10.);
                p.drawText(i*XStep,T-10,35,20,Qt::AlignRight,b);
            }
        }
        for(unsigned int i=0;i<_Mesh._NumberOfRectangles;++i){
            if(_Mesh._Points(_Mesh._Rectangles(i,3),0)>Xmin && _Mesh._Points(_Mesh._Rectangles(i,3),1)<Ymax &&_Mesh._Points(_Mesh._Rectangles(i,1),0)<Xmax && _Mesh._Points(_Mesh._Rectangles(i,1),1)>Ymin){
                //arma::vec c(3);c=_Mesh.MeanColor(i);
               // p.fillRect(QRectF(QPointF(XtoPixel(_Mesh._Points(_Mesh._Rectangles(i,3),0)),YtoPixel(_Mesh._Points(_Mesh._Rectangles(i,3),1))),QPointF(XtoPixel(_Mesh._Points(_Mesh._Rectangles(i,1),0)),YtoPixel(_Mesh._Points(_Mesh._Rectangles(i,1),1)))),QColor(c[0],c[1],c[2]));
            }
        }

    p.end();
    }
}

void TwoDMeshPainter::mousePressEvent(QMouseEvent * event){
    if(event->button()==Qt::LeftButton){
    origin = event->pos();
    rubberband = new QRubberBand(QRubberBand::Rectangle, this);
    rubberband->setGeometry(QRect(origin, QSize()));
    rubberband->show();
    }
    else{
        mouseposition=event->pos();
    }
}

void TwoDMeshPainter::mouseMoveEvent(QMouseEvent * event){
    if(rubberband)
        rubberband->setGeometry(QRect(origin, event->pos()).normalized());
    else{
        QPointF pos=event->globalPos();
        float tmp;
        if(mouseposition.x()<pos.x()){
            tmp=-(4./this->width())*(Xmax-Xmin);
            Xmin+=tmp;
            Xmax+=tmp;
        }
        else if(mouseposition.x()>pos.x()){
            tmp=(4./this->width())*(Xmax-Xmin);
            Xmin+=tmp;
            Xmax+=tmp;
        }
        if(mouseposition.y()<pos.y()){
            tmp=-(4./this->height())*(Ymax-Ymin);
            Ymin-=tmp;
            Ymax-=tmp;
        }
        else if(mouseposition.y()>pos.y()){
            tmp=(4./this->height())*(Ymax-Ymin);
            Ymin-=tmp;
            Ymax-=tmp;
        }
        mouseposition=pos;
        this->update();
    }
}

void TwoDMeshPainter::mouseReleaseEvent(QMouseEvent *event){
    QPoint end=event->pos();
    if(rubberband){
        Xmin+=abs(((float)std::min(end.x(),origin.x())/this->width())*(Xmax-Xmin));
        Xmax-=abs((1-(float)std::max(end.x(),origin.x())/this->width())*(Xmax-Xmin));

        Ymin+=abs((1-(float)std::max(end.y(),origin.y())/this->height())*(Ymax-Ymin));
        Ymax-=abs(((float)std::min(end.y(),origin.y())/this->height())*(Ymax-Ymin));
        rubberband->hide();
        delete rubberband;
        rubberband=0;
        this->update();
    }
}

void TwoDMeshPainter::wheelEvent(QWheelEvent * event){
    if(event->delta()>0){
        float tmp=(Xmax-Xmin)*0.1;
        Xmin+=tmp;
        Xmax-=tmp;
        tmp=(Ymax-Ymin)*0.1;
        Ymin+=tmp;
        Ymax-=tmp;
    }
    else{
        float tmp=(Xmax-Xmin)*0.1;
        Xmin-=tmp;
        Xmax+=tmp;
        tmp=(Ymax-Ymin)*0.1;
        Ymin-=tmp;
        Ymax+=tmp;
    }
    this->update();
}
