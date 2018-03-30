#-------------------------------------------------
#
# Project created by QtCreator 2018-03-26T13:28:16
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TargetTrackingV2
TEMPLATE = app


SOURCES += main.cpp\
        mainui.cpp \
    globalsettings.cpp \
    serialPortService/dataparser4firectrl.cpp \
    serialPortService/dataparser4payload.cpp \
    serialPortService/dataparserbase.cpp \
    serialPortService/serviceserialfirectrl.cpp \
    serialPortService/serviceserialpayload.cpp \
    udpService/serviceudpvideo.cpp \
    udpService/udpsendservice.cpp \
    videoProcService/servicevideoproc.cpp \
    videoProcService/trackalgorithm.cpp \
    videoProcService/videoencodeh264.cpp \
    videoProcService/videoprocess.cpp \
    comutils.cpp \
    serialcomsetting.cpp \
    managerofcommunicaion.cpp \
    datamanager.cpp

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
    serialPortService/serviceserialfirectrl.h \
    serialPortService/serviceserialpayload.h \
    udpService/serviceudpvideo.h \
    udpService/udpsendservice.h \
    videoProcService/servicevideoproc.h \
    videoProcService/trackalgorithm.h \
    videoProcService/videoencodeh264.h \
    videoProcService/videoprocess.h \
    comutils.h \
    serialcomsetting.h \
    managerofcommunicaion.h \
    datamanager.h

FORMS    += mainui.ui
INCLUDEPATH += /usr/local/include/opencv310 /usr/local/include/ffmpeg \
                /usr/include/log4cplus
LIBS += -L/usr/local/lib/opencv310 -lopencv_core -lopencv_imgproc   -lopencv_videoio \
    -lopencv_highgui  -lopencv_video  -lopencv_features2d -lopencv_calib3d  -lopencv_imgcodecs \
    -lopencv_shape -lopencv_cudaobjdetect -lopencv_cudawarping -lopencv_cudaimgproc
LIBS += -L/usr/local/lib -lavcodec-ffmpeg -lavutil-ffmpeg
LIBS += -L../algorithm/lib -lcvFeatureExtractMatch -lKCFTracker -lCalibrateInformationMeasure
LIBS += -L/usr/lib/aarch64-linux-gnu/ -llog4cplus

OBJECTS_DIR += ./temp
MOC_DIR += ./temp
