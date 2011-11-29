#include <QString>
#include <QStringList>
#include <QMutex>

class LogEntry {
public:
        LogEntry(QString errorMessage);
        QString getErrorStringWithTimestamp();
private:
        char m_dateStr[9];
        char m_timeStr[9];
        QString m_errorMessage;
};

class Logger {
public: 
	Logger();
	~Logger();
        void addEntry(QString errorMessage);
        QString getEntry(int entryNumber);
        int getTotalEntries();

private:
        QMutex *loggerLock;
        QStringList *logEntries;
};

extern Logger *logger;
