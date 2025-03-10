#ifndef SAFETYEVENTSETUP_H
#define SAFETYEVENTSETUP_H

#include "LogConsole.h"

/**
 * @brief The SafetyEventSetup class is responsible for setting:
 *        - Help Alarm Event and time step
 *        - Door Obstacle Event and time step
 *        - Fire Alarm Event and time step
 *        - Overload Alarm Event and time step
 *        - Power Out Alarm Event and time step
 */
class SafetyEventSetup : public QObject
{
    Q_OBJECT

    public:
        explicit SafetyEventSetup(QLineEdit *helpTimeStep,
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
                                         QObject *parent = nullptr);

    // Displays log safety event setup on log console
    void logSafetyEventParameters () const;

    // Get functions default to -1 if no input value
    int getHelpTimeStep() const;
    int getDoorObstacleTimeStep() const;
    int getFireTimeStep() const;
    int getOverloadTimeStep() const;
    int getPowerOutTimeStep() const;

    // Handles buttons by retrieving and storing time steps
    private slots:
        void onHelpBtnClicked();
        void onDoorObstacleBtnClicked();
        void onFireBtnClicked();
        void onOverloadBtnClicked();
        void onPowerOutBtnClicked();

    private:
        QLineEdit *helpTimeStep;
        QLineEdit *doorObstacleTimeStep;
        QLineEdit *fireTimeStep;
        QLineEdit *overloadTimeStep;
        QLineEdit *powerOutTimeStep;
        QPushButton *helpBtn;
        QPushButton *doorObstacleBtn;
        QPushButton *fireBtn;
        QPushButton *overloadBtn;
        QPushButton *powerOutBtn;
        LogConsole *logConsole;

};

#endif // SAFETYEVENTSETUP_H
