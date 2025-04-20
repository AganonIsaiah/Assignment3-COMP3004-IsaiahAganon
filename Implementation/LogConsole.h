#ifndef LOGCONSOLE_H
#define LOGCONSOLE_H

#include <QObject>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

/**
 * @brief The LogConsole class is responsible for logging:
 *        - The passengers actions
 *        - The elevators movement
 *        - The handling of safety events
 *        - System responses
 *        - The running state of the simulation
 */
class LogConsole : public QObject
{
    Q_OBJECT

public:
    explicit LogConsole(QTextEdit *logOutput, QObject *parent = nullptr);
    void logMessage(const QString &message);

private:
    QTextEdit *logOutput;
};

#endif // LOGCONSOLE_H
