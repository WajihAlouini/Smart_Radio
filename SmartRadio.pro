QT += core gui serialport sql printsupport charts network multimedia multimediawidgets widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 c++14
DEFINES += QT_DEPRECATED_WARNINGS

# Project Structure Paths
INCLUDEPATH += core \
               core/auth \
               core/dashboard \
               core/database \
               modules/Staff \
               modules/Broadcast \
               modules/Sponsorship \
               modules/Guest \
               utils \
               utils/studio \
               utils/ui \
               utils/system \
               utils/network

SOURCES += \
    core/main.cpp \
    core/database/connection.cpp \
    core/dashboard/Dashboard.cpp \
    core/auth/Login.cpp \
    core/auth/Signup.cpp \
    core/auth/ProfileDialog.cpp \
    core/auth/PasswordRecoveryDialog.cpp \
    modules/Staff/Staff.cpp \
    modules/Staff/StaffModule.cpp \
    modules/Broadcast/Broadcast.cpp \
    modules/Broadcast/BroadcastModule.cpp \
    modules/Sponsorship/Sponsor.cpp \
    modules/Sponsorship/SponsorshipModule.cpp \
    modules/Guest/Guest.cpp \
    modules/Guest/GuestModule.cpp \
    utils/studio/Arduino.cpp \
    utils/studio/StudioNotes.cpp \
    utils/studio/BroadcastHistory.cpp \
    utils/studio/BroadcastStats.cpp \
    utils/ui/notifications.cpp \
    utils/ui/StudioNotification.cpp \
    utils/system/ConfigManager.cpp \
    utils/system/Logger.cpp \
    utils/system/InputValidator.cpp \
    utils/network/SmtpClient.cpp \
    utils/qcustomplot.cpp

HEADERS += \
    core/database/connection.h \
    core/dashboard/Dashboard.h \
    core/auth/Login.h \
    core/auth/Signup.h \
    core/auth/ProfileDialog.h \
    core/auth/PasswordRecoveryDialog.h \
    modules/Staff/Staff.h \
    modules/Staff/StaffModule.h \
    modules/Broadcast/Broadcast.h \
    modules/Broadcast/BroadcastModule.h \
    modules/Sponsorship/Sponsor.h \
    modules/Sponsorship/SponsorshipModule.h \
    modules/Guest/Guest.h \
    modules/Guest/GuestModule.h \
    utils/studio/Arduino.h \
    utils/studio/StudioNotes.h \
    utils/studio/BroadcastHistory.h \
    utils/studio/BroadcastStats.h \
    utils/ui/notifications.h \
    utils/ui/ThemeManager.h \
    utils/ui/StudioNotification.h \
    utils/system/ConfigManager.h \
    utils/system/Logger.h \
    utils/system/InputValidator.h \
    utils/system/ReportGenerator.h \
    utils/network/SmtpClient.h \
    utils/qcustomplot.h

FORMS += \
    core/dashboard/Dashboard.ui \
    core/auth/Login.ui \
    core/auth/Signup.ui \
    modules/Staff/StaffModule.ui \
    modules/Broadcast/BroadcastModule.ui \
    modules/Sponsorship/SponsorshipModule.ui \
    modules/Guest/GuestModule.ui \
    utils/studio/StudioNotes.ui \
    utils/studio/BroadcastStats.ui

RESOURCES += resources.qrc

RC_ICONS = resources/icons/AppIcon.ico
DISTFILES += resources/icons/AppIcon.png
