#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T10:26:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = neural-net
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        simulationwindow.cpp \
    algo/neuron.cpp \
    algo/neuroninput.cpp \
    algo/test/neurontestinput.cpp \
    algo/test/suits/neurontest.cpp \
    algo/test/suits/testsuit.cpp \
    third_party/json.cpp \
    algo/test/suits/imagenettest.cpp \
    algo/test/suits/doubleneurontest.cpp

HEADERS  += simulationwindow.h \
    algo/neuron.h \
    algo/neuroninput.h \
    algo/test/neurontestinput.h \
    algo/test/suits/neurontest.h \
    algo/test/suits/testsuit.h \
    third_party/json.h \
    algo/test/suits/imagenettest.h \
    algo/test/suits/doubleneurontest.h

# Cycle Frequency
DEFINES += "CLK_FREQ=2500"
