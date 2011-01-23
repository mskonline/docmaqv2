# -------------------------------------------------
# Project created by QtCreator 2010-04-08T19:32:39
# -------------------------------------------------
QT += sql
TARGET = DocmaQ_Configure
CONFIG += debug
SOURCES += main.cpp \
    config.cpp \
    getconfig.cpp
HEADERS += config.h \
    ui_config.h \
    getconfig.h
win32:RC_FILE = Resources/resource.rc
RESOURCES += Resources/res.qrc
FORMS += config.ui
