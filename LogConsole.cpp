#include "LogConsole.h"

LogConsole::LogConsole(QTextEdit *logOutput, QObject *parent)
    : QObject(parent), logOutput(logOutput)
{
}

void LogConsole::logMessage(const QString &message)
{
    if (logOutput) {
        logOutput->append(message);
    }
}
