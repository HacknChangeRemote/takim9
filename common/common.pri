QT += core gui qml quick quickwidgets opengl

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/androidhelper.h \
    $$PWD/hqmlviewer.h \
    $$PWD/huseriobridge.h \
    $$PWD/fboinsgrenderer.h \
    $$PWD/logorenderer.h \
    $$PWD/hviewer.h \
    $$PWD/hopenglwidget.h \
    $$PWD/huiwidget.h \

SOURCES += \
    $$PWD/androidhelper.cpp \
    $$PWD/hqmlviewer.cpp \
    $$PWD/huseriobridge.cpp \
    $$PWD/fboinsgrenderer.cpp \
    $$PWD/logorenderer.cpp \
    $$PWD/hopenglwidget.cpp \
    $$PWD/huiwidget.cpp \
    $$PWD/hviewer.cpp \

android {
    QT += androidextras
}
