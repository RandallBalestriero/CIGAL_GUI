#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPolygon>
#include <string>
#include <cmath>
#include "./PlottingSpectrogram.h"
#include "../geometry/geometry.h"
#include "../utils.h"

PlottingSpectrogram::PlottingSpectrogram(QWidget *parent):QWidget(parent){}




void PlottingSpectrogram::paintEvent(QPaintEvent*){
    if(_Matrix.n_cols>1){
        const int MarginLeft=40,MarginRight=20;
        p.begin(this);
        p.setPen(QPen(Qt::blue,1));

        int Ysize=this->height()-MarginRight;
        int Xsize=this->width()-MarginLeft-MarginRight;
        const float dy=((float)Ysize)/(this->_Matrix.n_rows+1);

        Xmin=std::max(0.,std::floor(SignalXmin/((1.-_Noverlapp)*_Window)));
        Xmax=std::ceil(SignalXmax/((1.-_Noverlapp)*_Window))-1;

        const float DX=(float)Xsize/(Xmax-Xmin);
        float TMP1,TMP2;

        for(int j=0;j<Xmax-Xmin;++j){//access row wise becasue armadillo
            TMP1=Xmin+j;TMP2=MarginLeft+j*DX;
            for(unsigned int i=0;i<_Matrix.n_rows;++i)
                p.fillRect(QRectF(QPointF(TMP2,i*dy),
                        QPointF(TMP2+DX,(i+1)*dy)),QColor(R(_Matrix.at(i,TMP1)),G(_Matrix.at(i,TMP1)),B(_Matrix.at(i,TMP1))));
            }


        QFont Font;
        Font.setPixelSize(9);
        p.setFont(Font);
        Xsize-=10;
        Ysize-=10;
        p.setPen(QPen(Qt::DotLine));
        for(int i=0;i<11;++i){
            p.drawText(QRectF(30+i*Xsize/10.0,this->height()-20,Xsize/11.0,30),QString::number((Xmin+(i*(Xmax-Xmin)/10.0))*(_Noverlapp*_Window)/(2*NyqFs)));
        }
        Font.setPixelSize(9);

        for(int i=0;i<5;++i){
            std::cout<<std::round(10*(i*NyqFs/4.0))/10.0<<std::endl;
            p.drawText(QRectF(0,Ysize-i*Ysize/4.0,39,40),Qt::AlignRight,QString::number(std::round(std::round(10*(i*NyqFs/4.0))/10.0)));
        }
        p.drawText(QRectF(this->width()/2-20,this->height()-10,80,10),"Time in sec.");
        p.drawText(QRectF(5,this->height()/2-10,20,20),"Hz");
        p.end();
    }
}



void PlottingSpectrogram::wheelEvent(QWheelEvent * event){
    int tmp=std::max(0.05*(SignalXmax-SignalXmin),1.0);
    if(event->delta()>0){
        if(SignalXmin+tmp<SignalXmax-tmp){
            SignalXmin+=tmp;
            SignalXmax-=tmp;emit XChanged();this->update();}
    }
    else{
        SignalXmin=std::max(0,(int)SignalXmin-tmp);
        SignalXmax=SignalXmax+tmp;
        emit XChanged();
        this->update();

    }
}
