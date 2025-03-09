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
      elapsedTime(0),
      elevatorCurrentFloor(1),
      elevatorState(Idle)
{
    // Creating timer for simulation timer
    timer = new QTimer(this);
    timer->setInterval(1000);

    // Connecting buttons
    connect(startBtn, &QPushButton::clicked, this, &SimulationControls::onStartClicked);
    connect(stopBtn, &QPushButton::clicked, this, &SimulationControls::onStopClicked);
    connect(pauseBtn, &QPushButton::clicked, this, &SimulationControls::onPauseClicked);
    connect(timer, &QTimer::timeout, this, &SimulationControls::onTimeout);
}

/**
 * @brief Handles Start Button clicked by displaying all the log information from the setup classes on the log console and running the simulation
 */
void SimulationControls::onStartClicked()
{
    // Start time and print log information
    if (!timer->isActive()) {
        timer->start();
        isPaused = false;
        elapsedTime = 0;

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
    }

    logConsole->logMessage("----------------");
    runSimulation();
}

/**
 * @brief Handles Stop Button clicked by ending the simulation and resetting the time to 0
 */
void SimulationControls::onStopClicked()
{
    if (timer->isActive()) {
        timer->stop();
        isPaused = false;
        elapsedTime = 0;

        logConsole->logMessage("Simulation stopped.");
        if (simTimeOutput) {
            simTimeOutput->setText("0");
        }
    }
}

/**
 * @brief Handles Pause Button clicked by toggling timer's active state
 */
void SimulationControls::onPauseClicked()
{
    if (timer->isActive()) {
        if (isPaused) {
            timer->start();
            isPaused = false;
            if (logConsole) {
                logConsole->logMessage("Simulation resumed.");
            }
        } else {
            timer->stop();
            isPaused = true;
            if (logConsole) {
                logConsole->logMessage("Simulation paused.");
            }
        }
    }
}

/**
 * @brief Increments timer and updates display
 */
void SimulationControls::onTimeout()
{
    elapsedTime += timer->interval();
    if (simTimeOutput) {
        simTimeOutput->setText(QString::number(elapsedTime / 1000));
    }
}

/**
 * @brief Function runs until all passengers have reached their desired floors (i.e., number of passengers reaches 0)
 *        Handles all inputted safety events and custom passenger behaviours
 */
void SimulationControls::runSimulation() {
    if (logConsole) {
        logConsole->logMessage("Running Simulation...");
    }

    int passengerCount = buildingSetup->getPassengerCount();
    int completedPassengers = 0; // Simulation ends when this equals passengerCount
    int currentTimeStep = 0;

    // Process initialized passenger actions first
    QList<PassengerAction> actionList = passengerBehaviourSetup->getActionList();
    if (actionList.isEmpty()) {
        logConsole->logMessage("No actions in the action list.");
    } else {
        for (const auto &action : actionList) {
            if (completedPassengers >= passengerCount) break;
            logConsole->logMessage("----------------");
            executePassengerAction(action, completedPassengers);
        }
    }

    // Randomize behaviours and process remaining passengers
    while (completedPassengers < passengerCount) {
        logConsole->logMessage("----------------");
        currentTimeStep++;
        randomizePassengerBehaviour(completedPassengers);
        printElevatorMovement(completedPassengers);

        // Check for safety events at this time step
        processSafetyEvents(currentTimeStep, completedPassengers, passengerCount);

        // Update time display
        if (simTimeOutput) {
            simTimeOutput->setText(QString::number(currentTimeStep));
        }

        // Minor text output delays
        QApplication::processEvents();
        QThread::msleep(500);

        if (isPaused || !timer->isActive()) {
            logConsole->logMessage("Simulation interrupted.");
            break;
        }
    }

    logConsole->logMessage("----------------");
    logConsole->logMessage("All passengers have reached their destinations.");
    logConsole->logMessage("Simulation Complete");
    timer->stop();
}

/**
 * @brief Executes custom passenger behaviour
 * @param action The passenger's behaviour
 * @param completedPassengers Tracks number of passengers who reached their destination
 */
