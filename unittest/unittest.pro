QT += core

TARGET = QsLogUnitTest
CONFIG += console qtestlib
CONFIG -= app_bundle
TEMPLATE = app

# test-case sources
SOURCES += TestLog.cpp

# component sources
include(../QsLog.pri)

SOURCES += \
    ./QtTestUtil/TestRegistry.cpp \
    ./QtTestUtil/SimpleChecker.cpp

HEADERS += \
    ./QtTestUtil/TestRegistry.h \
    ./QtTestUtil/TestRegistration.h \
    ./QtTestUtil/QtTestUtil.h
