#include "Logger.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

Logger::Logger() : currentLevel(LogLevel::Info), logToFile(true)
{
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(logDir)) {
        dir.mkpath(logDir);
    }
    
    QString logPath = logDir + "/SmartRadio.log";
    logFile = new QFile(logPath);
    
    if (logToFile && logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        logStream = new QTextStream(logFile);
        // Qt 6 uses UTF-8 by default, no need to set codec
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        logStream->setCodec("UTF-8");
        #endif
    } else {
        logStream = nullptr;
    }
}

Logger::~Logger()
{
    if (logStream) {
        delete logStream;
    }
    if (logFile) {
        logFile->close();
        delete logFile;
    }
}

void Logger::setLogLevel(LogLevel level)
{
    currentLevel = level;
}

void Logger::setLogToFile(bool enabled)
{
    logToFile = enabled;
}

QString Logger::logLevelToString(LogLevel level)
{
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::writeLog(LogLevel level, const QString& message, const QString& module)
{
    if (level < currentLevel) {
        return;
    }
    
    QMutexLocker locker(&mutex);
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString moduleStr = module.isEmpty() ? "APP" : module;
    QString logMessage = QString("[%1] [%2] [%3] %4")
                         .arg(timestamp)
                         .arg(logLevelToString(level))
                         .arg(moduleStr)
                         .arg(message);
    
    // Write to file
    if (logToFile && logStream) {
        *logStream << logMessage << Qt::endl;
        logStream->flush();
    }
    
    // Also output to console in debug mode
    #ifdef QT_DEBUG
    qDebug() << logMessage;
    #endif
}

void Logger::log(LogLevel level, const QString& message, const QString& module)
{
    writeLog(level, message, module);
}

void Logger::debug(const QString& message, const QString& module)
{
    writeLog(LogLevel::Debug, message, module);
}

void Logger::info(const QString& message, const QString& module)
{
    writeLog(LogLevel::Info, message, module);
}

void Logger::warning(const QString& message, const QString& module)
{
    writeLog(LogLevel::Warning, message, module);
}

void Logger::error(const QString& message, const QString& module)
{
    writeLog(LogLevel::Error, message, module);
}

void Logger::critical(const QString& message, const QString& module)
{
    writeLog(LogLevel::Critical, message, module);
}

