#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger
{
public:
    static Logger& instance();
    
    void log(LogLevel level, const QString& message, const QString& module = "");
    void debug(const QString& message, const QString& module = "");
    void info(const QString& message, const QString& module = "");
    void warning(const QString& message, const QString& module = "");
    void error(const QString& message, const QString& module = "");
    void critical(const QString& message, const QString& module = "");
    
    void setLogLevel(LogLevel level);
    void setLogToFile(bool enabled);
    
private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    QString logLevelToString(LogLevel level);
    void writeLog(LogLevel level, const QString& message, const QString& module);
    
    QFile* logFile;
    QTextStream* logStream;
    LogLevel currentLevel;
    bool logToFile;
    QMutex mutex;
};

#define LOG_DEBUG(msg, mod) Logger::instance().debug(msg, mod)
#define LOG_INFO(msg, mod) Logger::instance().info(msg, mod)
#define LOG_WARNING(msg, mod) Logger::instance().warning(msg, mod)
#define LOG_ERROR(msg, mod) Logger::instance().error(msg, mod)
#define LOG_CRITICAL(msg, mod) Logger::instance().critical(msg, mod)

#endif // LOGGER_H

