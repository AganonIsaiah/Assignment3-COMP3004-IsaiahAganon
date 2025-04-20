#ifndef BUILDINGSETUP_H
#define BUILDINGSETUP_H

#include <QObject>
#include <QLineEdit>
#include "LogConsole.h"

/**
 * @brief The BuildingSetup class is responsible for setting:
 *        - The number of passengers
 *        - The number of floors
 *        - The number of elevators
 *        It displays the building setup on the log console
 */
class BuildingSetup : public QObject
{
    Q_OBJECT

public:
    explicit BuildingSetup(QLineEdit *passengersInput,
                          QLineEdit *floorsInput,
                          QLineEdit *elevatorsInput,
                          LogConsole *logConsole,
                          QObject *parent = nullptr);

    // Get functions default to 0 if input is empty
    int getPassengerCount() const;
    int getFloorCount() const;
    int getElevatorCount() const;

    // Displays building setup on log console
    void logBuildingParameters() const;

private:
    QLineEdit *passengersInput;
    QLineEdit *floorsInput;
    QLineEdit *elevatorsInput;
    LogConsole *logConsole;
};

#endif // BUILDINGSETUP_H
