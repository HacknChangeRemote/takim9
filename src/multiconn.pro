QT       += core gui qml quick quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    huiobject.cpp \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    huiobject.h

RESOURCES += \
    resources.qrc

include($$PWD/../common/common.pri)

include($$(HARF_DIR)/lib/include/path.pri)
include($$(HARF_DIR)/lib/include/platform.pri)
include($$(HARF_DIR)/lib/include/harfcore.pri)
include($$(HARF_DIR)/lib/include/arengine.pri)
include($$(HARF_DIR)/lib/include/harfprotobuf.pri)
include($$(HARF_DIR)/lib/include/cpprestsdk.pri)
include($$(HARF_DIR)/lib/include/harfutils.pri)
include($$(HARF_DIR)/lib/include/stream.pri)
include($$(HARF_DIR)/lib/include/harfconnection.pri)
include($$(HARF_DIR)/lib/include/cpprestsdk.pri)
include($$(HARF_DIR)/lib/include/boost.pri)
include($$(HARF_DIR)/lib/include/enet.pri)
include($$(HARF_DIR)/lib/include/pjproject.pri)
include($$(HARF_DIR)/lib/include/openssl.pri)
include($$(HARF_DIR)/lib/include/breakpad.pri)

android {
    DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew \
        android/gradlew.bat \
        android/res/values/libs.xml
        android/libs/harfandroid-release.aar \

    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
