#include "logger.h"
#include <time.h>

Logger *logger = new Logger();

Logger::Logger(QObject *parent) :
        QObject(parent)
{
    SLoggerLock = new QMutex();
    logEntries = new QStringList();
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
    SLoggerLock->lock();
    logEntries->push_back(ss);
    SLoggerLock->unlock();
    emit logEntryAdded();
}

QString Logger::getEntry(int entryNumber) {
    QString errorStringWithTimestamp;
    SLoggerLock->lock();
    errorStringWithTimestamp = logEntries->at(entryNumber);
    SLoggerLock->unlock();
    return errorStringWithTimestamp;
}

int Logger::getTotalEntries() {
    SLoggerLock->lock();
    int size = logEntries->size();
    SLoggerLock->unlock();
    return size;
}

