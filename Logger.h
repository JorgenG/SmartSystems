#include <QString>
#include <QStringList>
#include <QMutex>

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
        char m_dateStr[9];
        char m_timeStr[9];
};

extern Logger *logger;
