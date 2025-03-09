#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setting up everything
    logConsole = new LogConsole(ui->logConsoleOutput, this);

    buildingSetup = new BuildingSetup(
        ui->passNumInput,
        ui->floorNumInput,
        ui->elevatorNumInput,
        logConsole,
        this
    );

    safetyEventSetup = new SafetyEventSetup(
            ui->helpTimeStep,
            ui->doorObstacleTimeStep,
            ui->fireTimeStep,
            ui->overloadTimeStep,
            ui->powerOutTimeStep,
            ui->helpBtn,
            ui->doorObstacleBtn,
            ui->fireBtn,
            ui->overloadBtn,
            ui->powerOutBtn,
            logConsole,
            this
     );

    passengerBehaviourSetup = new PassengerBehaviourSetup(
           ui->passIdInput,
           logConsole,
           ui->closeDoorBtn,
           ui->closeDoorFloor,
           ui->closeDoorTimeStep,
           ui->exitCarBtn,
           ui->exitCarFloor,
           ui->exitCarTimeStep,
           ui->openDoorBtn,
           ui->openDoorFloor,
           ui->openDoorTimeStep,
           ui->pushHelpBtn,
           ui->pushHelpFloor,
           ui->pushHelpTimeStep,
           ui->requestCarBtn,
           ui->requestCarFloor,
           ui->requestCarTimeStep,
           this
       );

    simulationControls = new SimulationControls(
                ui->startBtn,
                ui->stopBtn,
                ui->pauseBtn,
                ui->simTimeOutput,
                logConsole,
                buildingSetup,
                safetyEventSetup,
                passengerBehaviourSetup,
                this
    );

    ui->logConsoleOutput->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
