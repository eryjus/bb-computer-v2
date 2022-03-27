QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bus.cpp \
    clock-module.cpp \
    control-module.cpp \
    main.cpp \
    bbcomputer.cpp \
    program-counter-module.cpp \
    register.cpp \
    temp-reg-1.cpp \
    temp-reg-2.cpp \
    timer.cpp

HEADERS += \
    bbcomputer.h \
    bus.h \
    clock-module.h \
    control-module.h \
    program-counter-module.h \
    register.h \
    temp-reg-1.h \
    temp-reg-2.h \
    timer.h

FORMS += \
    bbcomputer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    RoundLed.pro

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib64/release/ -lKF5WidgetsAddons
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib64/debug/ -lKF5WidgetsAddons
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib64/ -lKF5WidgetsAddons

INCLUDEPATH += $$PWD/../../../../../../usr/include/KF5/KWidgetsAddons
DEPENDPATH += $$PWD/../../../../../../usr/include/KF5/KWidgetsAddons
