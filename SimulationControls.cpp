#include "SimulationControls.h"

SimulationControls::SimulationControls(QPushButton *startBtn,
                                       QPushButton *stopBtn,
                                       QPushButton *pauseBtn,
                                       QLineEdit *simTimeOutput,
                                       LogConsole *logConsole,
                                       BuildingSetup *buildingSetup,
                                       SafetyEventSetup *safetyEventSetup,
                                       PassengerBehaviourSetup *passengerBehaviourSetup,
                                       QObject *parent)
    : QObject(parent),
      startBtn(startBtn),
      stopBtn(stopBtn),
      pauseBtn(pauseBtn),
      simTimeOutput(simTimeOutput),
      logConsole(logConsole),
      buildingSetup(buildingSetup),
      safetyEventSetup(safetyEventSetup),
      passengerBehaviourSetup(passengerBehaviourSetup),
      isPaused(false),
      simulationRunning(false),
      elapsedTime(0),
      elevatorCurrentFloor(1),
      elevatorState(Idle),
      completedPassengers(0),
      currentActionIndex(-1),
      currentFloorInMovement(0)
{
    // Creating timer for simulation timer
    timer = new QTimer(this);
    timer->setInterval(1000); // 1-second interval

    // Connecting buttons
    connect(startBtn, &QPushButton::clicked, this, &SimulationControls::onStartClicked);
    connect(stopBtn, &QPushButton::clicked, this, &SimulationControls::onStopClicked);
    connect(pauseBtn, &QPushButton::clicked, this, &SimulationControls::onPauseClicked);
    connect(timer, &QTimer::timeout, this, &SimulationControls::onTimeout);
}

/**
 * @brief Handles Start Button clicked and prints setups
 */
void SimulationControls::onStartClicked()
{
    if (!timer->isActive()) {
        timer->start();
        isPaused = false;
        elapsedTime = 0;
        completedPassengers = 0;
        simulationRunning = true;
        currentActionIndex = -1;
        currentFloorInMovement = elevatorCurrentFloor;

        // Log setup information
        if (logConsole) {
            logConsole->logMessage("Simulation started.");

            if (buildingSetup) {
                buildingSetup->logBuildingParameters();
            }
            if (safetyEventSetup) {
                safetyEventSetup->logSafetyEventParameters();
            }
            if (passengerBehaviourSetup) {
                passengerBehaviourSetup->logPassengerBehaviourSetup();
            }
        }

        logConsole->logMessage("----------------");
        processSimulationStep();
    }
}

/**
 * @brief Handles Stop Button clicked and sets simulation timer to 0
 */
void SimulationControls::onStopClicked()
{
    if (timer->isActive()) {
        timer->stop();
        isPaused = false;
        elapsedTime = 0;
        simulationRunning = false;
        currentActionIndex = -1;
        currentFloorInMovement = elevatorCurrentFloor;

        logConsole->logMessage("Simulation stopped.");
        if (simTimeOutput) {
            simTimeOutput->setText("0");
        }
    }
}

/**
 * @brief Handles Pause Button clicked and toggles simulation timer
 */
void SimulationControls::onPauseClicked()
{
    if (timer->isActive() && !isPaused) {
        timer->stop();
        isPaused = true;
        if (logConsole) {
            logConsole->logMessage("Simulation paused.");
        }
    } else if (isPaused && simulationRunning) {
        timer->start();
        isPaused = false;
        if (logConsole) {
            logConsole->logMessage("Simulation resumed.");
        }
    }
}

/**
 * @brief Increments simulation timer
 */
void SimulationControls::onTimeout()
{
    elapsedTime += timer->interval();
    if (simTimeOutput) {
        simTimeOutput->setText(QString::number(elapsedTime / 1000));
    }

    if (simulationRunning) {
        processSimulationStep();
    }
}

/**
 * @brief Manages simulation time display
 */
