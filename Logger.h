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

    /**
      This method is used when adding an entry to the log. Will
      automatically add a timestamp when added.
      */
    void addEntry(QString errorMessage);

    /**
      Returns an entry specified by entryNumber
      */
    QString getEntry(int entryNumber);

    /**
      Returns the total amount of entries
      */
    int getTotalEntries();

private:
    QMutex *SLoggerLock;
    QStringList *logEntries;
    char m_dateStr[9];
    char m_timeStr[9];
signals:
    /**
      Signals to the GUI when a logentry is added.
      */
    void logEntryAdded();
};

extern Logger*logger;

#endif // SLOGGER_H
