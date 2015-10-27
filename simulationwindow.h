#ifndef SIMULATIONWINDOW_H
#define SIMULATIONWINDOW_H

#include <QWidget>

class SimulationWindow : public QWidget {
    Q_OBJECT

public:
    SimulationWindow(QWidget *parent = 0);
    ~SimulationWindow();
};

#endif // SIMULATIONWINDOW_H
