#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LogConsole.h"
#include "SimulationControls.h"
#include "BuildingSetup.h"
#include "PassengerBehaviourSetup.h"
#include "SafetyEventSetup.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LogConsole *logConsole;
    BuildingSetup *buildingSetup;
    SimulationControls *simulationControls;
    PassengerBehaviourSetup *passengerBehaviourSetup;
    SafetyEventSetup *safetyEventSetup;
};
#endif // MAINWINDOW_H
