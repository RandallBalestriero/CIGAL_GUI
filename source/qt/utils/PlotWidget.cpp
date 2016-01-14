#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygon>
#include <string>
#include <cmath>
#include "./PlotWidget.h"

#include "../utils.h"

PlotWidget::PlotWidget(QWidget *parent,arma::vec *v):QWidget(parent),_Values(v){}




void PlotWidget::paintEvent(QPaintEvent*){
    if(_Values){//if we have data to plot
        p.begin(this);
        //setup margins
        int margin_left=0.05*this->width(),margin_right=margin_left/2;
        int margin_bottom=0.1*this->height(),margin_top=margin_bottom/2;
        //setup available space in pixels
        int Xsize=this->width()-margin_left-margin_right;
        int Ysize=this->height()-margin_bottom-margin_top;
        const int NumberX=Xsize/100,NumberY=Ysize/75;
        //setup min and max values for normalization
        const double Ymin=this->_Values->min(),Ymax=this->_Values->max(),Yrange=std::abs(Ymax-Ymin);
        //set up step in x dimension
        double dx=(double)Xsize/(_Xlim[1]-_Xlim[0]);
        p.setPen(QPen(Qt::black,1));
        for(int i=0;i<this->_Xlim[1]-this->_Xlim[0]-1;++i)
            p.drawLine(QPointF(margin_left+i*dx,margin_top+(1-(_Values->at(this->_Xlim[0]+i)-Ymin)/Yrange)*Ysize),
                    QPointF(margin_left+(i+1)*dx,margin_top+(1-(_Values->at(this->_Xlim[0]+i+1)-Ymin)/Yrange)*Ysize));
        //setup for drawing axis and legend and labels
        QFont Font;
        Font.setPixelSize(std::min(std::min(this->width()*0.02,this->height()*0.04),this->linewidth));
        p.setFont(Font);
        p.drawText(QRectF(margin_left,this->height()-margin_bottom/2,this->width()-margin_left-margin_right,margin_bottom),Qt::AlignHCenter,QString::fromStdString( _XaxisLabel));
        p.setPen(QPen(Qt::black,1,Qt::DotLine));
        //X axis
        for(int i=0;i<NumberX+1;++i){
            p.drawText(QRectF(i*(Xsize/(float)NumberX)+margin_left-Xsize/(2*(NumberX+4.0)),this->height()-margin_bottom,Xsize/(NumberX+4.0),margin_bottom/2),Qt::AlignCenter,QString::number(_XaxisScaling*(_Xlim[0]+i*(_Xlim[1]-_Xlim[0])/(float)NumberX)));
            p.drawLine(QPoint(i*(Xsize/(float)NumberX)+margin_left,this->height()-margin_bottom),QPoint(i*(Xsize/(float)NumberX)+margin_left,margin_top));
        }
        //Y axis
        for(int i=0;i<NumberY+1;++i){
            p.drawText(QRectF(1,Ysize-i*(Ysize/(float)(NumberY))-Ysize/(2*(NumberY+3))+margin_top,margin_left-1,Ysize/(NumberY+3)),Qt::AlignCenter,QString::number(std::round(10*(Ymin+i*(Yrange)/NumberY))/10.0));
            p.drawLine(QPoint(margin_left,Ysize-i*(Ysize/(float)(NumberY))+margin_top),QPoint(this->width()-margin_right,Ysize-i*(Ysize/(float)(NumberY))+margin_top));
        }

        p.end();
    }
}

void PlotWidget::mousePressEvent(QMouseEvent * event){
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

void PlotWidget::mouseMoveEvent(QMouseEvent * event){
    if(event->pos().x()<this->rubberband->x()){
        this->rubberband->setGeometry(QRect(QPoint(event->pos().x(),this->y()),QPoint(this->origin.x(),this->height()-30)));
    }
    else this->rubberband->setGeometry(QRect(origin,QPoint(event->pos().x(),this->y()+this->height()-30)));
}


void PlotWidget::wheelEvent(QWheelEvent * event){
    if(event->delta()>0){
        int tmp=0.05*(_Xlim[1]-_Xlim[0]);
        if(_Xlim[0]+tmp<_Xlim[1]-tmp){
            _Xlim[0]+=tmp;
            _Xlim[1]-=tmp;emit XChanged();}
    }
    else{
        int tmp=std::max(0.05*(_Xlim[1]-_Xlim[0]),1.0);
        _Xlim[0]=std::max(0,(int)_Xlim[0]-tmp);
        _Xlim[1]=std::min((int)this->_Values->n_elem,_Xlim[1]+tmp);
        emit XChanged();
    }
    this->update();
}
