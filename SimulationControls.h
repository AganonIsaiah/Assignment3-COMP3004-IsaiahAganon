#ifndef SIMULATIONCONTROLS_H
#define SIMULATIONCONTROLS_H

#include "LogConsole.h"
#include "BuildingSetup.h"
#include "SafetyEventSetup.h"
#include "PassengerBehaviourSetup.h"
#include <QTimer>
#include <QApplication>
#include <QThread>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <QDebug>
#include <string>

/**
 * @brief The SimulationControls class is responsible for:
 *        - Running the simulation
 *        - Pausing the simulation
 *        - Stopping the simulation
 *        - Displaying the elevator's movement
 *        - Displaying the safety events and how they're handled
 *        - Displaying passengers' behaviours
 */
class SimulationControls : public QObject
{
    Q_OBJECT

public:
    explicit SimulationControls(QPushButton *startButton,
                                QPushButton *stopButton,
                                QPushButton *pauseButton,
                                QLineEdit *simTimeOutput,
                                LogConsole *logConsole,
                                BuildingSetup *buildingSetup,
                                SafetyEventSetup *safetyEventSetup,
                                PassengerBehaviourSetup *passengerBehaviourSetup,
                                QObject *parent = nullptr);

    void runSimulation();
    void printSafetyEvent(const std::string &event, int &completedPassengers, int totalPassengers);
    void printElevatorMovement(int &completedPassengers);

    // Helper functions for running elevator simulation
    void executePassengerAction(const PassengerAction &action, int &completedPassengers);
    void randomizePassengerBehaviour(int &completedPassengers);
    void processSafetyEvents(int currentTimeStep, int &completedPassengers, int totalPassengers);
    void processSimulationStep();

    // For displaying elevator states
    enum ElevatorState {
        Idle,
        Moving,
        Stopped
    };

private slots:
    void onStartClicked();
    void onStopClicked();
    void onPauseClicked();
    void onTimeout();

private:
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QPushButton *pauseBtn;
    QLineEdit *simTimeOutput;
    LogConsole *logConsole;
    BuildingSetup *buildingSetup;
    SafetyEventSetup *safetyEventSetup;
    PassengerBehaviourSetup *passengerBehaviourSetup;
    QTimer *timer;
    bool isPaused;
    bool simulationRunning;
    int elapsedTime;
    int elevatorCurrentFloor;
    ElevatorState elevatorState;
    int completedPassengers;

    // State tracking for non-blocking elevator movement
    int currentActionIndex;     // Tracks the current action being processed
    int currentFloorInMovement; // Tracks the current floor during elevator movement
    QList<PassengerAction> currentActionList; // Stores the list of actions for the current step
};

#endif // SIMULATIONCONTROLS_H
