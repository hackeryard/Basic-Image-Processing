#-------------------------------------------------
#
# Project created by QtCreator 2016-11-16T09:29:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Thinkcv
TEMPLATE = app

INCLUDEPATH+=C:\OpenCV\11\include\opencv\
                    C:\OpenCV\11\include\opencv2\
                    C:\OpenCV\11\include

LIBS+=C:\OpenCV\11\lib\libopencv_calib3d2413.dll.a\
        C:\OpenCV\11\lib\libopencv_contrib2413.dll.a\
        C:\OpenCV\11\lib\libopencv_core2413.dll.a\
        C:\OpenCV\11\lib\libopencv_features2d2413.dll.a\
        C:\OpenCV\11\lib\libopencv_flann2413.dll.a\
        C:\OpenCV\11\lib\libopencv_gpu2413.dll.a\
        C:\OpenCV\11\lib\libopencv_highgui2413.dll.a\
        C:\OpenCV\11\lib\libopencv_imgproc2413.dll.a\
        C:\OpenCV\11\lib\libopencv_legacy2413.dll.a\
        C:\OpenCV\11\lib\libopencv_ml2413.dll.a\
        C:\OpenCV\11\lib\libopencv_objdetect2413.dll.a\
        C:\OpenCV\11\lib\libopencv_video2413.dll.a


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    pick.h

RC_FILE = hackico.rc

FORMS    += \
    mainwindow.ui

RESOURCES += \
    hack.qrc
