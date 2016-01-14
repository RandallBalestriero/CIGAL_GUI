######################################################################
# Automatically generated by qmake (3.0) Thu Jan 14 08:26:15 2016
######################################################################

TEMPLATE = app
DEFINES += CIGAL
TARGET = CIGAL_GUI
CONFIG+= c++11
QT+= widgets
QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += . \
               source/qt \
               source/qt/plotting \
               source/mesh \
               source/automaton \
               source/algebra \
               source/cigal \
               source/fem \
               source \
               source/qt/plotting2 \
               source/qt/fem2 \
               source/geometry \
               source/qt/auda \
               source/qt/utils

# Input
HEADERS += source/CIGAL.h \
#           source/CIRIL.h \
           source/algebra/solve.h \
           source/automaton/1D_function_parser.h \
           source/automaton/2D_function_parser.h \
           source/cigal/fft.h \
           source/cigal/filter_struct.h \
           source/cigal/filters.h \
           source/cigal/layer_struct.h \
#           source/cigal/loadtxt.h \
#           source/cigal/meta_param.h \
#           source/cigal/network_struct.h \
           source/cigal/spectrogram.h \
           source/cigal/wav.h \
           source/fem/fem.h \
           source/geometry/geometry.h \
           source/linked_list/linked_list.h \
           source/linked_list/souble_list.h \
           source/mesh/1D_mesh.h \
           source/mesh/2D_rectangular_mesh.h \
           source/mesh/2D_triangular_mesh.h \
           source/qt/main_window.h \
           source/qt/utils.h \
           source/qt/window_info.h \
           source/utils/directories.h \
           source/qt/auda/subplot_fft.h \
           source/qt/auda/subplot_scattering.h \
           source/qt/auda/subplot_spectro.h \
           source/qt/auda/subplot_wav.h \
           source/qt/auda/window.h \
           source/qt/fem2/fem_plotting.h \
           source/qt/fem2/fem_window.h \
           source/qt/plotting/1D_mesh_painter.h \
           source/qt/plotting/1D_window.h \
           source/qt/plotting2/2D_mesh_painter.h \
           source/qt/plotting2/2D_window.h \
           source/qt/utils/ImagescWidget.h \
           source/qt/utils/Plotting2DMatrix.h \
           source/qt/utils/PlottingFFTWidget.h \
#           source/qt/utils/PlottingScattering.h \
#           source/qt/utils/PlottingSpectrogram.h \
           source/qt/utils/PlottingWav.h \
           source/qt/utils/PlotWidget.h
SOURCES += app.cpp \
           scattering.cpp \
           source/automaton/1D_function_parser.cpp \
           source/automaton/2D_function_parser.cpp \
           source/cigal/fft.cpp \
           source/cigal/filters.cpp \
           source/cigal/spectrogram.cpp \
           source/fem/fem.cpp \
           source/mesh/1D_mesh.cpp \
           source/mesh/2D_rectangular_mesh.cpp \
           source/mesh/2D_triangular_mesh.cpp \
           source/qt/main_window.cpp \
           source/qt/utils.cpp \
           source/qt/auda/subplot_fft.cpp \
           source/qt/auda/subplot_scattering.cpp \
           source/qt/auda/subplot_spectro.cpp \
           source/qt/auda/subplot_wav.cpp \
           source/qt/auda/window.cpp \
           source/qt/fem2/fem_plotting.cpp \
           source/qt/fem2/fem_window.cpp \
           source/qt/plotting/1D_mesh_painter.cpp \
           source/qt/plotting/1D_window.cpp \
           source/qt/plotting2/2D_mesh_painter.cpp \
           source/qt/plotting2/2D_window.cpp \
           source/qt/utils/ImagescWidget.cpp \
           source/qt/utils/Plotting2DMatrix.cpp \
           source/qt/utils/PlottingFFTWidget.cpp \
#           source/qt/utils/PlottingScattering.cpp \
#           source/qt/utils/PlottingSpectrogram.cpp \
           source/qt/utils/PlottingWav.cpp \
           source/qt/utils/PlotWidget.cpp