void SimulationControls::executePassengerAction(const PassengerAction &action, int &completedPassengers) {
    int passengerCount = buildingSetup->getPassengerCount();

    // Display elevator location and state
    logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                           .arg(elevatorCurrentFloor)
                           .arg(elevatorState == Idle ? "Idle" :
                                elevatorState == Moving ? "Moving" : "Stopped"));

    if (action.actionType == "RequestCar") {
        logConsole->logMessage(QString("> Passenger requested car at floor %1.").arg(action.floor));

        // Update elevator state to Moving
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
                QThread::msleep(200);
            }
        } else if (destinationFloor < elevatorCurrentFloor) {
            logConsole->logMessage("----------------");
            for (int floor = elevatorCurrentFloor - 1; floor >= destinationFloor; --floor) {
                logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
                QThread::msleep(200);
            }
        }

        elevatorCurrentFloor = destinationFloor; // Update elevator's current floor

        // Update elevator state to Stopped
        elevatorState = Stopped;
        logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                               .arg(elevatorCurrentFloor)
                               .arg("Stopped"));

        // Passenger enters the elevator
        logConsole->logMessage("> Passenger has entered the elevator.");
        QThread::msleep(200);

        // Simulate elevator moving to a random exit floor
        int exitFloor = (std::rand() % buildingSetup->getFloorCount()) + 1;

        // Update elevator state to Moving
        elevatorState = Moving;
        logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                               .arg(elevatorCurrentFloor)
                               .arg("Moving"));

        // Handles elevator movement from top to bottom or bottom to top
        if (exitFloor > elevatorCurrentFloor) {
            logConsole->logMessage("----------------");
            for (int floor = elevatorCurrentFloor + 1; floor <= exitFloor; ++floor) {
                logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
                QThread::msleep(200); // Simulate time between floors
            }
        } else if (exitFloor < elevatorCurrentFloor) {
            logConsole->logMessage("----------------");
            for (int floor = elevatorCurrentFloor - 1; floor >= exitFloor; --floor) {
                logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
                QThread::msleep(200); // Simulate time between floors
            }
        }
        elevatorCurrentFloor = exitFloor; // Update elevator's current floor

        // Update elevator state to Stopped
        elevatorState = Stopped;
        logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                               .arg(elevatorCurrentFloor)
                               .arg("Stopped"));

        // Passenger exits the elevator
        logConsole->logMessage(QString("> Passenger exited at floor %1.").arg(exitFloor));
        completedPassengers++;
    }
    else if (action.actionType == "ExitCar") {
        logConsole->logMessage(QString("> Passenger exited car at floor %1.").arg(action.floor));
        QThread::msleep(200);
        completedPassengers++;
    }
    else if (action.actionType == "OpenDoor") {
        logConsole->logMessage(QString("> Door opened at floor %1.").arg(action.floor));
        QThread::msleep(200);
    }
    else if (action.actionType == "CloseDoor") {
        logConsole->logMessage(QString("> Door closed at floor %1.").arg(action.floor));
        QThread::msleep(200);
    }
    else if (action.actionType == "PushHelp") {
        logConsole->logMessage(QString("> Passenger pushed help button at floor %1.").arg(action.floor));
        QThread::msleep(200);
        // Trigger the "help" safety event
        printSafetyEvent("help", completedPassengers, passengerCount);
    }

    // Update elevator state to Idle if no actions are being processed
    elevatorState = Idle;
    logConsole->logMessage(QString("Elevator is at floor %1, state: %2.")
                           .arg(elevatorCurrentFloor)
                           .arg("Idle"));

    // Log the current progress
    logConsole->logMessage(QString("Completed passengers: %1/%2")
                           .arg(completedPassengers).arg(passengerCount));
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

    // Process each action in the action list
    for (int i = 0; i < actionList.size(); ++i) {
        const auto &action = actionList[i];

        if (processedActions.contains(i)) {
            continue; // Skip already processed actions
        }

        // Check if all passengers have been completed
        if (completedPassengers >= passengerCount) {
            break;
        }

        // Simulate the action based on its type
        if (action.actionType == "RequestCar") {
            logConsole->logMessage(QString("> Passenger requested car at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));

            // Simulate elevator moving to the requested floor
            int destinationFloor = action.floor;
            if (destinationFloor > elevatorCurrentFloor) {
                logConsole->logMessage("----------------");
                for (int floor = elevatorCurrentFloor + 1; floor <= destinationFloor; ++floor) {
                    logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
                    QThread::msleep(200); // Simulate time between floors
                }
            } else if (destinationFloor < elevatorCurrentFloor) {
                logConsole->logMessage("----------------");
                for (int floor = elevatorCurrentFloor - 1; floor >= destinationFloor; --floor) {
                    logConsole->logMessage(QString("Elevator moving to floor %1...").arg(floor));
                    QThread::msleep(200); // Simulate time between floors
                }
            }
            elevatorCurrentFloor = destinationFloor; // Update elevator's current floor

            // Passenger enters the elevator
            logConsole->logMessage("> Passenger has entered the elevator.");
            processedActions.insert(i);
        }
        else if (action.actionType == "ExitCar") {
            logConsole->logMessage(QString("> Passenger exited car at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));

            // Simulate elevator stopping at the destination floor
            logConsole->logMessage(QString("> Stopping at floor %1...").arg(action.floor));
            QThread::msleep(200);

            // Passenger exits the elevator
            logConsole->logMessage("> Passenger has exited the elevator.");
            completedPassengers++;
            processedActions.insert(i);
            logConsole->logMessage(QString("Completed passengers: %1/%2")
                                   .arg(completedPassengers).arg(passengerCount));
        }
        else if (action.actionType == "OpenDoor") {
            logConsole->logMessage(QString("> Door opened at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));
            processedActions.insert(i);
        }
        else if (action.actionType == "CloseDoor") {
            logConsole->logMessage(QString("> Door closed at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));
            processedActions.insert(i);
        }
        else if (action.actionType == "PushHelp") {
            logConsole->logMessage(QString("> Help button pushed at floor %1 at time step %2.")
                                   .arg(action.floor).arg(action.timeStep));
            processedActions.insert(i);
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
        completedPassengers += totalPassengers - completedPassengers;
    }

    logConsole->logMessage("Elevator doors open (10 seconds).");
    logConsole->logMessage("Bell rings.");
    logConsole->logMessage("Elevator doors closed.");
    // Log the current progress
    logConsole->logMessage(QString("Completed passengers: %1/%2")
                           .arg(completedPassengers).arg(totalPassengers));

}


