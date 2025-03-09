#include "PassengerBehaviourSetup.h"

PassengerBehaviourSetup::PassengerBehaviourSetup(QLineEdit *passIdInput,
                                                 LogConsole *logConsole,
                                                 QPushButton *closeDoorBtn,
                                                 QLineEdit *closeDoorFloor,
                                                 QLineEdit *closeDoorTimeStep,
                                                 QPushButton *exitCarBtn,
                                                 QLineEdit *exitCarFloor,
                                                 QLineEdit *exitCarTimeStep,
                                                 QPushButton *openDoorBtn,
                                                 QLineEdit *openDoorFloor,
                                                 QLineEdit *openDoorTimeStep,
                                                 QPushButton *pushHelpBtn,
                                                 QLineEdit *pushHelpFloor,
                                                 QLineEdit *pushHelpTimeStep,
                                                 QPushButton *requestCarBtn,
                                                 QLineEdit *requestCarFloor,
                                                 QLineEdit *requestCarTimeStep,
                                                 QObject *parent)
    : QObject(parent),
      passIdInput(passIdInput),
      logConsole(logConsole),
      closeDoorBtn(closeDoorBtn),
      closeDoorFloor(closeDoorFloor),
      closeDoorTimeStep(closeDoorTimeStep),
      exitCarBtn(exitCarBtn),
      exitCarFloor(exitCarFloor),
      exitCarTimeStep(exitCarTimeStep),
      openDoorBtn(openDoorBtn),
      openDoorFloor(openDoorFloor),
      openDoorTimeStep(openDoorTimeStep),
      pushHelpBtn(pushHelpBtn),
      pushHelpFloor(pushHelpFloor),
      pushHelpTimeStep(pushHelpTimeStep),
      requestCarBtn(requestCarBtn),
      requestCarFloor(requestCarFloor),
      requestCarTimeStep(requestCarTimeStep)
{
    // Connecting buttons
    connect(requestCarBtn, &QPushButton::clicked, this, &PassengerBehaviourSetup::onRequestBtnClicked);
    connect(exitCarBtn, &QPushButton::clicked, this, &PassengerBehaviourSetup::onExitCarBtnClicked);
    connect(openDoorBtn, &QPushButton::clicked, this, &PassengerBehaviourSetup::onOpenDoorBtnClicked);
    connect(closeDoorBtn, &QPushButton::clicked, this, &PassengerBehaviourSetup::onCloseDoorBtnClicked);
    connect(pushHelpBtn, &QPushButton::clicked, this, &PassengerBehaviourSetup::onPushHelpBtnClicked);
}

/**
 * @brief Handles Request Car button by retrieving time step, floor, and passenger id, and adding information to actionList
 */
void PassengerBehaviourSetup::onRequestBtnClicked()
{
    if (logConsole) {
        QString passengerId = passIdInput->text();
        int floor = requestCarFloor->text().toInt();
        int timeStep = requestCarTimeStep->text().toInt();
        actionList.append(PassengerAction("RequestCar", floor, timeStep));
                logConsole->logMessage(QString("Passenger %1 requested car at floor %2 at time step %3.")
                               .arg(passengerId).arg(floor).arg(timeStep));
    }
}

/**
 * Handles Exit Car button by retrieving time step, floor, and passenger id, and adding information to actionList
 */
void PassengerBehaviourSetup::onExitCarBtnClicked()
{
    if (logConsole) {
        QString passengerId = passIdInput->text();
        int floor = exitCarFloor->text().toInt();
        int timeStep = exitCarTimeStep->text().toInt();
        actionList.append(PassengerAction("ExitCar", floor, timeStep));
        logConsole->logMessage(QString("Passenger %1 exited car at floor %2 at time step %3.")
                               .arg(passengerId).arg(floor).arg(timeStep));
    }
}

/**
 * Handles Open Door button by retrieving time step, floor, and passenger id, and adding information to actionList
 */
void PassengerBehaviourSetup::onOpenDoorBtnClicked()
{
    if (logConsole) {
        QString passengerId = passIdInput->text();
        int floor = openDoorFloor->text().toInt();
        int timeStep = openDoorTimeStep->text().toInt();
        actionList.append(PassengerAction("OpenDoor", floor, timeStep));
        logConsole->logMessage(QString("Passenger %1 opened door at floor %2 at time step %3.")
                               .arg(passengerId).arg(floor).arg(timeStep));
    }
}

