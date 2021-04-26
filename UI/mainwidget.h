#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>
#include <QResizeEvent>
#include <QPropertyAnimation>
//DBus
#include <QDBusInterface>
#include <QDBusConnection>
//窗口显示在屏幕中心
#include <QApplication>
#include <QScreen>
//单例需要的头文件
#include <fcntl.h>
//窗体拉起
#include <KWindowSystem>

#include "UIControl/base/musicDataBase.h"
#include "UI/tableview/tableone.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UI/tableview/tablebaseview.h"
#include "UI/base/widgetstyle.h"
#include "UI/sidebar/sidebarwidget.h"
#include "UI/player/playsongarea.h"
#include "UI/tableview/tablehistory.h"
#include "UI/titlebar/titlebar.h"
#include "UI/player/miniwidget.h"

class Widget : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ukui.kylin_music.play")
public:
    Widget(QStringList str, QWidget *parent = nullptr);
    ~Widget();
public:
    static Widget *mutual;          //指针类型静态成员变量
public slots:
    //mini窗口
    void slotShowMiniWidget();
    //关闭主窗体
    void slotClose();
    //最小化
    void slotShowMinimized();
    //最大化
    void slotShowMaximized();
    //mini
    void slotRecoverNormalWidget();
    //mini 关闭窗体
    void slotCloseMiniWidget();
    //命令参数
    int kylin_music_play_request(QString cmd1, QString cmd2 = "", QString cmd3 = "");
private slots:
protected:
    void resizeEvent(QResizeEvent *event)override;
//    void mousePressEvent(QResizeEvent *event)override;
private:
    //初始化dbus
    void initDbus();
    //单例
    void Single(QStringList path);

    void initAllComponent();
    void allConnect();
    void initGSettings();

    void changeDarkTheme();
    void changeLightTheme();
private:
    QVBoxLayout *mainVBoxLayout;
    QHBoxLayout *mainHBoxLayout;
    TableOne *musicListTable;
    TableHistory *historyListTable;
    QGSettings *themeData = nullptr;
    SideBarWidget *sideBarWid;
    PlaySongArea *playSongArea;
    TitleBar *m_titleBar;
    miniWidget *m_miniWidget;

    bool Minimize = false;       //最大化和还原俩个状态
};
#endif // WIDGET_H
