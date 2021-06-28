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

#include "miniwidget.h"
#include "UI/base/widgetstyle.h"
#include <QDebug>

miniWidget::miniWidget(QWidget *parent) : QFrame(parent)
{
    setFixedSize(300,60);
    this->setObjectName("miniWidget");

    m_mouseState = false;
//    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);     //窗体透明
//    this->setWindowTitle(tr("音乐"));
    this->setWindowTitle(tr("Music Player"));

    setMouseTracking(true);
    init_miniWidget();

    this->setLayout(m_HMainLayout);
    minicolor();

    initAction();
    initConnect();
    initStyle();
}

void miniWidget::initAction()
{
    playModeMenu = new QMenu(tr("列表循环"),this);
    playMode_Loop_Action = new QAction(QIcon(":/img/default/listloop.png"), tr("列表循环"), playModeMenu);
    playMode_Random_Action = new QAction(QIcon(":/img/default/random.png"), tr("随机播放"), playModeMenu);
    playMode_Sequential_Action = new QAction(QIcon(":/img/default/sequence.png"), tr("顺序播放"), playModeMenu);
    playMode_CurrentItemInLoop_Action = new QAction(QIcon(":/img/default/oneloop.png"), tr("单曲循环"), playModeMenu);
    playModeMenu->addAction(playMode_Loop_Action);
    playModeMenu->addAction(playMode_Random_Action);
    playModeMenu->addAction(playMode_Sequential_Action);
    playModeMenu->addAction(playMode_CurrentItemInLoop_Action);
    playModeMenu->addMenu(playModeMenu);
}

void miniWidget::mousePressEvent(QMouseEvent *event)
{
    m_WindowPos = this->pos();
    if(QRect(0,0,width(),height()).contains(event->pos()) && event->button() == Qt::LeftButton)
    {
        m_MousePos = event->globalPos();
        m_mouseState = true;
    }
}

void miniWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mouseState)
    {
        move(m_WindowPos + (event->globalPos() - m_MousePos));
    }
}

void miniWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_mouseState = false;
}

void miniWidget::enterEvent(QEvent *)
{
    m_songNameLab->setVisible(false);
    m_timeLab->setVisible(false);
    m_palyFrame->setVisible(true);
    coverWid->setVisible(true);
    this->setFixedWidth(328);
}

void miniWidget::leaveEvent(QEvent *)
{
    m_songNameLab->setVisible(true);
    m_timeLab->setVisible(true);
    m_palyFrame->setVisible(false);
    coverWid->setVisible(false);
    this->setFixedWidth(300);

}

void miniWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        return;
    }
    else if(event->key() == Qt::Key_Right)
    {
        return;
    }
    else if(event->key() == Qt::Key_Left)
    {
        return;
    }
    QWidget::keyPressEvent(event);
}

