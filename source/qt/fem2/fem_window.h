#ifndef FEM_WINDOW_HEADER_FILE
#define FEM_WINDOW_HEADER_FILE

#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QLineEdit>
#include <QGroupBox>
#include <QDialog>
#include <QBoxLayout>
#include <QMenu>
#include <QAction>
#include "fem_plotting.h"
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include "../../automaton/1D_function_parser.h"
#include <QFileDialog>

#include "../utils.h"
#include <QProgressBar>
#include "../fem/fem.h"


class FEMTwoDWindow : public QDialog{
    Q_OBJECT
public:
    FEMTwoDWindow(QMainWindow *parent);
    ~FEMTwoDWindow();
    ColorMap *             _MeshColorMap;

private :
    QMenuBar    * _BarMenu            ;//the menu to export mesh solution ....
    QMainWindow * _Parent;
    QMenu       * _FileMenu           ;

    QGroupBox   *        _ButtonBox            ;
    QHBoxLayout *        _ButtonBoxLayout      ;
    QPushButton *        _UtilityButton[2]     ;//menu and quit

    QHBoxLayout  *          _MeshLayout;
    float e;
    QGroupBox  *            _PlotMeshBox;
    FEMTwoDMeshPainter     * _OpenGLWindow       ;//the window to display mesh and result

    QGroupBox   *        _MeshBox            ;
    QGridLayout *        _MeshBoxLayout      ;
    std::vector<QLabel*>      _MeshLabel          ;
    std::vector<QLineEdit*>   _MeshLineEdit       ;
    QPushButton *        _MeshPlotButton     ;
    Mesh2DTriangular *  _Mesh          =0       ;
    QDialog * PopUpMesh            =0         ;
    QHBoxLayout * PopUpMeshLayout =0          ;
    PlotMeshWidget * plottingmesh=0;

    QGroupBox   *               _ProblemBox           ;
    QHBoxLayout *               _ProblemBoxLayout     ;
    QGridLayout *               _ProblemBoxSubLayoutLeft     ;
    QGridLayout *               _ProblemBoxSubLayoutRight     ;
    std::vector<QPushButton *>       _ProblemBoxButton   ;
    std::vector<QLabel *>            _ProblemBoxLabel      ;

    std::vector<QLineEdit* >         _ProblemBoxAlphaGridLineEdit;
    QGridLayout * alphagrid;
    QGridLayout * ggrid;
    QDialog * PopAlpha;
    QDialog * PopG;
    std::vector<QLineEdit* >         _ProblemBoxGGridLineEdit;


    QGroupBox   *               _SolveBox;
    QGridLayout *               _SolveBoxLayout ;
    QComboBox   *               _SolveMenu ;
    QLineEdit   *               _SolveCriteria;
    QPushButton *               _SolveButton;
    QCheckBox   *               _SolvePrecon;
    std::vector<QLabel*>             _SolveLabel;

    QVBoxLayout * _MainLayout         ;

    alpha *                 _Alpha=0;
    g       *               _G=0;

    arma::sp_mat     _A;
    arma::vec        _B;

    void CreateMeshBox();
    void CreateButtonBox();
    void CreateProblemBox();
    void CreateMenu();
    void CreateSolveBox();//for the utility buttons
    void CreateGLWidget();//the OpenGL window

private slots:
    void FromTxt();
    void Solve();
    void PlotMesh();
    void CreateAlpha();
    void CreateG();
    void PopUpMenu();//when clicking on menu !

};


#endif
