#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T19:13:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plan1
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h \
    RollingGuidanceFilter.h

FORMS    += widget.ui

INCLUDEPATH+=F:\final\include\opencv\
                    F:\final\include\opencv2\
                    F:\final\include
LIBS+=F:\final\lib\libopencv_calib3d249.dll.a\
        F:\final\lib\libopencv_contrib249.dll.a\
        F:\final\lib\libopencv_core249.dll.a\
        F:\final\lib\libopencv_features2d249.dll.a\
        F:\final\lib\libopencv_flann249.dll.a\
        F:\final\lib\libopencv_gpu249.dll.a\
        F:\final\lib\libopencv_highgui249.dll.a\
        F:\final\lib\libopencv_imgproc249.dll.a\
        F:\final\lib\libopencv_legacy249.dll.a\
        F:\final\lib\libopencv_ml249.dll.a\
        F:\final\lib\libopencv_objdetect249.dll.a\
        F:\final\lib\libopencv_video249.dll.a

RC_FILE+=myico.rc
