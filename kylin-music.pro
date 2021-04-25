QT       += core gui sql widgets
QT       += multimedia multimediawidgets
QT       += dbus x11extras KWindowSystem

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

target.path = /usr/bin
target.source += $$TARGET

icon.path = /usr/share/pixmaps
icon.files = img/kylin-music.png

desktop.path = /usr/share/applications/
desktop.files = kylin-music.desktop

schemes.files = data/org.kylin-music-data.gschema.xml
schemes.path = /usr/share/glib-2.0/schemas/

INSTALLS += \
    target  \
    icon    \
    desktop \
    schemes

TRANSLATIONS += ./translations/kylin-music_side.ts

CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt taglib
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += qtsingleapplication
DEPENDPATH += qtsingleapplication
# 适配窗口管理器圆角阴影
LIBS +=-lpthread
LIBS +=-lX11

SOURCES += \
    UI/base/allpupwindow.cpp \
    UI/base/mylabel.cpp \
    UI/base/widgetstyle.cpp \
    UI/mainwidget.cpp \
    UI/player/miniwidget.cpp \
    UI/player/musicslider.cpp \
    UI/player/playbackmodewidget.cpp \
    UI/player/playsongarea.cpp \
    UI/player/sliderwidget.cpp \
    UI/base/xatom-helper.cpp \
    UI/sidebar/mytoolbutton.cpp \
    UI/sidebar/sidebarwidget.cpp \
    UI/tableview/tablebaseview.cpp \
    UI/tableview/tablehistory.cpp \
    UI/tableview/tableone.cpp \
    UI/tableview/tableviewdelegate.cpp \
    UI/titlebar/menumodule.cpp \
    UI/titlebar/titlebar.cpp \
    UIControl/base/daemonipcdbus.cpp \
    UIControl/base/musicDataBase.cpp \
    UIControl/base/musicfileinformation.cpp \
    UIControl/player/player.cpp \
    UIControl/tableview/musiclistmodel.cpp \
    main.cpp



HEADERS += \
    UI/base/allpupwindow.h \
    UI/base/mylabel.h \
    UI/base/widgetstyle.h \
    UI/mainwidget.h \
    UI/player/miniwidget.h \
    UI/player/musicslider.h \
    UI/player/playbackmodewidget.h \
    UI/player/playsongarea.h \
    UI/player/sliderwidget.h \
    UI/base/xatom-helper.h \
    UI/sidebar/mytoolbutton.h \
    UI/sidebar/sidebarwidget.h \
    UI/tableview/tablebaseview.h \
    UI/tableview/tablehistory.h \
    UI/tableview/tableone.h \
    UI/tableview/tableviewdelegate.h \
    UI/titlebar/menumodule.h \
    UI/titlebar/titlebar.h \
    UIControl/base/daemonipcdbus.h \
    UIControl/base/musicDataBase.h \
    UIControl/base/musicfileinformation.h \
    UIControl/player/player.h \
    UIControl/tableview/musiclistmodel.h \


# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    data/org.kylin-music-data.gschema.xml \
    kylin-music.desktop
