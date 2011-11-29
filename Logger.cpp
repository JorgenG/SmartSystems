#include "Logger.h"
#include <time.h>

Logger *logger = new Logger();

Logger::Logger() {
    loggerLock = new QMutex();
    logEntries = new QStringList();
}

Logger::~Logger() {

}

void Logger::addEntry(QString errorMessage) {
    QString ss;
    _strdate(m_dateStr);
    _strtime(m_timeStr);
    ss.append(m_dateStr);
    ss.append(" ");
    ss.append(m_timeStr);
    ss.append(" ");
    ss.append(errorMessage);
    loggerLock->lock();
    logEntries->push_back(ss);
    loggerLock->unlock();
}

QString Logger::getEntry(int entryNumber) {
    QString errorStringWithTimestamp;
    loggerLock->lock();
    errorStringWithTimestamp = logEntries->at(entryNumber);
    loggerLock->unlock();
    return errorStringWithTimestamp;
}

int Logger::getTotalEntries() {
    loggerLock->lock();
    int size = logEntries->size();
    loggerLock->unlock();
    return size;
}
