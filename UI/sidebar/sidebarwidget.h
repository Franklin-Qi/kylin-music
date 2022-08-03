#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <UI/base/popupDialog.h>
#include <QPalette>
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QStyleOption>
#include <QGSettings>

#include "customSiderBarScrollArea.h"
#include "customToolButton.h"
#include "leftsiderbarwidget.h"


class SideBarWidget : public LeftsiderbarWidget
{
    Q_OBJECT

public:
    explicit SideBarWidget(QWidget *parent = nullptr);

    PopupDialog *newSonglistPup = nullptr; //新建歌单弹窗界面
    PopupDialog *renameSongListPup = nullptr; //重命名歌单弹窗界面
    CustomToolButton *playListBtn = nullptr;

    void initAddButton();
    void sidecolor();
    void initWidget();
    void initConnect();
    void getPlayListName();
    QString newPlayListName();

public Q_SLOTS:
    void addPlayList();
    void addItemToSongList();
    void playAll(QString btnText);
    void rename(QString btnText);
    void renamePlayList();
    void removePlayList(QString text);
    void slotListSearch();
    void slotSongListHigh();

Q_SIGNALS:
    void playListBtnClicked(QString listname);
    void playListRemoved(QString listname);
    void playListRenamed(QString oldName,QString newName);
    void playListAdded(QString listName);

    void signalPlayAll(QString listName);

private:
    QGSettings *m_transparencyGSettings = nullptr; // 控制面板透明度
    double m_transparency = 1.0;  // 透明度

    QPushButton *myPlayListBtn = nullptr; //添加歌单按钮

    CustomSiderBarScrollArea *scrollArea; //新建歌单按钮界面(包含我喜欢在内)

    QWidget *newPlayListWidget = nullptr;
    QWidget *logoWid = nullptr;
    QWidget *mainWid = nullptr;
//    QWidget *sideWid = nullptr;
    QVBoxLayout *newPlayListLayout = nullptr;
    QStringList playListName; //歌单名
    QLabel *libraryLabel = nullptr; //音乐库label
    QLabel *myPlayListLabel = nullptr; //我的歌单label
    QString btnText; //获取歌单名
    CustomToolButton *m_lastBtn = nullptr; //保存程序关闭之前歌单按钮
};

#endif // SIDEBARWIDGET_H
