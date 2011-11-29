#ifndef SLOGGER_H
#define SLOGGER_H

#include <QString>
#include <QStringList>
#include <QMutex>
#include <QObject>

class Logger: public QObject {
    Q_OBJECT
public:
        explicit Logger(QObject *parent = 0);
        void addEntry(QString errorMessage);
        QString getEntry(int entryNumber);
        int getTotalEntries();

private:
        QMutex *SLoggerLock;
        QStringList *logEntries;
        char m_dateStr[9];
        char m_timeStr[9];
signals:
        void logEntryAdded();
};

extern Logger*logger;

#endif // SLOGGER_H
