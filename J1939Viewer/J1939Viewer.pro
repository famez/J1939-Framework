#-------------------------------------------------
#
# Project created by QtCreator 2017-11-19T21:51:29
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = J1939Viewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    framemodel.cpp \
    treeitem.cpp \
    frameitem.cpp \
    spnitem.cpp \
    frametreeview.cpp \
    spndialog.cpp \
    framedialog.cpp \
    genericframeitem.cpp \
    numericspnitem.cpp \
    statusspnitem.cpp \
    trcinspector.cpp \
    trcloader.cpp \
    spnchart.cpp \
    spnloader.cpp \
    colouredrangesarea.cpp \
    customdelegate.cpp \
    customeditor.cpp \
    spnstatuseditor.cpp \
    form.cpp \
    spnstatusdialog.cpp

HEADERS += \
        mainwindow.h \
    framemodel.h \
    treeitem.h \
    frameitem.h \
    spnitem.h \
    frametreeview.h \
    spndialog.h \
    framedialog.h \
    genericframeitem.h \
    numericspnitem.h \
    statusspnitem.h \
    trcinspector.h \
    trcloader.h \
    spnchart.h \
    spnloader.h \
    colouredrangesarea.h \
    customdelegate.h \
    common.h \
    customeditor.h \
    spnstatuseditor.h \
    form.h \
    spnstatusdialog.h

FORMS += \
        mainwindow.ui \
    spndialog.ui \
    framedialog.ui \
    trcinspector.ui \
    spnstatuseditor.ui \
    spnstatusdialog.ui

INCLUDEPATH += $$PWD/../Common
DEPENDPATH += $$PWD/../Common

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-CAN-Desktop_Qt_5_9_2_MinGW_32bit-Debug/release/ -lCAN
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-CAN-Desktop_Qt_5_9_2_MinGW_32bit-Debug/debug/ -lCAN
else:unix: LIBS += -L$$PWD/../build-CAN-Desktop_Qt_5_9_2_MinGW_32bit-Debug/ -lCAN

INCLUDEPATH += $$PWD/../CAN
DEPENDPATH += $$PWD/../CAN

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-CAN-Desktop_Qt_5_9_2_MinGW_32bit-Debug/release/libCAN.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-CAN-Desktop_Qt_5_9_2_MinGW_32bit-Debug/debug/libCAN.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-CAN-Desktop_Qt_5_9_2_MinGW_32bit-Debug/release/CAN.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-CAN-Desktop_Qt_5_9_2_MinGW_32bit-Debug/debug/CAN.lib
else:unix: PRE_TARGETDEPS += $$PWD/../build-CAN-Desktop_Qt_5_9_2_MinGW_32bit-Debug/libCAN.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-J1939-Desktop_Qt_5_9_2_MinGW_32bit-Debug/release/ -lJ1939
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-J1939-Desktop_Qt_5_9_2_MinGW_32bit-Debug/debug/ -lJ1939
else:unix: LIBS += -L$$PWD/../build-J1939-Desktop_Qt_5_9_2_MinGW_32bit-Debug/ -lJ1939

INCLUDEPATH += $$PWD/../J1939
DEPENDPATH += $$PWD/../J1939

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-J1939-Desktop_Qt_5_9_2_MinGW_32bit-Debug/release/libJ1939.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-J1939-Desktop_Qt_5_9_2_MinGW_32bit-Debug/debug/libJ1939.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-J1939-Desktop_Qt_5_9_2_MinGW_32bit-Debug/release/J1939.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-J1939-Desktop_Qt_5_9_2_MinGW_32bit-Debug/debug/J1939.lib
else:unix: PRE_TARGETDEPS += $$PWD/../build-J1939-Desktop_Qt_5_9_2_MinGW_32bit-Debug/libJ1939.a

RESOURCES += \
    icons.qrc