void miniWidget::minicolor()
{
    if(WidgetStyle::themeColor == 1)
    {
        m_mainFrame->setStyleSheet("#mainFrame{border-radius:6px;background-color:#252526;}");
        m_timeLab->setStyleSheet("QLabel{line-height:12px;color:#8F9399;font-size:12px;}");
//        m_coverLabel->setPixmap(QPixmap(":/img/fengmian.png").scaled(48,48));
        m_coverLabel->setStyleSheet("border-image:url(:/img/fengmian.png);");

        m_songNameLab->setStyleSheet("QLabel{line-height:14px;color:#F9F9F9;font-size:14px;}");

        m_preBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/lastsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");

//        m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
//                                      "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
//                                      "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

        if(playController::getInstance().getState() == playController::PlayState::PLAY_STATE)
        {
            m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                          "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                          "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
        }
        else if(playController::getInstance().getState() == playController::PlayState::PAUSED_STATE)
        {
            m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                          "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                          "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        }
        else if(playController::getInstance().getState() == playController::PlayState::STOP_STATE)
        {
            m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                          "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                          "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        }

//        m_loveBtn->setIcon(QIcon(":/img/dark/icon_love2_b@2x.png"));
        slotFavExixtsDark();

        m_closeBtn->setIcon(QIcon(":/img/clicked/close.png"));

        m_closeBtn->setIconSize(QSize(30, 30));
        m_closeBtn->setStyleSheet("QPushButton{background:transparent;}");

        m_recoveryWinBtn->setIcon(QIcon(":/img/lightshow/maximize.png"));
        m_recoveryWinBtn->setIconSize(QSize(30, 30));
        m_recoveryWinBtn->setStyleSheet("QPushButton{background-color:transparent;}");

        m_nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

        coverWid->setStyleSheet("background-color:rgba(0,0,0,0.4);\
                                border-top-right-radius:6px;border-bottom-right-radius:6px;\
                                border-top-left-radius:0px;border-bottom-left-radius:0px");

    }
    else if(WidgetStyle::themeColor == 0)
    {
        m_mainFrame->setStyleSheet("#mainFrame{border-radius:6px;background-color:#FFFFFF;}");
        m_timeLab->setStyleSheet("QLabel{line-height:12px;color:#8F9399;font-size:12px;}");
        m_coverLabel->setStyleSheet("border-image:url(:/img/fengmian.png);");
//        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        m_songNameLab->setStyleSheet("QLabel{line-height:14px;color:#303133;font-size:14px;}");

        m_preBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/lastsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");
//        m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
//                                      "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
//                                      "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

        if(playController::getInstance().getState() == playController::PlayState::PLAY_STATE)
        {
            m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                          "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                          "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
        }
        else if(playController::getInstance().getState() == playController::PlayState::PAUSED_STATE)
        {
            m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                          "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                          "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        }
        else if(playController::getInstance().getState() == playController::PlayState::STOP_STATE)
        {
            m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                          "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                          "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        }

//        m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
//                                 "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
//                                 "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
        slotFavExixts();

        m_closeBtn->setIcon(QIcon(":/img/clicked/close.png"));
        m_closeBtn->setIconSize(QSize(30, 30));
        m_closeBtn->setStyleSheet("QPushButton{background:transparent;}");

        m_recoveryWinBtn->setIcon(QIcon(":/img/lightshow/maximize.png"));
        m_recoveryWinBtn->setIconSize(QSize(30, 30));
        m_recoveryWinBtn->setStyleSheet("QPushButton{background-color:transparent;}");

        m_nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/nextsong.png);}"
                                 "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

        coverWid->setStyleSheet("background-color:rgba(0,0,0,0.4);\
                                border-top-right-radius:6px;border-bottom-right-radius:6px;\
                                border-top-left-radius:0px;border-bottom-left-radius:0px");

    }
}

void miniWidget::init_miniWidget()
{
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);

    m_HMainLayout = new QHBoxLayout();
    m_hLayout = new QHBoxLayout();

    m_mainFrame = new QFrame;
    m_mainFrame->setFixedSize(300,60);
    m_mainFrame->setObjectName("mainFrame");

    m_hMainLayout = new QHBoxLayout(m_mainFrame);

    /*******************************************/
    m_coverLabel = new QLabel;
    m_coverLabel->setFixedSize(40,40);
    m_coverLabel->setCursor(Qt::PointingHandCursor);

    /************************************/
    m_vInfoLayout = new QVBoxLayout;

    m_songNameLab = new MyLabel;
    m_songNameLab->setFixedSize(180,20);
    m_songNameLab->setText(tr("Music Player"));
    m_songNameLab->setAlignment(Qt::AlignLeft);

    m_timeLab = new QLabel;
    m_timeLab->setFixedSize(180,20);
    m_timeLab->setText(tr("00:00/00:00"));
    m_timeLab->setAlignment(Qt::AlignLeft);

//    m_vInfoLayout->setMargin(3);
//    m_vInfoLayout->setSpacing(3);
    m_vInfoLayout->addWidget(m_songNameLab);
//    m_vInfoLayout->addSpacing(8);
    m_vInfoLayout->addWidget(m_timeLab);
//    m_vInfoLayout->setAlignment(Qt::AlignVCenter);

    /******************************************/
    m_palyFrame = new QFrame;
    m_palyFrame->setFixedWidth(120);
    m_palyFrame->setVisible(false);

    m_hPlayLayout = new QHBoxLayout(m_palyFrame);

    m_preBtn = new QPushButton;
    m_preBtn->setFixedSize(16,16);
    m_preBtn->setCursor(Qt::PointingHandCursor);

    m_playStateBtn = new QPushButton;
    m_playStateBtn->setFixedSize(36,36);
    m_playStateBtn->setCursor(Qt::PointingHandCursor);

    m_nextBtn = new QPushButton;
    m_nextBtn->setFixedSize(16,16);
    m_nextBtn->setCursor(Qt::PointingHandCursor);

    m_hPlayLayout->setMargin(0);
    m_hPlayLayout->setSpacing(9);
    m_hPlayLayout->addWidget(m_preBtn);
    m_hPlayLayout->addWidget(m_playStateBtn);
    m_hPlayLayout->addWidget(m_nextBtn);
    /****************************************************/

    m_loveBtn = new QPushButton;
    m_loveBtn->setFixedSize(16,16);
    m_loveBtn->setCursor(Qt::PointingHandCursor);

    m_orderBtn = new QPushButton;
    m_orderBtn->setFixedSize(25,25);
    m_orderBtn->setCursor(Qt::PointingHandCursor);
    m_orderBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
    m_orderBtn->setProperty("isWindowButton", 0x1);
    m_orderBtn->setProperty("useIconHighlightEffect", 0x2);
    m_orderBtn->setFlat(true);

    /***************************************/
    m_vSysLayout = new QVBoxLayout;
    coverWid = new QWidget(this);
