#ifndef QT_CLASS_HEADER_FILE
#define QT_CLASS_HEADER_FILE
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include "./plotting/1D_window.h"
#include <QDialog>

#include <vector>

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QMainWindow *parent=0);
    ~MainWindow();
private :
    QDialog * _Pop;
    QWidget * _CentralWidget;
    std::vector<QPushButton *> _ButtonVector;
    QVBoxLayout * _MainLayout;
    std::vector<QHBoxLayout *> _QHBoxLayoutVector;

    void CreateLayout();
    void CreateConnect();
private slots:
    void PopUp1D();
    void PopUp2D();
    void PopUpFEM2D();
    void PopUpHelp();
    void PopUpAUDA();


};


#endif
