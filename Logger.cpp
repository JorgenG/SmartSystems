#include "Logger.h"
#include <time.h>

Logger *logger = new Logger();

LogEntry::LogEntry(QString errorMessage) {
    m_errorMessage = errorMessage;
    _strdate(m_dateStr);
    _strtime(m_timeStr);
}

QString LogEntry::getErrorStringWithTimestamp() {
    QString ss;
    ss.append(m_dateStr);
    ss.append(" ");
    ss.append(m_timeStr);
    ss.append(" ");
    ss.append(m_errorMessage);
    return ss;
}

Logger::Logger() {
    loggerLock = new QMutex();
    logEntries = new QStringList();
}

Logger::~Logger() {

}

void Logger::addEntry(QString errorMessage) {
    LogEntry logEntry(errorMessage);
    loggerLock->lock();
    logEntries->push_back(logEntry);
    loggerLock->unlock();
}

QString Logger::getEntry(int entryNumber) {
    QString errorStringWithTimestamp;
    loggerLock->lock();
    errorStringWithTimestamp = logEntries->at(entryNumber)->getErrorStringWithTimestamp();
    loggerLock->unlock();
    return errorStringWithTimestamp;
}

int Logger::getTotalEntries() {
    loggerLock->lock();
    int size = logEntries.size();
    loggerLock->unlock();
    return size;
}
