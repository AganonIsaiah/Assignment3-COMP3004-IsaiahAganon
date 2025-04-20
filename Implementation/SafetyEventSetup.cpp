#include "SafetyEventSetup.h"

SafetyEventSetup::SafetyEventSetup(QLineEdit *helpTimeStep,
                                               QLineEdit *doorObstacleTimeStep,
                                               QLineEdit *fireTimeStep,
                                               QLineEdit *overloadTimeStep,
                                               QLineEdit *powerOutTimeStep,
                                               QPushButton *helpBtn,
                                               QPushButton *doorObstacleBtn,
                                               QPushButton *fireBtn,
                                               QPushButton *overloadBtn,
                                               QPushButton *powerOutBtn,
                                               LogConsole *logConsole,
                                               QObject *parent)
    : QObject(parent),
      helpTimeStep(helpTimeStep),
      doorObstacleTimeStep(doorObstacleTimeStep),
      fireTimeStep(fireTimeStep),
      overloadTimeStep(overloadTimeStep),
      powerOutTimeStep(powerOutTimeStep),
      helpBtn(helpBtn),
      doorObstacleBtn(doorObstacleBtn),
      fireBtn(fireBtn),
      overloadBtn(overloadBtn),
      powerOutBtn(powerOutBtn),
      logConsole(logConsole)
{
    // Connecting buttons
    connect(helpBtn, &QPushButton::clicked, this, &SafetyEventSetup::onHelpBtnClicked);
    connect(doorObstacleBtn, &QPushButton::clicked, this, &SafetyEventSetup::onDoorObstacleBtnClicked);
    connect(fireBtn, &QPushButton::clicked, this, &SafetyEventSetup::onFireBtnClicked);
    connect(overloadBtn, &QPushButton::clicked, this, &SafetyEventSetup::onOverloadBtnClicked);
    connect(powerOutBtn, &QPushButton::clicked, this, &SafetyEventSetup::onPowerOutBtnClicked);
}

/**
 * @brief Handles Help Button clicked by retrieving time step
 */
void SafetyEventSetup::onHelpBtnClicked()
{
    if (logConsole) {
        int timeStep = helpTimeStep->text().toInt();
        logConsole->logMessage(QString("Help button pressed. Triggers at time step: %1").arg(timeStep));
    }
}

/**
 * @brief Handles Door Obstacle Button clicked by retrieving time step
 */
void SafetyEventSetup::onDoorObstacleBtnClicked()
{
    if (logConsole) {
        int timeStep = doorObstacleTimeStep->text().toInt();
        logConsole->logMessage(QString("Door Obstacle button pressed. Triggers at time step: %1").arg(timeStep));
    }
}

/**
 * @brief Handles Fire Alarm Button clicked by retrieving time step
 */
void SafetyEventSetup::onFireBtnClicked()
{
    if (logConsole) {
        int timeStep = fireTimeStep->text().toInt();
        logConsole->logMessage(QString("Fire button pressed. Triggers at time step: %1").arg(timeStep));
    }
}

/**
 * @brief Handles OVerload Alarm Button clicked by retrieving time step
 */
void SafetyEventSetup::onOverloadBtnClicked()
{
    if (logConsole) {
        int timeStep = overloadTimeStep->text().toInt();
        logConsole->logMessage(QString("Overload button pressed. Triggers at time step: %1").arg(timeStep));
    }
}

/**
 * @brief Handles Power Out Alarm Button clicked by retrieving time step
 */
void SafetyEventSetup::onPowerOutBtnClicked()
{
    if (logConsole) {
        int timeStep = powerOutTimeStep->text().toInt();
        logConsole->logMessage(QString("Power Out button pressed. Triggers at time step: %1").arg(timeStep));
    }
}

int SafetyEventSetup::getHelpTimeStep() const
{
    if (helpTimeStep && !helpTimeStep->text().isEmpty()) {
        return helpTimeStep->text().toInt();
    }
    return -1;
}

int SafetyEventSetup::getDoorObstacleTimeStep() const
{
    if (doorObstacleTimeStep && !doorObstacleTimeStep->text().isEmpty()) {
        return doorObstacleTimeStep->text().toInt();
    }
    return -1;
}

int SafetyEventSetup::getFireTimeStep() const
{
    if (fireTimeStep && !fireTimeStep->text().isEmpty()) {
        return fireTimeStep->text().toInt();
    }
    return -1;
}

int SafetyEventSetup::getOverloadTimeStep() const
{
    if (overloadTimeStep && !overloadTimeStep->text().isEmpty()) {
        return overloadTimeStep->text().toInt();
    }
    return -1;
}

int SafetyEventSetup::getPowerOutTimeStep() const
{
    if (powerOutTimeStep && !powerOutTimeStep->text().isEmpty()) {
        return powerOutTimeStep->text().toInt();
    }
    return -1;
}

/**
 * @brief Displays safety event setup on log console
 */
void SafetyEventSetup::logSafetyEventParameters() const
{
    if (logConsole) {
        QString message = "Safety Event Setup:\n";
        bool hasInput = false;

        if (helpTimeStep && !helpTimeStep->text().isEmpty()) {
            message += QString("> Help Event Trigger Time Step: %1\n").arg(helpTimeStep->text().toInt());
            hasInput = true;
        }
        if (doorObstacleTimeStep && !doorObstacleTimeStep->text().isEmpty()) {
            message += QString("> Door Obstacle Trigger Time Step: %1\n").arg(doorObstacleTimeStep->text().toInt());
            hasInput = true;
        }
        if (fireTimeStep && !fireTimeStep->text().isEmpty()) {
            message += QString("> Fire Trigger Time Step: %1\n").arg(fireTimeStep->text().toInt());
            hasInput = true;
        }
        if (overloadTimeStep && !overloadTimeStep->text().isEmpty()) {
            message += QString("> Overload Trigger Time Step: %1\n").arg(overloadTimeStep->text().toInt());
            hasInput = true;
        }
        if (powerOutTimeStep && !powerOutTimeStep->text().isEmpty()) {
            message += QString("> Power Out Trigger Time Step: %1\n").arg(powerOutTimeStep->text().toInt());
            hasInput = true;
        }

        if (hasInput) {
            logConsole->logMessage(message.trimmed());
        }
    }
}

