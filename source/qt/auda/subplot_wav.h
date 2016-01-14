#ifndef SUBPLOT_WAV
#define SUBPLOT_WAV


#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QScrollBar>

#include "../utils/PlotWidget.h"
#include "../../cigal/wav.h"

class SubplotWav:public QWidget{
    Q_OBJECT
public:
    PlotWidget * _RightWidget;
    WAV _Wav;
    SubplotWav(QWidget *);
    QLabel * _LeftWidgetLabelWavInfo[3];

    ~SubplotWav()noexcept{}/*
        delete _MainLayout;
        delete _LeftWidget;
        delete _LeftWidgetLayout;
        delete _LeftWidgetPushButton;
        delete _LeftWidgetQFileDialog;
        delete[] _LeftWidgetLabelWavInfo;
    }*/
private:
    QHBoxLayout * _MainLayout;

    QGridLayout * _LeftWidgetLayout;
    QPushButton * _LeftWidgetPushButton;
    QWidget * _LeftWidget;
    QFileDialog * _LeftWidgetQFileDialog;

public slots:
    void LoadWav();
};








#endif
