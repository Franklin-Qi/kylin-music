/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MINIWIDGET_H
#define MINIWIDGET_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>

#include "UIControl/player/player.h"
#include "UIControl/base/musicDataBase.h"
#include "UI/base/mylabel.h"

const QString miniMainColor = "QFrame{background:#FFFFFF;}";

#pragma pack(push)
#pragma pack(1)

class miniWidget : public QFrame
{
    Q_OBJECT
public:
    explicit miniWidget(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);

    void songText(QString songName); //mini 正在播放
protected:
    void keyPressEvent(QKeyEvent *event);
signals:
    //发送信号mini窗口我喜欢按钮改变
    void signalFavBtnChange(QString filePath);
    //刷新我喜欢列表
    void signalRefreshFav(QString listName);
private:
    void initAction();
    void initConnect();
    //初始化样式
    void initStyle();

public slots:
    void playerStateChange(playController::PlayState newState);
    void slotFavExixts();
    void slotFavExixtsDark();
    void slotFavIsExixts(QString filePath);
    void slotSongInfo(QString path);
    void slotFav();
    //接收播放区我喜欢按钮改变
    void slotFavBtnChange(QString filePath);
    //获取正在播放的title
    void slotPlayingLab(QString playing);
    //获取正在播放的时长
    void slotTimeLab(QString time);
    //设置播放模式
    void slotPlayModeClicked();
    //设置图标
    void setPlayMode(int playModel);
    //没有歌曲在播放
    void slotNotPlaying();
public:
    QFrame *m_mainFrame;

    QLabel *m_coverLabel;
    QPushButton *m_loveBtn;
    QPushButton *m_volBtn;
    QPushButton *m_orderBtn;

    QPushButton *m_closeBtn;
    QPushButton *m_recoveryWinBtn;

    QPushButton *m_preBtn;
    QPushButton *m_playStateBtn;
    QPushButton *m_nextBtn;

    MyLabel *m_songNameLab;
    QLabel *m_timeLab;

    void minicolor();

    QMenu *playModeMenu;
    QAction *playMode_Sequential_Action;
    QAction *playMode_Loop_Action;
    QAction *playMode_CurrentItemInLoop_Action;
    QAction *playMode_Random_Action;

private:
    void init_miniWidget();
    bool m_mouseState;

    QPoint  m_WindowPos;
    QPoint m_MousePos;

    QHBoxLayout *m_hMainLayout;
    QHBoxLayout *m_hPlayLayout;
    QHBoxLayout *m_hLayout;

    QFrame *m_palyFrame;
    QWidget *coverWid;

    QHBoxLayout *m_HMainLayout;

    QVBoxLayout *m_vInfoLayout;
    QVBoxLayout *m_vSysLayout;
    QString filePath;
};

#pragma pack(pop)

#endif // MINIWIDGET_H
