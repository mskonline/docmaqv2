# -------------------------------------------------
# Project created by QtCreator 2010-03-15T23:21:44
# -------------------------------------------------
QT += sql
TARGET = DocmaQ
TEMPLATE = app
CONFIG += console \
    uitools
SOURCES += main.cpp \
    App_Manager/appmanager.cpp \
    Database_Manager/cdatabase.cpp \
    Interface/lineitem.cpp \
    Interface/interface.cpp \
    Interface/courseitem.cpp \
    Interface/comboitem.cpp \
    Interface/certificatescene.cpp \
    Interface/dateitem.cpp \
    Print_Manager/cprinter.cpp \
    Settings_Manager/settings.cpp \
    Settings_Manager/getsettings.cpp \
    Authentication_Manager/authendialog.cpp \
    Log_Manager/log.cpp \
    Log_Manager/logmanager.cpp \
    Interface/btable/bulktable.cpp
HEADERS += App_Manager/appmanager.h \
    Interface/interface.h \
    Data_Structures/student.h \
    Database_Manager/cdatabase.h \
    Interface/lineitem.h \
    Interface/effectspix.h \
    Interface/courseitem.h \
    Interface/comboitem.h \
    Interface/certificatescene.h \
    Interface/dateitem.h \
    Print_Manager/cprinter.h \
    btable/bulktable.h \
    Interface/rolledit.h \
    Settings_Manager/settings.h \
    Settings_Manager/getsettings.h \
    Authentication_Manager/authendialog.h \
    Log_Manager/log.h \
    Log_Manager/logmanager.h \
    Interface/btable/bulktable.h
win32:UI_DIR = ui
FORMS += Interface/interface.ui \
    Interface/dialogues/print.ui \
    Authentication_Manager/authendialog.ui \
    Settings_Manager/settings.ui
RESOURCES += resources/res.qrc
win32:MOC_DIR = temp/moc
win32:OBJECTS_DIR = temp/obj