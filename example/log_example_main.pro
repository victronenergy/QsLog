#This project links with QsLog dynamically and outputs an executable file.

QT -= gui
TARGET = log_example
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += log_example_main.cpp
INCLUDEPATH += $$PWD/../
DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT

LIBS += -L$$PWD/../build-QsLogShared
win32 {
    LIBS += -lQsLog2
} else {
    LIBS += -lQsLog
}
LIBS += -L$$PWD/../build-QsLogExample -llog_example_shared

DESTDIR = $$PWD/../build-QsLogExample
OBJECTS_DIR = $$DESTDIR/obj