/**
 * Handles Close Door button by retrieving time step, floor, and passenger id, and adding information to actionList
 */
void PassengerBehaviourSetup::onCloseDoorBtnClicked()
{
    if (logConsole) {
        QString passengerId = passIdInput->text();
        int floor = closeDoorFloor->text().toInt();
        int timeStep = closeDoorTimeStep->text().toInt();
        actionList.append(PassengerAction("CloseDoor", floor, timeStep));
        logConsole->logMessage(QString("Passenger %1 closed door at floor %2 at time step %3.")
                               .arg(passengerId).arg(floor).arg(timeStep));
    }
}

/**
 * Handles Push Help button by retrieving time step, floor, and passenger id, and adding information to actionList
 */
void PassengerBehaviourSetup::onPushHelpBtnClicked()
{
    if (logConsole) {
        QString passengerId = passIdInput->text();
        int floor = pushHelpFloor->text().toInt();
        int timeStep = pushHelpTimeStep->text().toInt();
        actionList.append(PassengerAction("PushHelp", floor, timeStep));
        logConsole->logMessage(QString("Passenger %1 pushed help button at floor %2 at time step %3.")
                               .arg(passengerId).arg(floor).arg(timeStep));
    }
}

/**
 * @brief Displays passenger behaviour setup on log console
 */
void PassengerBehaviourSetup::logPassengerBehaviourSetup() const
{
    if (logConsole) {
        QString message = "Passenger Behaviour Setup:\n";
        bool hasInput = false;

        if (passIdInput && !passIdInput->text().isEmpty()) {
            message += QString("> Passenger ID: %1\n").arg(passIdInput->text());
            hasInput = true;
        }
        if (requestCarFloor && !requestCarFloor->text().isEmpty()) {
            message += QString("> Request Car Floor: %1\n").arg(requestCarFloor->text().toInt());
            hasInput = true;
        }
        if (requestCarTimeStep && !requestCarTimeStep->text().isEmpty()) {
            message += QString("> Request Car Time Step: %1\n").arg(requestCarTimeStep->text().toInt());
            hasInput = true;
        }
        if (exitCarFloor && !exitCarFloor->text().isEmpty()) {
            message += QString("> Exit Car Floor: %1\n").arg(exitCarFloor->text().toInt());
            hasInput = true;
        }
        if (exitCarTimeStep && !exitCarTimeStep->text().isEmpty()) {
            message += QString("> Exit Car Time Step: %1\n").arg(exitCarTimeStep->text().toInt());
            hasInput = true;
        }
        if (openDoorFloor && !openDoorFloor->text().isEmpty()) {
            message += QString("> Open Door Floor: %1\n").arg(openDoorFloor->text().toInt());
            hasInput = true;
        }
        if (openDoorTimeStep && !openDoorTimeStep->text().isEmpty()) {
            message += QString("> Open Door Time Step: %1\n").arg(openDoorTimeStep->text().toInt());
            hasInput = true;
        }
        if (closeDoorFloor && !closeDoorFloor->text().isEmpty()) {
            message += QString("> Close Door Floor: %1\n").arg(closeDoorFloor->text().toInt());
            hasInput = true;
        }
        if (closeDoorTimeStep && !closeDoorTimeStep->text().isEmpty()) {
            message += QString("> Close Door Time Step: %1\n").arg(closeDoorTimeStep->text().toInt());
            hasInput = true;
        }
        if (pushHelpFloor && !pushHelpFloor->text().isEmpty()) {
            message += QString("> Push Help Floor: %1\n").arg(pushHelpFloor->text().toInt());
            hasInput = true;
        }
        if (pushHelpTimeStep && !pushHelpTimeStep->text().isEmpty()) {
            message += QString("> Push Help Time Step: %1\n").arg(pushHelpTimeStep->text().toInt());
            hasInput = true;
        }

        if (hasInput) {
            logConsole->logMessage(message.trimmed());
        }
    }
}
