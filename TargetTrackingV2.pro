#-------------------------------------------------
#
# Project created by QtCreator 2018-03-26T13:28:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TargetTrackingV2
TEMPLATE = app


SOURCES += main.cpp\
        mainui.cpp \
    globalsettings.cpp \
    serialPortService/dataparser4firectrl.cpp \
    serialPortService/dataparser4payload.cpp \
    serialPortService/dataparserbase.cpp \
    serialPortService/serialportboost.cpp \
    serialPortService/serviceserialfirectrl.cpp \
    serialPortService/serviceserialpayload.cpp \
    udpService/managerofcommunicaion.cpp \
    udpService/serviceudpvideo.cpp \
    udpService/udpsendservice.cpp \
    videoProcService/servicevideoproc.cpp \
    videoProcService/trackalgorithm.cpp \
    videoProcService/videoencodeh264.cpp \
    videoProcService/videoprocess.cpp \
    comutils.cpp \
    serialcomsetting.cpp

HEADERS  += mainui.h \
    globalsettings.h \
    algorithm/include/calibrateinformationmeasure.h \
    algorithm/include/calibrateinformationmeasure_global.h \
    algorithm/include/cvfeatureextractmatch.h \
    algorithm/include/cvfeatureextractmatch_global.h \
    algorithm/include/ffttools.hpp \
    algorithm/include/fhog.hpp \
    algorithm/include/kcftracker.h \
    algorithm/include/kcftracker_global.h \
    algorithm/include/labdata.hpp \
    algorithm/include/recti.h \
    algorithm/include/recti_global.h \
    algorithm/include/recttools.hpp \
    algorithm/include/tracker.h \
    serialPortService/dataparser4firectrl.h \
    serialPortService/dataparser4payload.h \
    serialPortService/dataparserbase.h \
    serialPortService/serialportboost.h \
    serialPortService/serviceserialfirectrl.h \
    serialPortService/serviceserialpayload.h \
    udpService/managerofcommunicaion.h \
    udpService/serviceudpvideo.h \
    udpService/udpsendservice.h \
    videoProcService/servicevideoproc.h \
    videoProcService/trackalgorithm.h \
    videoProcService/videoencodeh264.h \
    videoProcService/videoprocess.h \
    comutils.h \
    serialcomsetting.h

FORMS    += mainui.ui
