#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygon>
#include <string>
#include <cmath>
#include "./PlottingFFTWidget.h"

#include "../utils.h"

PlottingFFTWidget::PlottingFFTWidget(QWidget *parent):QWidget(parent){Checked[0]=1;Checked[1]=0;Checked[2]=0;}




void PlottingFFTWidget::paintEvent(QPaintEvent*){
    p.begin(this);
    p.fillRect(QRectF(0,0,this->width(),this->height()),Qt::white);
    p.end();

    if(_Values.size()){
        p.begin(this);
        p.setPen(QPen(Qt::blue,1));
        int Ysize=this->height()-20;
        int Xsize=this->width()-100;
        float dx=((float)Xsize)/(this->_Values[0]->n_elem);
        float Min=std::min(0.0,arma::min(*_Values[0]));
        float Max=arma::max(*_Values[0]);
        float dy=(Ysize)/(Max-Min);
        for(unsigned int N=0;N<_Values.size();++N)
            if(Checked[N]){
                for(unsigned int i=0;i<this->_Values[0]->n_elem-1;++i)
                    p.drawLine(QPointF(60+i*dx,Ysize-(_Values[N]->at(i)+std::abs(Min))*dy),
                        QPointF(60+(i+1)*dx,Ysize-(_Values[N]->at(i+1)+std::abs(Min))*dy));}
        QFont Font;
        Font.setPixelSize(9);
        p.setFont(Font);
        Xsize-=10;
        Ysize-=10;
        p.setPen(QPen(Qt::DotLine));
        for(int i=0;i<11;++i){
            p.drawText(QRectF(50+i*Xsize/10.0,this->height()-20,Xsize/11.0,30),QString::number(std::round((1.0/(2*_Values[0]->n_elem)+(i*_Values[0]->n_elem/10)*(2*NyqFs-1.0/_Values[0]->n_elem)/(2*_Values[0]->n_elem))*10/10.0)));
            p.drawLine(QPoint(60+i*Xsize/10.0,Ysize+10),QPoint(60+i*Xsize/10.0,5));
        }
        for(int i=0;i<5;++i){
            p.drawText(QRectF(0,Ysize-i*Ysize/4,55,this->width()/6),Qt::AlignRight,QString::number(std::round(10*(Min+i*(Max-Min)/4))/10.0));
            p.drawLine(QPoint(60,Ysize-i*Ysize/4+5),QPoint(Xsize+60,Ysize-i*Ysize/4+5));
        }
        p.setPen(QPen(Qt::black,2));
        p.drawLine(QPoint(60,Ysize+10),QPoint(Xsize+60,Ysize+10));
        p.drawLine(QPoint(60,Ysize+10),QPoint(60,0));
        p.end();
    }
}

