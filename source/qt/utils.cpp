#include "./utils.h"
#include <QProgressBar>

PlotMeshWidget::PlotMeshWidget(QWidget * parent):QWidget(parent){}

void PlotMeshWidget::SetMesh(Mesh2DTriangular * m){mesh=m;}

PlotMeshWidget::~PlotMeshWidget(){}

void PlotMeshWidget::paintEvent(QPaintEvent*){
    float xmin=mesh->_Points(0,0),xmax=mesh->_Points(mesh->_NumberOfPoints-1,0),ymin=mesh->_Points(0,1),ymax=mesh->_Points(mesh->_NumberOfPoints-1,1);
    float ratiox=(this->width()-40)/(xmax-xmin),ratioy=(this->height()-40)/(ymax-ymin);
    QPainterPath path;
    int const Y=this->height()-20;
    QPainter p;
    p.begin(this);
    p.setPen(QPen(Qt::black,1));
    for(int i=0;i<mesh->_NumberOfTriangles;++i){
        path.moveTo(20+ratiox*(mesh->_Points(mesh->_Triangles(i,0),0)-xmin),Y-ratioy*(mesh->_Points(mesh->_Triangles(i,0),1)-ymin));
        path.lineTo(20+ratiox*(mesh->_Points(mesh->_Triangles(i,1),0)-xmin),Y-ratioy*(mesh->_Points(mesh->_Triangles(i,1),1)-ymin));
        path.lineTo(20+ratiox*(mesh->_Points(mesh->_Triangles(i,2),0)-xmin),Y-ratioy*(mesh->_Points(mesh->_Triangles(i,2),1)-ymin));
        path.lineTo(20+ratiox*(mesh->_Points(mesh->_Triangles(i,0),0)-xmin),Y-ratioy*(mesh->_Points(mesh->_Triangles(i,0),1)-ymin));
    }
    p.drawPath(path);
    p.drawText(QRectF(0,Y,40,20),"("+QString::number(mesh->_Points(0,0))+","+QString::number(mesh->_Points(0,1))+")");
    p.drawText(QRectF(this->width()-40,0,30,20),"("+QString::number(mesh->_Points(mesh->_NumberOfPoints-1,0))+","+QString::number(mesh->_Points(mesh->_NumberOfPoints-1,1))+")");
    p.end();
}


