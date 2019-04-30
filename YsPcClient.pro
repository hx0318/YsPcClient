#-------------------------------------------------
#
# Project created by QtCreator 2018-08-27T09:34:26
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YsPcClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        logindialog.cpp \
    global.cpp \
    namanager.cpp \
    teacherdialog.cpp \
    noticedialog.cpp \
    classroomdialog.cpp \
    studentwidget.cpp \
    mediawidget.cpp \
    graffitiwidget.cpp \
    upload2ui.cpp \
    giftwidget.cpp \
    barragewidget.cpp \
    screenshotdialog.cpp \
    screenshotwidget.cpp \
    coursewarewidget.cpp \
    recorddialog.cpp \
    coursewidget.cpp \
    picturewidget.cpp \
    downloadappdialog.cpp \
    mycoursewidget.cpp \
    pagingwidget.cpp \
    courseinfodialog.cpp \
    coursewareoutwidget.cpp \
    messagewidget.cpp \
    gregoriancalendarwidget.cpp \
    recordwidget.cpp \
    recordplaydialog.cpp \
    updownwidget.cpp \
    updowndisposewidget.cpp \
    foronelogin.cpp \
    slidetextwidget.cpp \
    lookwidget.cpp

HEADERS += \
        logindialog.h \
    global.h \
    namanager.h \
    teacherdialog.h \
    noticedialog.h \
    classroomdialog.h \
    studentwidget.h \
    mediawidget.h \
    graffitiwidget.h \
    upload2ui.h \
    giftwidget.h \
    barragewidget.h \
    screenshotdialog.h \
    screenshotwidget.h \
    coursewarewidget.h \
    recorddialog.h \
    coursewidget.h \
    picturewidget.h \
    downloadappdialog.h \
    mycoursewidget.h \
    pagingwidget.h \
    courseinfodialog.h \
    coursewareoutwidget.h \
    messagewidget.h \
    gregoriancalendarwidget.h \
    recordwidget.h \
    recordplaydialog.h \
    updownwidget.h \
    updowndisposewidget.h \
    foronelogin.h \
    slidetextwidget.h \
    lookwidget.h

FORMS += \
        logindialog.ui \
    teacherdialog.ui \
    noticedialog.ui \
    classroomdialog.ui \
    studentwidget.ui \
    mediawidget.ui \
    graffitiwidget.ui \
    giftwidget.ui \
    barragewidget.ui \
    screenshotdialog.ui \
    screenshotwidget.ui \
    coursewarewidget.ui \
    recorddialog.ui \
    coursewidget.ui \
    downloadappdialog.ui \
    mycoursewidget.ui \
    pagingwidget.ui \
    courseinfodialog.ui \
    coursewareoutwidget.ui \
    messagewidget.ui \
    gregoriancalendarwidget.ui \
    recordwidget.ui \
    recordplaydialog.ui \
    updownwidget.ui \
    updowndisposewidget.ui \
    slidetextwidget.ui \
    lookwidget.ui

win32-msvc* {
    QMAKE_CFLAGS_RELEASE += -zi
    QMAKE_LFLAGS_RELEASE += /INCREMENTAL:NO /DEBUG
}

INCLUDEPATH += 3rdparty/media_device_test/include
INCLUDEPATH += 3rdparty/pdu/include
INCLUDEPATH += 3rdparty/msg/include
INCLUDEPATH += 3rdparty/voice_sms/include
INCLUDEPATH += 3rdparty/file_trans/include
INCLUDEPATH += 3rdparty/courseware/include
INCLUDEPATH += 3rdparty/libfc_client/include
INCLUDEPATH += 3rdparty/center_media/include
INCLUDEPATH += 3rdparty/media_play/include

LIBS += -L$$PWD/3rdparty/pdu/lib/ -llibpdu
LIBS += -L$$PWD/3rdparty/msg/lib/ -llibmsg
LIBS += -L$$PWD/3rdparty/voice_sms/lib/ -lvoice_sms
LIBS += -L$$PWD/3rdparty/courseware/lib/ -llibcourseware
LIBS += -L$$PWD/3rdparty/file_trans/lib/ -llibfile_transfer
LIBS += -L$$PWD/3rdparty/libfc_client/lib/ -llibfc_client
LIBS += -L$$PWD/3rdparty/media_device_test/lib/ -lmedia_device_test
LIBS += -L$$PWD/3rdparty/center_media/lib/ -llibcenter_media
LIBS += -L$$PWD/3rdparty/media_play/lib/ -lmedia_player

RESOURCES += \
    yspcclient.qrc

DISTFILES += \
    YsPcClient.rc

RC_FILE += \
    YsPcClient.rc
