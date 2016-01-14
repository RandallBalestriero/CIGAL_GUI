#ifndef OD_MESH_PAINTER_HEADER_FILE
#define OD_MESH_PAINTER_HEADER_FILE

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QWheelEvent>

#include "../../mesh/1D_mesh.h"


class OneDMeshPainter : public QWidget{
    Q_OBJECT

private :
    QPainter p;
    QPoint origin;
    QRubberBand * rubberband=0;

    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent*);

public:
    float Xmin,Xmax,Ymin,Ymax;
    QPointF mouseposition;
    std::vector<Mesh1D > _MeshVector;
    std::vector<std::string> _MeshColor;
    OneDMeshPainter(QWidget *parent=0);
    ~OneDMeshPainter(){};
    void AddMesh(float const&,float const&,unsigned int const&);

};


#endif
