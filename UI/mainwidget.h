﻿#ifndef WIDGET_H
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
#include <QProcess>

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
#include "UIControl/base/daemonipcdbus.h"

class Widget : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ukui.kylin_music.play")
public:
    Widget(QStringList str, QWidget *parent = nullptr);
    ~Widget();
    //计算播放历史
    void movePlayHistoryWid();
public:
    static Widget *mutual;          //指针类型静态成员变量
    QProcess *process;
public Q_SLOTS:
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
    //获取当前播放歌曲的名称
    void slotPlayingTitle(QString title);
    //当前播放歌曲的名称
    QString getTitle();
    //获取当前播放歌曲的状态
    QString getState();
    //命令参数
    int kylin_music_play_request(QString cmd1, QString cmd2 = "", QString cmd3 = "");
    //判断当前点击的按钮是否为歌曲列表（歌单名）
    void slotText(QString btnText);
    //
    void slotReturnPressed();
    //构造完成之后的流程事件函数
    void creartFinish();
    //构造完成后需要弹窗
    void setCreatFinishMsg(QString msg);
private Q_SLOTS:
    void onPrepareForShutdown(bool Shutdown);
    void onPrepareForSleep(bool isSleep);
    void client_get(QString str);
    void inputDevice_get(QString str);
    void slotPrepareForSwitchuser();
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
//    void mousePressEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
Q_SIGNALS:
    //空格键控制播放暂停
    void signalSpaceKey();
private:
    //初始化dbus
    void initDbus();
    void initStyle();
    //单例
    void Single(QStringList path);
    //处理双击文件打开时间
    void stateMusicFile(QStringList args);
    void initAllComponent();
    void allConnect();
    void initGSettings();
    void moveWidget(QString newWidth, QString newHeight);
    void changeDarkTheme();
    void changeLightTheme();
    void importFile(QStringList list);
    QStringList getPath(QString playListName);
    //初始化音乐
    void initMusic();
private:
    QVBoxLayout *mainVBoxLayout = nullptr;
    QHBoxLayout *mainHBoxLayout = nullptr;
    TableOne *musicListTable = nullptr;
    TableHistory *historyListTable = nullptr;
    QGSettings *themeData = nullptr;
    SideBarWidget *sideBarWid = nullptr;
    PlaySongArea *playSongArea = nullptr;
    TitleBar *m_titleBar = nullptr;
    miniWidget *m_miniWidget = nullptr;
    QWidget *rightVWidget = nullptr;

    bool Minimize = false;       //最大化和还原俩个状态

    //判断是否为第一个实例
    bool isFirstObject = false;
    QStringList argName;
    //歌单名
    QString listName;
    //正在播放歌曲的title
    QString m_playTitle;
    //避免初始化流程中触发点击事件
    bool m_initFinish = false;
    //主界面初始化完成后执行的操作
    enum CreatFinishEnum{NONE=0,MESSAGE,OTHER} m_creatFinishEnum = NONE;
    QString m_creatFinishMsg;

Q_SIGNALS:
    void signalShowGuide();
    //刷新歌曲列表界面
    void signalRefreshList(QString listName);
    //字体
    void signalSetFontSize(int);
};
#endif // WIDGET_H
