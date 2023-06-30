include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt

SOURCES += \
        main.cpp  \
        ../search_engine/src/invertedindex.cpp \
        ../search_engine/src/searchserver.cpp \
        tst_tests.cpp

HEADERS += ../search_engine/src/invertedindex.h \
           ../search_engine/src/searchserver.h
