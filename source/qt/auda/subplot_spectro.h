#ifndef SUBPLOT_SPECTRO
#define SUBPLOT_SPECTRO


#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <vector>
#include <armadillo>
#include <QLineEdit>
#include <QComboBox>
#include "../utils/PlottingSpectrogram.h"
#include "../utils/ImagescWidget.h"

class SubplotSpectro:public QDialog{
    Q_OBJECT
public:
    ImagescWidget * _RightWidget;

    SubplotSpectro(QDialog *);
    ~SubplotSpectro()noexcept{}/*delete _MainLayout;
                     delete _LineEditOverlapp;
                     delete _LineEditWindow;
                     delete _CheckLabel1;
                     delete _CheckLabel2;
                     delete _CheckLabel3;
                     delete _ComboBox;
                     delete _LeftWidget;
                     delete _LeftWidgetLayout;
                     delete _LeftWidgetPushButton;
                     delete _LeftWidgetQFileDialog;}*/
    QHBoxLayout * _MainLayout;
    QLineEdit *   _LineEditOverlapp;
    QLabel  *     _CheckLabel3;
    QLineEdit *   _LineEditWindow;
    QLabel  *     _CheckLabel2;
    QComboBox *   _ComboBox;
    QLabel  *     _CheckLabel1;
    QFont         _Font;


    QGridLayout * _LeftWidgetLayout;
    QPushButton * _LeftWidgetPushButton;
    QPushButton * _RefreshButton;
    QWidget * _LeftWidget;
    QFileDialog * _LeftWidgetQFileDialog;

private slots:
    void ExportFFT();
};











#endif
