#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QString>
#include <QValidator>

class InputValidator
{
public:
    // Validate ID (positive integer)
    static bool validateId(const QString& id, int min = 1, int max = 99999999);
    
    // Validate name (non-empty, reasonable length)
    static bool validateName(const QString& name, int minLength = 1, int maxLength = 100);
    
    // Validate time format (HH:MM or HH:MM:SS)
    static bool validateTime(const QString& time);
    
    // Validate salary/amount (positive number)
    static bool validateAmount(const QString& amount, double min = 0, double max = 9999999);
    
    // Validate duration (positive integer, reasonable range)
    static bool validateDuration(const QString& duration, int min = 1, int max = 1440);
    
    // Validate email
    static bool validateEmail(const QString& email);
    
    // Validate phone number
    static bool validatePhone(const QString& phone);
    
    // Get validation error message
    static QString getErrorMessage();
    
private:
    static QString lastError;
};

#endif // INPUTVALIDATOR_H

