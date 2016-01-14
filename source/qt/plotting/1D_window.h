#ifndef OD_WINDOW_HEADER_FILE
#define OD_WINDOW_HEADER_FILE

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
#include "1D_mesh_painter.h"
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include "../../automaton/1D_function_parser.h"
#include <QFileDialog>



class OneDWindow : public QDialog{
    Q_OBJECT
public:
    OneDWindow(QMainWindow *parent);
    ~OneDWindow();
private :
    QMenuBar    * _BarMenu            ;//the menu to export mesh solution ....
    QMainWindow * _Parent;

    QComboBox * _FunctionNumberMenu ;

    QMenu       * _FileMenu           ;
    QPushButton * _UtilityButton[2]     ;//menu and quit
    OneDMeshPainter     * _OpenGLWindow       ;//the window to display mesh and result

    QGroupBox   * _MeshBox            ;
    QGridLayout * _MeshBoxLayout      ;

    QGroupBox   * _ButtonBox          ;
    QHBoxLayout * _ButtonBoxLayout    ;

    QGroupBox   *               _RangeBox           ;
    QGridLayout *               _RangeBoxLayout     ;
    std::vector<QLineEdit *>    _RangeBoxLineEdit   ;
    std::vector<QLabel *>       _RangeBoxLabel      ;
    QPushButton *               _RangeBoxPlotButton ;

    QGroupBox   *               _FunctionBox;
    QGridLayout *               _FunctionBoxLayout ;
    QLineEdit   *               _FunctionBoxLineEdit;
    QComboBox   *               _FunctionBoxCombo   ;
    QComboBox   *               _FunctionBoxComboColor;
    std::vector<QPushButton *>  _FunctionBoxImportButton;
    QLabel      *               _FunctionBoxLabel;
    QFileDialog *               _FunctionBoxDialogTxt;
    std::vector<QString>    _FunctionBoxSource;

    QVBoxLayout * _MainLayout         ;

    void CreateFunctionBox();
    void CreateRangeBox();
    void CreateMenu();
    void CreateButtonBox();//for the utility buttons
    void CreateGLWidget();//the OpenGL window

private slots:
    void FunctionIndexChanged();
    void FromTxt();
    void FromWav();
    void ClearPlot();
    void Plot();
    void PopUpMenu();//when clicking on menu !
    void Trig();
};


#endif