void SimulationControls::processSimulationStep()
{
    int currentTimeStep = elapsedTime / 1000;
    int passengerCount = buildingSetup->getPassengerCount();

    // Check if all passengers have reached their destinations
    if (completedPassengers >= passengerCount) {
        logConsole->logMessage("----------------");
        logConsole->logMessage("All passengers have reached their destinations.");
        logConsole->logMessage("Simulation Complete");
        timer->stop();
        simulationRunning = false;
        return;
    }

    logConsole->logMessage("----------------");

    // Process actions that should happen at this time step
    QList<PassengerAction> actionList = passengerBehaviourSetup->getActionList();
    bool actionsProcessed = false;

    for (const auto &action : actionList) {
        if (action.timeStep == currentTimeStep) {
            executePassengerAction(action, completedPassengers);
            actionsProcessed = true;
        }
    }

    // If no scheduled actions, generate random behavior
    if (!actionsProcessed) {
        randomizePassengerBehaviour(completedPassengers);
    }

    printElevatorMovement(completedPassengers);

    // Check for safety events at this time step
    processSafetyEvents(currentTimeStep, completedPassengers, passengerCount);

    // Check again if all passengers have been completed after this step
    if (completedPassengers >= passengerCount) {
        logConsole->logMessage("----------------");
        logConsole->logMessage("All passengers have reached their destinations.");
        logConsole->logMessage("Simulation Complete");
        timer->stop();
        simulationRunning = false;
    }

    // Minor text output delays
    QApplication::processEvents();
}

/**
 * @brief Manages passengers' behaviours
 * @param action Passengers' behaviours
 * @param completedPassengers Number of passengers who reached their destination
 */
void SimulationControls::executePassengerAction(const PassengerAction &action, int &completedPassengers)
{
    int passengerCount = buildingSetup->getPassengerCount();

    // Display elevator location and state
    logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                           .arg(elevatorCurrentFloor)
                           .arg(elevatorState == Idle ? "Idle" :
                                elevatorState == Moving ? "Moving" : "Stopped"));

    if (action.actionType == "RequestCar") {
        logConsole->logMessage(QString("> Passenger requested car at floor %1.").arg(action.floor));
        elevatorState = Moving;
        logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                              .arg(elevatorCurrentFloor)
                              .arg("Moving"));

        // Simulate elevator moving to the requested floor
        int destinationFloor = action.floor;
        if (destinationFloor > elevatorCurrentFloor) {
            logConsole->logMessage("----------------");
            for (int floor = elevatorCurrentFloor + 1; floor <= destinationFloor; ++floor) {
                logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
            }
        } else if (destinationFloor < elevatorCurrentFloor) {
            logConsole->logMessage("----------------");
            for (int floor = elevatorCurrentFloor - 1; floor >= destinationFloor; --floor) {
                logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
            }
        }

        elevatorCurrentFloor = destinationFloor;
        elevatorState = Stopped;
        logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                              .arg(elevatorCurrentFloor)
                              .arg("Stopped"));

        logConsole->logMessage("> Passenger has entered the elevator.");

    } else if (action.actionType == "ExitCar") {
        int exitFloor = action.floor;
        if (elevatorCurrentFloor != exitFloor) {
            elevatorState = Moving;
            logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                                  .arg(elevatorCurrentFloor)
                                  .arg("Moving"));

            if (exitFloor > elevatorCurrentFloor) {
                logConsole->logMessage("----------------");
                for (int floor = elevatorCurrentFloor + 1; floor <= exitFloor; ++floor) {
                    logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
                }
            } else if (exitFloor < elevatorCurrentFloor) {
                logConsole->logMessage("----------------");
                for (int floor = elevatorCurrentFloor - 1; floor >= exitFloor; --floor) {
                    logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
                }
            }

            elevatorCurrentFloor = exitFloor;
        }

        elevatorState = Stopped;
        logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                              .arg(elevatorCurrentFloor)
                              .arg("Stopped"));

        logConsole->logMessage(QString("> Passenger exited at floor %1.").arg(exitFloor));
        completedPassengers++;
        logConsole->logMessage(QString("Completed passengers: %1/%2")
                              .arg(completedPassengers).arg(passengerCount));
    }

    elevatorState = Idle;
    logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                          .arg(elevatorCurrentFloor)
                          .arg("Idle"));
}

/**
 * @brief Randomizes passengers' behaviour and runs "RequestCar" by default
 * @param completedPassengers Tracks number of passengers who reached their destinations
 */
