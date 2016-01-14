#ifndef AUDA_WINDOW
#define AUDA_WINDOW

#include <QDialog>
#include <QMainWindow>
#include <QVBoxLayout>
#include <vector>
#include <QScrollBar>
#include <QComboBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include "./subplot_wav.h"
#include "./subplot_spectro.h"
#include "./subplot_fft.h"
#include "./subplot_scattering.h"
#include "../cigal/spectrogram.h"


class AudaWindow : public QDialog{
    Q_OBJECT

public:
    arma::vec tmp;
    AudaWindow(QMainWindow *parent=0);
    ~AudaWindow()noexcept{}/*
        delete _ScrollBar;
        delete _FileMenu;
        delete _TransformMenu;
        delete _OperationMenu;
        delete _Menubar;
        for(int i=0;i<_MenuActions.size();++i)
            delete _MenuActions[i];
        delete _MainLayout;
        delete _SubplotWav;
        delete _SubplotHelp;
        delete _SubplotFFT;
        delete _SubplotScattering;
        delete _SubplotSpectro;
    }*/
    QScrollBar  * _ScrollBar;
    spectrogram _Spectro;


private :
    QMenu       * _FileMenu;
    QMenu       * _TransformMenu;
    QMenu       * _OperationMenu;
    QMenuBar    *_Menubar;
    std::vector<QAction * > _MenuActions;

    QVBoxLayout * _MainLayout;
    QLabel      * _SubplotHelp;
    SubplotWav  * _SubplotWav;
    SubplotFFT  * _SubplotFFT;
    SubplotScattering * _SubplotScattering=0;
    SubplotSpectro * _SubplotSpectro;

    ScatteringLayer _ScatteringLayer;

    float SignalScatteringRatio;
    float SignalSpectrogramRatio;



private slots:
    void LoadWav();
    void AddFFT();
    void AddSpectro();
    void AddScattering();

    void ZoomSubplotWav();
    void ZoomSubplotSpectro();
    void ZoomSubplotScattering();
    void RefreshSpectro();
    void RefreshScattering();

    void ScrollBarMoved();
};


#endif
