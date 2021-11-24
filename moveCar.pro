QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Vibe.cpp \
    drawimage.cpp \
    main.cpp \
    mainwindow.cpp \
    tof.cpp

HEADERS += \
    Vibe.h \
    dmcam.h \
    drawimage.h \
    mainwindow.h \
    tof.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += D:\Programs\Qt\QtFile\smartTof\lib\libdmcam.dll
INCLUDEPATH += D:\Programs\opencv450QtMinGW\build\install\include
LIBS += D:\Programs\opencv450QtMinGW\build\bin\libopencv_world450.dll

RESOURCES += \
    Resource/img.qrc
