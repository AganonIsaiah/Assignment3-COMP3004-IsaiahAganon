#ifndef PASSENGERBEHAVIOURSETUP_H
#define PASSENGERBEHAVIOURSETUP_H

#include "LogConsole.h"
#include "PassengerAction.h"

/**
 * @brief The PassengerBehaviourSetup class is responsible for setting passengers':
 *        - Car requests at floor and time step
 *        - Exit from car at floor and time step
 *        - Push help button at floor and time step
 *        - Open door at floor and time step
 *        - Close door at floor and time step
 *
 */
class PassengerBehaviourSetup : public QObject {
    Q_OBJECT

    public:
        explicit PassengerBehaviourSetup(QLineEdit *passIdInput,
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
                                         QObject *parent = nullptr
                                         );

       // Displays custom passenger behaviour to log console
       void logPassengerBehaviourSetup () const;

       // Returns list of passengers' actions
       QList<PassengerAction> getActionList() const {
           return actionList;
       }

    // Handles buttons by retrieving time step, floor, passenger id, and adding information to actionList
    private slots:
       void onRequestBtnClicked();
       void onExitCarBtnClicked();
       void onOpenDoorBtnClicked();
       void onCloseDoorBtnClicked();
       void onPushHelpBtnClicked();

    private:
       QLineEdit *passIdInput;
       LogConsole *logConsole;
       QPushButton *closeDoorBtn;
       QLineEdit *closeDoorFloor;
       QLineEdit *closeDoorTimeStep;
       QPushButton *exitCarBtn;
       QLineEdit *exitCarFloor;
       QLineEdit *exitCarTimeStep;
       QPushButton *openDoorBtn;
       QLineEdit *openDoorFloor;
       QLineEdit *openDoorTimeStep;
       QPushButton *pushHelpBtn;
       QLineEdit *pushHelpFloor;
       QLineEdit *pushHelpTimeStep;
       QPushButton *requestCarBtn;
       QLineEdit *requestCarFloor;
       QLineEdit *requestCarTimeStep;
       QList<PassengerAction> actionList;
};



#endif // PASSENGERBEHAVIOURSETUP_H