void SimulationControls::randomizePassengerBehaviour(int &completedPassengers) {
    int remainingPassengers = buildingSetup->getPassengerCount() - completedPassengers;
    if (remainingPassengers <= 0) return;

    // Generate random entry floor and have passenger exit on random exit floor
    int randomFloor = (std::rand() % buildingSetup->getFloorCount()) + 1;
    PassengerAction randomAction = {"RequestCar", randomFloor, 0};
    executePassengerAction(randomAction, completedPassengers);

    // Add exit action to complete the passenger's journey
    // Randomly select a different floor for the passenger to exit at
    int exitFloor;
    do {
        exitFloor = (std::rand() % buildingSetup->getFloorCount()) + 1;
    } while (exitFloor == randomFloor);

    PassengerAction exitAction = {"ExitCar", exitFloor, 0};
    executePassengerAction(exitAction, completedPassengers);
}

/**
 * @brief Handles safety event cases
 * @param currentTimeStep The current simulation time
 * @param completedPassengers Tracks number of passengers who reached their destinations
 * @param totalPassengers Tracks total number of passengers in simulation
 */
void SimulationControls::processSafetyEvents(int currentTimeStep, int &completedPassengers, int totalPassengers) {
    if (safetyEventSetup) {
        if (currentTimeStep == safetyEventSetup->getHelpTimeStep()) {
            printSafetyEvent("help", completedPassengers, totalPassengers);
        }
        if (currentTimeStep == safetyEventSetup->getDoorObstacleTimeStep()) {
            printSafetyEvent("doorobstacle", completedPassengers, totalPassengers);
        }
        if (currentTimeStep == safetyEventSetup->getFireTimeStep()) {
            printSafetyEvent("fire", completedPassengers, totalPassengers);
        }
        if (currentTimeStep == safetyEventSetup->getOverloadTimeStep()) {
            printSafetyEvent("overload", completedPassengers, totalPassengers);
        }
        if (currentTimeStep == safetyEventSetup->getPowerOutTimeStep()) {
            printSafetyEvent("powerout", completedPassengers, totalPassengers);
        }
    }
}

/**
 * @brief Handles displaying elevator movement, accounts for elevators changing between floors to get to destination
 * @param completedPassengers Tracks number of passengers who reached their destinations
 */
