#include "ImagescWidget.h"
#include "../../geometry/geometry.h"

ImagescWidget::ImagescWidget(QWidget * parent):QWidget(parent){
    _Values.clear();
    _Xlabels.clear();
    _Ylabels.clear();
}


void ImagescWidget::paintEvent(QPaintEvent*){
    if(_Values.n_rows){//if we have data to plot
        const int margin_left=0.05*this->width(),margin_right=margin_left/2;
        const int margin_bottom=0.1*this->height(),margin_top=margin_bottom/2,TMP2=margin_left;
        const int Xsize=this->width()-margin_left-margin_right;
        const int Ysize=this->height()-margin_bottom-margin_top;
        const int NumberX=Xsize/100,NumberY=Ysize/75;

        const double Min=_Values.min(),Max=_Values.max(),Yrange=std::abs(Max-Min);

        double dx=(double)Xsize/(_Xlimit[1]-_Xlimit[0]),dy=(double)Ysize/(_Ylimit[1]-_Ylimit[0]);
        p.begin(this);
        p.setPen(QPen(Qt::black,1));
        for(int i=_Ylimit[0];i<_Ylimit[1];++i)
            for(int j=0;j<_Xlimit[1]-_Xlimit[0];++j)
                p.fillRect(QRectF(QPointF(margin_left+j*dx,margin_top+i*dy),
                        QPointF(margin_left+(j+1)*dx,margin_top+(i+1)*dy)),QColor(R((_Values.at(i,j+_Xlimit[0])-Min)/Yrange),G((_Values.at(i,j+_Xlimit[0])-Min)/Yrange),B((_Values.at(i,j+_Xlimit[0])-Min)/Yrange)));
        QFont Font;
        Font.setPixelSize(std::min(std::min(this->width()*0.02,this->height()*0.04),14.0));
        p.setFont(Font);
        p.drawText(QRectF(margin_left,this->height()-margin_bottom/2,this->width()-margin_left-margin_right,margin_bottom),Qt::AlignHCenter,QString::fromStdString( _XaxisLabel));
        for(int i=0;i<NumberX+1;++i)
            p.drawText(QRectF(i*(Xsize/(float)NumberX)+margin_left-Xsize/(2*(NumberX+4.0)),this->height()-margin_bottom,Xsize/(NumberX+4.0),margin_bottom/2),Qt::AlignCenter,QString::number(_XaxisScaling*(_Xlimit[0]+i*(_Xlimit[1]-_Xlimit[0])/(float)NumberX)));
        for(int i=0;i<NumberY+1;++i)
            p.drawText(QRectF(margin_left/2,Ysize-i*(Ysize/(float)(NumberY))-Ysize/(2*(NumberY+3))+margin_top,margin_left/2,Ysize/(NumberY+3)),Qt::AlignCenter,QString::number(_Ylimit[0]+i*(_Ylimit[1]-_Ylimit[0])/((float)NumberY)));

        p.rotate(-90);
        p.drawText(QRectF(1,1,margin_left/2,this->height()-margin_bottom),Qt::AlignCenter,QString::fromStdString( _YaxisLabel));
        p.rotate(-90);
        p.end();
    }
}


void ImagescWidget::wheelEvent(QWheelEvent * event){
    int tmp=std::max(0.05*(_Xlimit[1]-_Xlimit[0]),1.0);
    std::cout<<"WDwdwadawdwadad"<<std::endl;
    if(event->delta()>0){
        if(_Xlimit[0]+tmp<_Xlimit[1]-tmp){
            _Xlimit[0]+=tmp;
            _Xlimit[1]-=tmp;emit this->XChanged();this->update();}
    }
    else{
        _Xlimit[0]=std::max(0,(int)_Xlimit[0]-tmp);
        _Xlimit[1]=_Xlimit[1]+tmp;
        emit this->XChanged();
        this->update();
    }
}
