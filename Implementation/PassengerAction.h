#ifndef PASSENGERACTION_H
#define PASSENGERACTION_H

#include <QString>

/**
 * @brief The PassengerAction struct Is a helper object for PassengerBehaviourSetup
 *          - Allows the log console to accurately display custom passenger behaviour
 */
struct PassengerAction {
    QString actionType;  // e.g., "RequestCar", "ExitCar", "OpenDoor", etc.
    int floor;           // The floor associated with the action
    int timeStep;        // The time step associated with the action

    PassengerAction(QString type, int f, int ts)
        : actionType(type), floor(f), timeStep(ts) {}
};

#endif // PASSENGERACTION_H