void SimulationControls::printElevatorMovement(int &completedPassengers) {
    // Get the action list from PassengerBehaviourSetup
    QList<PassengerAction> actionList = passengerBehaviourSetup->getActionList();
    int passengerCount = buildingSetup->getPassengerCount(); // Total number of passengers in simulation
    static QSet<int> processedActions; // Store indices of processed actions
    int currentTimeStep = elapsedTime / 1000;

    // Process each action in the action list
    for (int i = 0; i < actionList.size(); ++i) {
        const auto &action = actionList[i];

        if (processedActions.contains(i)) {
            continue; // Skip already processed actions
        }

        // Skip actions that don't belong to this time step
        if (action.timeStep != currentTimeStep) {
            continue;
        }

        // Check if all passengers have been completed
        if (completedPassengers >= passengerCount) {
            break;
        }

        // Simulate the action based on its type
        if (action.actionType == "OpenDoor") {
            logConsole->logMessage(QString("> Door opened at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));
            processedActions.insert(i);
        }
        if (action.actionType == "CloseDoor") {
            logConsole->logMessage(QString("> Door closed at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));
            processedActions.insert(i);
        }
        if (action.actionType == "PushHelp") {
            logConsole->logMessage(QString("> Help button pushed at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));
            processedActions.insert(i);
            // Call the printSafetyEvent function to handle the "help" safety event
               int passengerCount = buildingSetup->getPassengerCount();
               printSafetyEvent("help", completedPassengers, passengerCount);

        }
        if (action.actionType == "RequestCar") {
            logConsole->logMessage(QString("> Passenger requested car at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));

            // Simulate elevator moving to the requested floor
            int destinationFloor = action.floor;
            if (destinationFloor > elevatorCurrentFloor) {
                logConsole->logMessage("----------------");
                for (int floor = elevatorCurrentFloor + 1; floor <= destinationFloor; ++floor) {
                    logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
//                    QThread::msleep(200); // Simulate time between floors
                }
            } else if (destinationFloor < elevatorCurrentFloor) {
                logConsole->logMessage("----------------");
                for (int floor = elevatorCurrentFloor - 1; floor >= destinationFloor; --floor) {
                    logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
//                    QThread::msleep(200); // Simulate time between floors
                }
            }
            elevatorCurrentFloor = destinationFloor; // Update elevator's current floor

            // Passenger enters the elevator
            logConsole->logMessage("> Passenger has entered the elevator.");
            processedActions.insert(i);
        }
        if (action.actionType == "ExitCar") {
            logConsole->logMessage(QString("> Passenger exited car at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));

            // Simulate elevator stopping at the destination floor
            logConsole->logMessage(QString("> Stopping at floor %1...").arg(action.floor));
//            QThread::msleep(200);

            // Passenger exits the elevator
            logConsole->logMessage("> Passenger has exited the elevator.");
            completedPassengers++;
            processedActions.insert(i);
            logConsole->logMessage(QString("Completed passengers: %1/%2")
                                   .arg(completedPassengers).arg(passengerCount));
        }
    }
}

/**
 * @brief Displays safety events, in each event >= 1 passengers is completed, allowing for runSimulation to reach its base case
 * @param event The safety event
 * @param completedPassengers Tracks the number of passengers who reached their destination
 * @param totalPassengers Tracks the total number of passengers in simulation
 *
 */
void SimulationControls:: printSafetyEvent(const std::string &event, int &completedPassengers, int totalPassengers)
{
    // Generates random number for handling cases where safety events are resolved or worsen
    static bool seeded = false;
    if (!seeded){
        std::srand(std::time(nullptr));
        seeded = true;
    }

    logConsole->logMessage("----------------");
    if (event == "help"){
        logConsole->logMessage("Help Alarm Triggered");
        logConsole->logMessage("> Stay calm, connecting passenger to building safety services.");

        // 50/50 chance that the building safety responds
        if (std::rand() % 2 == 0){
            logConsole->logMessage("> Connected to building safety services. Please remain calm, help is on the way");
        } else {
            logConsole->logMessage("> Unable to contact building safety services, 911 emergency call has been placed.");
        }
        completedPassengers++;
     }

    if (event == "doorobstacle"){
        logConsole->logMessage("Door Obstacle Triggered by Light Sensors");
        logConsole->logMessage("Elevator doors remain open.");
        logConsole->logMessage("> Please remove the obstacle blocking the door!");

        // 50/50 chance that door obstacle is moved,
        if (std::rand() % 2 == 0){
            logConsole->logMessage("> Obstacle has been moved.");
        } else {
            logConsole->logMessage("> Obstacle has not been moved.");
            logConsole->logMessage("> Passengers are asked to disembark.");
        }
        completedPassengers++;
    }

    // Moves either the elevator, or all the elevators to their safe floors
    if (event == "fire"){
        logConsole->logMessage("Fire Alarm Triggered");
        logConsole->logMessage("> Stay calm, moving the elevator(s) to a safe floor.");
        // 50/50 chance that all elevators experience the fire signal
        if (std::rand() % 2 == 0){
            logConsole->logMessage("> All elevators have reached a safe floor, please exit!");
            completedPassengers += totalPassengers - completedPassengers;
        } else {
            logConsole->logMessage("Elevator has reached a safe floor, please exit");
            completedPassengers++;
        }
    }

    if (event == "overload"){
        logConsole->logMessage("Overload Alarm Triggered");
        logConsole->logMessage("> Please reduce the weight load before the elevator proceeds.");

        // 50/50 chance that the load is moved
        if (std::rand() % 2 == 0){
            logConsole->logMessage("> Load has been moved, elevator will commence.");
        } else {
            logConsole->logMessage("Elevator is still overloaded.");
            logConsole->logMessage("> Passengers are asked to disembark.");
        }
        completedPassengers++;
    }

    // All elevators reach their safe floors, and everyone exits, ending the simulation
    if (event == "powerout") {
        logConsole->logMessage("Power Out Alarm Triggered");
        logConsole->logMessage("> Stay calm, moving the elevators to a safe floor.");
        logConsole->logMessage("> All elevators have reached a safe floor, please exit!");
        logConsole->logMessage("All passengers have exited the elevators.");
        completedPassengers += totalPassengers - completedPassengers;
    }

    logConsole->logMessage("Elevator doors open (10 seconds).");
    logConsole->logMessage("Bell rings.");
    logConsole->logMessage("Elevator doors closed.");
    // Log the current progress
    logConsole->logMessage(QString("Completed passengers: %1/%2")
                           .arg(completedPassengers).arg(totalPassengers));
}
