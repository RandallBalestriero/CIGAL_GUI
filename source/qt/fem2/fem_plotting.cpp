#include "./fem_plotting.h"
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygon>
#include <string>
#include <cmath>
#include <iostream>
#include "../utils.h"

FEMTwoDMeshPainter::FEMTwoDMeshPainter(QWidget *parent):QWidget(parent){}


void FEMTwoDMeshPainter::paintEvent(QPaintEvent*){
    p.begin(this);
    p.fillRect(QRectF(0,0,this->width(),this->height()),Qt::white);
    if(mesh){
        float xmin=mesh->_Points(0,0),xmax=mesh->_Points(mesh->_NumberOfPoints-1,0),ymin=mesh->_Points(0,1),ymax=mesh->_Points(mesh->_NumberOfPoints-1,1);
        float ratiox=(this->width()-40)/(xmax-xmin),ratioy=(this->height()-40)/(ymax-ymin);
        int const Y=this->height()-20;
        arma::vec a(3),b(3),c(3);
        for(int i=0;i<mesh->_NumberOfTriangles;++i){
            a[0]=20+ratiox*(mesh->_Points(mesh->_Triangles(i,0),0)-xmin);
            a[1]=Y-ratioy*(mesh->_Points(mesh->_Triangles(i,0),1)-ymin);
            a[2]=(mesh->_Points(mesh->_Triangles(i,0),2)-mesh->Zmin)/(mesh->Zmax-mesh->Zmin);
            b[0]=20+ratiox*(mesh->_Points(mesh->_Triangles(i,1),0)-xmin);
            b[1]=Y-ratioy*(mesh->_Points(mesh->_Triangles(i,1),1)-ymin);
            b[2]=(mesh->_Points(mesh->_Triangles(i,1),2)-mesh->Zmin)/(mesh->Zmax-mesh->Zmin);
            c[0]=20+ratiox*(mesh->_Points(mesh->_Triangles(i,2),0)-xmin);
            c[1]=Y-ratioy*(mesh->_Points(mesh->_Triangles(i,2),1)-ymin);
            c[2]=(mesh->_Points(mesh->_Triangles(i,2),2)-mesh->Zmin)/(mesh->Zmax-mesh->Zmin);
            PlotTriangle(a,b,c,p);
        }
    }
    p.end();
}

void FEMTwoDMeshPainter::mousePressEvent(QMouseEvent * event){
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

void FEMTwoDMeshPainter::mouseMoveEvent(QMouseEvent * event){
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

void FEMTwoDMeshPainter::mouseReleaseEvent(QMouseEvent *event){
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

void FEMTwoDMeshPainter::wheelEvent(QWheelEvent * event){
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
