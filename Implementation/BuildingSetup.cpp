#include "BuildingSetup.h"

BuildingSetup::BuildingSetup(QLineEdit *passengersInput,
                           QLineEdit *floorsInput,
                           QLineEdit *elevatorsInput,
                           LogConsole *logConsole,
                           QObject *parent)
    : QObject(parent),
      passengersInput(passengersInput),
      floorsInput(floorsInput),
      elevatorsInput(elevatorsInput),
      logConsole(logConsole)
{
}

int BuildingSetup::getPassengerCount() const
{
    return passengersInput ? passengersInput->text().toInt() : 0;
}

int BuildingSetup::getFloorCount() const
{
    return floorsInput ? floorsInput->text().toInt() : 0;
}

int BuildingSetup::getElevatorCount() const
{
    return elevatorsInput ? elevatorsInput->text().toInt() : 0;
}

void BuildingSetup::logBuildingParameters() const
{
    if (logConsole) {
        QString message = QString("Building Setup:\n"
                                 "> Passengers: %1\n"
                                 "> Floors: %2\n"
                                 "> Elevators: %3")
                         .arg(getPassengerCount())
                         .arg(getFloorCount())
                         .arg(getElevatorCount());
        logConsole->logMessage(message);
    }
}
