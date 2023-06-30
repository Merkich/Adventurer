QT += core gui
QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src/

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        src/converterjson.cpp \
        src/invertedindex.cpp \
        src/searchserver.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    mainwindow.h \
    src/converterjson.h \
    src/exceptions.h \
    src/invertedindex.h \
    src/searchserver.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    config/answers.json \
    config/config.json \
    config/requests.json
