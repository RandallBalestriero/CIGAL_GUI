#ifndef TD_MESH_PAINTER_HEADER_FILE
#define TD_MESH_PAINTER_HEADER_FILE

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QWheelEvent>

#include "../../mesh/2D_rectangular_mesh.h"


class TwoDMeshPainter : public QWidget{
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

    inline float XtoPixel(float const&);
    inline float YtoPixel(float const&);

public:
    float Xmin,Xmax,Ymin,Ymax;
    QPointF mouseposition;
    Mesh2DRectangular _Mesh;
    TwoDMeshPainter(QWidget *parent=0);
    ~TwoDMeshPainter(){};
    void AddMesh(float const&,float const&,float const&,float const&,unsigned int const&,unsigned int const&);

};


#endif
