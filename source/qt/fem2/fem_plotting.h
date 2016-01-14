#ifndef FEM_MESH_PAINTER_HEADER_FILE
#define FEM_MESH_PAINTER_HEADER_FILE

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QWheelEvent>

#include "../../mesh/2D_triangular_mesh.h"


class FEMTwoDMeshPainter : public QWidget{
    Q_OBJECT

private :
    QPainter p;
    QPoint origin;
    QRubberBand * rubberband=0;
    virtual void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent*);

public:
    float Xmin,Xmax,Ymin,Ymax;
    QPointF mouseposition;
    Mesh2DTriangular * mesh=0;
    FEMTwoDMeshPainter(QWidget *parent=0);
    ~FEMTwoDMeshPainter(){};

};


#endif