//    coverWid->setAttribute(Qt::WA_TranslucentBackground, true);     //窗体透明

    m_closeBtn = new QPushButton;
    m_closeBtn->setFixedSize(30,30);
    m_closeBtn->setCursor(Qt::PointingHandCursor);

    connect(m_closeBtn,&QPushButton::clicked,this,&miniWidget::close);
    //just hide not close

    m_recoveryWinBtn = new QPushButton;
    m_recoveryWinBtn->setFixedSize(30,30);
    m_recoveryWinBtn->setCursor(Qt::PointingHandCursor);


    m_vSysLayout->setSpacing(0);
    m_vSysLayout->setMargin(0);
    m_vSysLayout->addWidget(m_closeBtn);
    m_vSysLayout->addWidget(m_recoveryWinBtn);
//    m_vSysLayout->addStretch();

    coverWid->setLayout(m_vSysLayout);
    /********************************************/

    m_hMainLayout->setMargin(10);
    m_hMainLayout->addWidget(m_coverLabel);
    m_hMainLayout->addLayout(m_vInfoLayout);
    m_hMainLayout->addStretch();
    m_hMainLayout->addWidget(m_palyFrame);
    m_hMainLayout->addStretch();
    m_hMainLayout->addWidget(m_loveBtn);
    m_hMainLayout->addWidget(m_orderBtn);
//    m_hMainLayout->addLayout(m_vSysLayout);


    m_mainFrame->setLayout(m_hMainLayout);

    m_HMainLayout->setMargin(0);
    m_HMainLayout->addWidget(m_mainFrame);
    m_HMainLayout->addWidget(coverWid);

    //限制应用字体不随着主题变化
    QFont sizeFont;
    sizeFont.setPixelSize(14);
    m_songNameLab->setFont(sizeFont);
    m_timeLab->setFont(sizeFont);


//    QFrame *controlFrame = new QFrame(this);
//    controlFrame->setLayout(m_vSysLayout);

//    m_hLayout->addWidget(m_mainFrame);
//    m_hLayout->addWidget(controlFrame);

//    m_mainFrame->setStyleSheet("border-radius:12px;");
}

//初始化样式
void miniWidget::initStyle()
{
    //不接受焦点高亮。解决点击后有蓝框的问题
    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    for(QPushButton *btn :list)
        btn->setFocusPolicy(Qt::NoFocus);
}

void miniWidget::initConnect()
{
    connect(&playController::getInstance(),&playController::playerStateChange,this,&miniWidget::playerStateChange);
    connect(&playController::getInstance(),&playController::singalChangePath,this,&miniWidget::slotSongInfo);
    connect(m_loveBtn,&QPushButton::clicked,this,&miniWidget::slotFav);
    connect(m_orderBtn,&QPushButton::clicked,this,&miniWidget::slotPlayModeClicked);
    connect(&playController::getInstance(),&playController::signalPlayMode,this,&miniWidget::setPlayMode);
    connect(&playController::getInstance(),&playController::signalNotPlaying,this,&miniWidget::slotNotPlaying);
}

void miniWidget::playerStateChange(playController::PlayState newState)
{
    if(newState == playController::PlayState::PLAY_STATE)
    {
        m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                      "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                      "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
    }
    else if(newState == playController::PlayState::PAUSED_STATE)
    {
        m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                      "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                      "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
    }
    else if(newState == playController::PlayState::STOP_STATE)
    {
        m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                      "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                      "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
    }
}

void miniWidget::slotSongInfo(QString path)
{
    filePath = path.remove("file://");
    slotFavExixts();
}

void miniWidget::slotText(QString btnText)
{
    listName = btnText;
}

