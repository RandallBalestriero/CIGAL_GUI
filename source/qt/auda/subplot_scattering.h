#ifndef SUBPLOT_SCATTERING_Q
#define SUBPLOT_SCATTERING_Q


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
#include "../utils/PlottingScattering.h"
#include "../utils/ImagescWidget.h"

class SubplotScattering:public QDialog{
    Q_OBJECT
public:
    ImagescWidget * _RightWidget;

    SubplotScattering(QDialog*);
    ~SubplotScattering()noexcept{}
    QHBoxLayout * _MainLayout;
    QPushButton * _PushButtons[2];
    QComboBox   * _ComboBox;
    QLineEdit   * _LineEdits[3];
    QLabel  *_Labels[4];

    QGridLayout * _LeftLayout;
    QWidget * _LeftWidget;

};





#endif
