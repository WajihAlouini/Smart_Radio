#include "InputValidator.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

QString InputValidator::lastError = "";

bool InputValidator::validateId(const QString& id, int min, int max)
{
    bool ok;
    int value = id.toInt(&ok);
    
    if (!ok) {
        lastError = "ID must be a valid number";
        return false;
    }
    
    if (value < min || value > max) {
        lastError = QString("ID must be between %1 and %2").arg(min).arg(max);
        return false;
    }
    
    lastError = "";
    return true;
}

bool InputValidator::validateName(const QString& name, int minLength, int maxLength)
{
    QString trimmed = name.trimmed();
    
    if (trimmed.isEmpty()) {
        lastError = "Name cannot be empty";
        return false;
    }
    
    if (trimmed.length() < minLength || trimmed.length() > maxLength) {
        lastError = QString("Name must be between %1 and %2 characters").arg(minLength).arg(maxLength);
        return false;
    }
    
    lastError = "";
    return true;
}

bool InputValidator::validateTime(const QString& time)
{
    QRegularExpression timeRegex("^([0-1]?[0-9]|2[0-3]):[0-5][0-9](:[0-5][0-9])?$");
    QRegularExpressionMatch match = timeRegex.match(time);
    
    if (!match.hasMatch()) {
        lastError = "Time must be in format HH:MM or HH:MM:SS (24-hour format)";
        return false;
    }
    
    lastError = "";
    return true;
}

bool InputValidator::validateAmount(const QString& amount, double min, double max)
{
    bool ok;
    double value = amount.toDouble(&ok);
    
    if (!ok) {
        lastError = "Amount must be a valid number";
        return false;
    }
    
    if (value < min || value > max) {
        lastError = QString("Amount must be between %1 and %2").arg(min).arg(max);
        return false;
    }
    
    lastError = "";
    return true;
}

bool InputValidator::validateDuration(const QString& duration, int min, int max)
{
    bool ok;
    int value = duration.toInt(&ok);
    
    if (!ok) {
        lastError = "Duration must be a valid number (minutes)";
        return false;
    }
    
    if (value < min || value > max) {
        lastError = QString("Duration must be between %1 and %2 minutes").arg(min).arg(max);
        return false;
    }
    
    lastError = "";
    return true;
}

bool InputValidator::validateEmail(const QString& email)
{
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    QRegularExpressionMatch match = emailRegex.match(email);
    
    if (!match.hasMatch()) {
        lastError = "Invalid email format";
        return false;
    }
    
    lastError = "";
    return true;
}

bool InputValidator::validatePhone(const QString& phone)
{
    QRegularExpression phoneRegex("^[+]?[0-9\\s\\-\\(\\)]{7,20}$");
    QRegularExpressionMatch match = phoneRegex.match(phone);
    
    if (!match.hasMatch()) {
        lastError = "Invalid phone number format";
        return false;
    }
    
    lastError = "";
    return true;
}

QString InputValidator::getErrorMessage()
{
    return lastError;
}