void miniWidget::slotFav()
{
    if(g_db->checkSongIsInFav(filePath))
    {
        QList<musicDataStruct> resList;
        int ref = g_db->getSongInfoListFromDB(resList, "我喜欢");
        int ret = g_db->delMusicFromPlayList(filePath,"我喜欢");
        if(ref == DB_OP_SUCC)
        {
            //根据歌单名title值查询对应歌单列表
//            int ref = g_db->getSongInfoListFromDB(resList, "我喜欢");
            if(ret == DB_OP_SUCC)
            {
                for(int i = 0; i < resList.size(); i++)
                {
                    if(resList.at(i).filepath == filePath)
                    {
                        playController::getInstance().removeSongFromCurList("我喜欢", i);
                        //当前为我喜欢界面才刷新
                        if(listName == "我喜欢")
                        {
                            emit signalRefreshFav("我喜欢");
                        }
                        break;
                    }
                }
            }
        }
    }
    else
    {
        int ref = g_db->addMusicToPlayList(filePath,"我喜欢");
        if(ref == DB_OP_SUCC)
        {
            playController::getInstance().addSongToCurList("我喜欢", filePath);
            if(listName == "我喜欢")
            {
                emit signalRefreshFav("我喜欢");
            }
        }
    }
    slotFavExixts();

}

void miniWidget::slotFavExixts()
{
    if(WidgetStyle::themeColor == 1)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");

        }
        else
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
    }
    else if(WidgetStyle::themeColor == 0)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");

        }
        else
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
    }
    emit signalFavBtnChange(filePath);
}

void miniWidget::slotFavExixtsDark()
{
    if(g_db->checkSongIsInFav(filePath))
    {
        m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");

    }
    else
    {
        m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/ukui-play-love-symbolic-w.svg);}");
    }
    emit signalFavBtnChange(filePath);
}

void miniWidget::slotFavIsExixts(QString filePath)
{
    if(WidgetStyle::themeColor == 1)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            //由于mini歌曲title是播放区传送故判断 ""
            if(m_songNameLab->text() == "")
            {
                return;
            }
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");

        }
        else
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
    }
    else if(WidgetStyle::themeColor == 0)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            //由于mini歌曲title是播放区传送故判断 ""
            if(m_songNameLab->text() == "")
            {
                return;
            }
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");

        }
        else
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
    }
}

void miniWidget::slotFavBtnChange(QString filePath)
{
    if(WidgetStyle::themeColor == 1)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");

        }
        else
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
    }
    else if(WidgetStyle::themeColor == 0)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");

        }
        else
        {
            m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                     "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
    }
}

void miniWidget::slotPlayingLab(QString playing)
{
    m_songNameLab->setText(playing);
}

void miniWidget::slotTimeLab(QString time)
{
    m_timeLab->setText(time);
}

void miniWidget::slotPlayModeClicked()
{
    int playMode = playController::getInstance().playmode();
    qDebug() << "playMode" << playMode;
    switch (playMode)
    {
    case 1:
        ++playMode;
        ++playMode;
        break;
    case 3:
        ++playMode;
        break;
   case 4:
        playMode = 1;
    default:
        break;
    }
    setPlayMode(playMode);
}

void miniWidget::setPlayMode(int playModel)
{
    switch (playModel) {
    case 1:
        m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one-symbolic"));
        m_orderBtn->setToolTip(tr("CurrentItemInLoop"));
        playController::getInstance().setPlaymode(playController::CurrentItemInLoop);
        break;
    case 3:
        m_orderBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
        m_orderBtn->setToolTip(tr("Loop"));
        playController::getInstance().setPlaymode(playController::Loop);
        break;
    case 4:
        m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle-symbolic"));
        m_orderBtn->setToolTip(tr("Random"));
        playController::getInstance().setPlaymode(playController::Random);
        break;
    default:
        break;
    }
}

void miniWidget::slotNotPlaying()
{
    m_songNameLab->setText(tr("Music Player"));
    m_timeLab->setText("00:00/00:00");
}

void miniWidget::songText(QString songName)
{
    QString show_songName = "";
    if(songName.length() > 10)
    {
        show_songName = songName.mid(0,9);
        show_songName.append("...");
        m_songNameLab->setText(show_songName);
        m_songNameLab->setToolTip(songName);
    }
    else
    {
        m_songNameLab->setText(songName);
        m_songNameLab->setToolTip(songName);
    }
}
