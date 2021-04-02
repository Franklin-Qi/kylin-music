QT       += core gui sql
QT       += multimedia multimediawidgets
QT       += dbus x11extras KWindowSystem

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

schemes.files = data/org.kylin-music-data.gschema.xml
schemes.path = /usr/share/glib-2.0/schemas/

TRANSLATIONS += ./translations/kylin-music_side.ts

CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UI/base/allpupwindow.cpp \
    UI/base/widgetstyle.cpp \
    UI/player/musicslider.cpp \
    UI/player/playbackmodewidget.cpp \
    UI/player/playsongarea.cpp \
    UI/player/sliderwidget.cpp \
#    UI/player/xatom-helper.cpp \
    UI/sidebar/mytoolbutton.cpp \
    UI/sidebar/sidebarwidget.cpp \
    UI/tableview/tablebaseview.cpp \
    UI/tableview/tableone.cpp \
    UI/tableview/tableviewdelegate.cpp \
    UIControl/base/musicDataBase.cpp \
    UIControl/player/player.cpp \
    UIControl/tableview/scoreinfomodel.cpp \
    daemonipcdbus.cpp \
    main.cpp \
    mainwidget.cpp



HEADERS += \
    UI/base/allpupwindow.h \
    UI/base/widgetstyle.h \
    UI/player/musicslider.h \
    UI/player/playbackmodewidget.h \
    UI/player/playsongarea.h \
    UI/player/sliderwidget.h \
#    UI/player/xatom-helper.h \
    UI/sidebar/mytoolbutton.h \
    UI/sidebar/sidebarwidget.h \
    UI/tableview/tablebaseview.h \
    UI/tableview/tableone.h \
    UI/tableview/tableviewdelegate.h \
    UIControl/base/musicDataBase.h \
    UIControl/player/player.h \
    UIControl/tableview/scoreinfomodel.h \
    daemonipcdbus.h \
    mainwidget.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    data/org.kylin-music-data.gschema.xml
