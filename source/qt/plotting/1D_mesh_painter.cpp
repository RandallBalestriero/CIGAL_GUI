#include "./1D_mesh_painter.h"
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygon>
#include <string>
#include <cmath>
#include <iostream>
//#include "../automaton/base.h"
#include "../utils.h"

OneDMeshPainter::OneDMeshPainter(QWidget *parent):QWidget(parent){}

void OneDMeshPainter::AddMesh(float const& a,float const& b, unsigned int const& n){
    _MeshVector.push_back(Mesh1D (a,b,n));
    Xmin=a;
    Xmax=b;
}



void OneDMeshPainter::paintEvent(QPaintEvent*){
    p.begin(this);
    p.fillRect(QRectF(0,0,this->width(),this->height()),Qt::white);
    p.end();

    if(_MeshVector.size()>0){
        arma::vec Range(4);
        Range[0]=Xmin;Range[1]=Ymin;Range[2]=Xmax;Range[3]=Ymax;
        PlotAxisWidget(*this,Range);
        for(unsigned int I=0;I<_MeshVector.size();++I)
            PlotLineWidget(*this,_MeshVector[I]._X,_MeshVector[I]._Y,Range,_MeshColor[I]);
    }
}

void OneDMeshPainter::mousePressEvent(QMouseEvent * event){
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

void OneDMeshPainter::mouseMoveEvent(QMouseEvent * event){
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
            tmp=(4./this->height())*(Ymax-Ymin);
            Ymin+=tmp;
            Ymax+=tmp;
        }
        else if(mouseposition.y()>pos.y()){
            tmp=-(4./this->height())*(Ymax-Ymin);
            Ymin+=tmp;
            Ymax+=tmp;
        }
        mouseposition=pos;
        this->update();
    }
}

void OneDMeshPainter::mouseReleaseEvent(QMouseEvent *event){
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

void OneDMeshPainter::wheelEvent(QWheelEvent * event){
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
