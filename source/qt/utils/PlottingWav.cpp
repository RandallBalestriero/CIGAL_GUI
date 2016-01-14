#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygon>
#include <string>
#include <cmath>
#include "./PlottingWav.h"

#include "../utils.h"

PlottingWav::PlottingWav(QWidget *parent,arma::vec *v):QWidget(parent),_Values(v){}




void PlottingWav::paintEvent(QPaintEvent*){
  //  p.begin(this);
  //  p.fillRect(QRectF(0,0,this->width(),this->height()),Qt::);
  //  p.end();

    if(_Values){
        p.begin(this);
        p.setPen(QPen(Qt::black,1));
        int Ysize=this->height()-20;
        int Xsize=this->width()-60;
        const float dx=((float)Xsize/(Xmax-Xmin));
        const float Min=arma::min(*_Values);
        const float Max=arma::max(*_Values);
        const float dy=Ysize/(Max-Min);
        const float TMP1=std::abs(Min);
        for(int i=0;i<this->Xmax-this->Xmin-1;++i)
            p.drawLine(QPointF(40+i*dx,Ysize-(_Values->at(this->Xmin+i)+TMP1)*dy),
                    QPointF(40+(i+1)*dx,Ysize-(_Values->at(this->Xmin+i+1)+TMP1)*dy));
        QFont Font;
        Font.setPixelSize(9);
        p.setFont(Font);
        Xsize-=10;
        for(int i=0;i<11;++i)
            p.drawText(QRectF(30+i*Xsize/10.0,Ysize,this->width()/11.0,30),QString::number((Xmin+i*(Xmax-Xmin)/10.0)/this->Fs));
        p.end();
    }
}

void PlottingWav::mousePressEvent(QMouseEvent * event){
    if(event->button()==Qt::LeftButton){
        if(rubberband) delete rubberband;
    origin = event->pos();
    origin.setY(this->y());
    rubberband = new QRubberBand(QRubberBand::Rectangle, this);
    rubberband->setGeometry(QRect(origin,QPoint(origin.x(),this->y()+this->height()-30)));
    rubberband->show();
    }
    else if(event->button()==Qt::RightButton){
        if(rubberband) delete rubberband;rubberband=0;}
}

void PlottingWav::mouseMoveEvent(QMouseEvent * event){
    rubberband->setGeometry(QRect(origin,QPoint(event->pos().x(),this->y()+this->height()-30)));
}


void PlottingWav::wheelEvent(QWheelEvent * event){
    if(event->delta()>0){
        int tmp=0.05*(Xmax-Xmin);
        if(Xmin+tmp<Xmax-tmp){
            Xmin+=tmp;
            Xmax-=tmp;emit XChanged();}
    }
    else{
        int tmp=std::max(0.05*(Xmax-Xmin),1.0);
        Xmin=std::max(0,(int)Xmin-tmp);
        Xmax=std::min((int)this->_Values->n_elem,Xmax+tmp);
        emit XChanged();
    }
    this->update();
}
