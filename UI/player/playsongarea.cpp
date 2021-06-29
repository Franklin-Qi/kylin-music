#include "playsongarea.h"
#include "UI/base/widgetstyle.h"
#include "UI/base/xatom-helper.h"
#include "UIControl/base/musicDataBase.h"
#define VOLUME 5

PlaySongArea::PlaySongArea(QWidget *parent) : QWidget(parent)
{
    resize(750,88);
    setAttribute(Qt::WA_StyledBackground,true);
    this->setObjectName("PlaySongArea");
    initWidget();
    initConnect();
    playcolor();
}

void PlaySongArea::initWidget()
{
    QVBoxLayout *m_vmainLayout = new QVBoxLayout(this);
    QHBoxLayout *m_hmainLayout = new QHBoxLayout();
    m_mainLayout = new QHBoxLayout();

    preBtn = new QPushButton;
    preBtn->setFixedSize(18,18);
    preBtn->setCursor(Qt::PointingHandCursor);
    preBtn->setToolTip(tr("Previous"));

    playBtn = new QPushButton(this);
    playBtn->setFixedSize(36,36);
    playBtn->setCursor(Qt::PointingHandCursor);
//    playBtn->setToolTip("播放");
//    playBtn->setToolTip(tr("play"));

    nextBtn = new QPushButton;
    nextBtn->setFixedSize(18,18);
    nextBtn->setCursor(Qt::PointingHandCursor);
//    nextBtn->setToolTip("下一首");
    nextBtn->setToolTip(tr("Next"));

    volumeBtn = new QPushButton(this);
    volumeBtn->setFixedSize(25,25);
    volumeBtn->setCursor(Qt::PointingHandCursor);
    volumeBtn->setToolTip(tr("Volume"));    //音量
    int volume = playController::getInstance().getVolume();
    if(volume == 0)
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-muted-symbolic"));
        volumeBtn->setProperty("isWindowButton", 0x1);
        volumeBtn->setProperty("useIconHighlightEffect", 0x2);
        volumeBtn->setFlat(true);
    }
    else if(volume > 0 && volume <= 33)
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-low-symbolic"));
        volumeBtn->setProperty("isWindowButton", 0x1);
        volumeBtn->setProperty("useIconHighlightEffect", 0x2);
        volumeBtn->setFlat(true);
    }
    else if(volume > 33 && volume <= 77)
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-medium-symbolic"));
        volumeBtn->setProperty("isWindowButton", 0x1);
        volumeBtn->setProperty("useIconHighlightEffect", 0x2);
        volumeBtn->setFlat(true);
    }
    else
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-high-symbolic"));
        volumeBtn->setProperty("isWindowButton", 0x1);
        volumeBtn->setProperty("useIconHighlightEffect", 0x2);
        volumeBtn->setFlat(true);
    }

    m_volSliderWid = new SliderWidget(this);
    MotifWmHints hint;
    hint.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hint.functions = MWM_FUNC_ALL;
    hint.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(m_volSliderWid->winId(), hint);
    m_volSliderWid->hide();

    m_playBackModeWid = new PlayBackModeWidget(this);
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(m_playBackModeWid->winId(), hints);
    m_playBackModeWid->hide();

    hSlider = new MusicSlider(this);
    hSlider->setValue(0);
    hSlider->setDisabled(true);
    favBtn = new QPushButton;
    favBtn->setFixedSize(16,16);
//    favBtn->setCheckable(true); //按钮是否是可点击状态，默认不点击
//    favBtn->setChecked(false);
    favBtn->setCursor(Qt::PointingHandCursor);
    favBtn->setFocusPolicy(Qt::NoFocus);
    favBtn->setToolTip(tr("Favourite"));

    //播放模式
    playModeBtn = new QPushButton;
    playModeBtn->setFixedSize(25,25);
    playModeBtn->setCursor(Qt::PointingHandCursor);
    playModeBtn->setIcon(QIcon::fromTheme("ukui-playlist-order-symbolic"));
    playModeBtn->setProperty("isWindowButton", 0x1);
    playModeBtn->setProperty("useIconHighlightEffect", 0x2);
    playModeBtn->setFlat(true);
    playModeBtn->setToolTip(tr("Loop"));   //列表循环

    //历史播放列表
    listBtn = new QPushButton;
    listBtn->setFixedSize(25,25);
    listBtn->setCheckable(true); //按钮是否是可点击状态，默认不点击
//    listBtn->setChecked(false);    //只可检查按钮是否是点击状态，保存点击的状态
    listBtn->setCursor(Qt::PointingHandCursor);
    listBtn->setIcon(QIcon::fromTheme("ukui-play-list-symbolic"));
    listBtn->setProperty("isWindowButton", 0x1);
    listBtn->setProperty("useIconHighlightEffect", 0x2);
    listBtn->setFlat(true);
    listBtn->setToolTip(tr("Play List"));

    coverPhotoLabel = new QLabel(this);
    coverPhotoLabel->setFixedSize(40,40);

    playingLabel = new MyLabel(this);
    playingLabel->setFixedHeight(20);
//    playingLabel->setText(tr("Music Player"));

    timeLabel = new QLabel(this);
//    timeLabel->setText(tr("00:00/00:00"));

    QString playPath = playController::getInstance().getPath();
    if(playPath != "")
    {
        slotSongInfo(playPath);
        slotPositionChanged(0);
    }
    else
    {
        playingLabel->setText(tr("Music Player"));
        timeLabel->setText(tr("00:00/00:00"));
    }

    QWidget *letfWid = new QWidget(this);
    QWidget *centreWid = new QWidget(this);
    QWidget *rightWid = new QWidget(this);

    QHBoxLayout *leftLayout = new QHBoxLayout(letfWid);

    QWidget *vWidget = new QWidget(letfWid);
    vWidget->setFixedHeight(40);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addStretch();
    vLayout->addWidget(playingLabel,0,Qt::AlignVCenter);
    vLayout->addSpacing(8);
    vLayout->addWidget(timeLabel,0,Qt::AlignVCenter);
    vLayout->addStretch();
    vLayout->setContentsMargins(8,3,0,3);
    vLayout->setMargin(0);
    vWidget->setLayout(vLayout);


    leftLayout->addWidget(coverPhotoLabel,Qt::AlignVCenter);
    leftLayout->addWidget(vWidget,Qt::AlignVCenter);
    leftLayout->setMargin(0);

    QHBoxLayout *playLayout = new QHBoxLayout(centreWid);
    playLayout->addStretch(0);
    playLayout->addWidget(preBtn,Qt::AlignHCenter);
    playLayout->addSpacing(12);
    playLayout->addWidget(playBtn,Qt::AlignHCenter);
    playLayout->addSpacing(12);
    playLayout->addWidget(nextBtn,Qt::AlignHCenter);
    playLayout->addStretch();
    playLayout->setMargin(0);

    QHBoxLayout *rightLayout = new QHBoxLayout(rightWid);
    rightLayout->addStretch(0);
    rightLayout->addWidget(volumeBtn,Qt::AlignRight);
    rightLayout->addSpacing(8);
    rightLayout->addWidget(favBtn,Qt::AlignRight);
    rightLayout->addSpacing(8);
    rightLayout->addWidget(playModeBtn,Qt::AlignRight);
    rightLayout->addSpacing(8);
    rightLayout->addWidget(listBtn,Qt::AlignRight);
    rightLayout->setContentsMargins(0,14,30,14);
    rightLayout->setMargin(0);

    letfWid->setLayout(leftLayout);
//    letfWid->setStyleSheet("background: red;");
    centreWid->setLayout(playLayout);
//    centreWid->setStyleSheet("background: blue;");
    rightWid->setLayout(rightLayout);
//    rightWid->setStyleSheet("background: green;");

    m_hmainLayout->addWidget(hSlider);
    m_hmainLayout->setMargin(0);
    m_hmainLayout->setSpacing(0);

    m_mainLayout->addWidget(letfWid);
//    m_mainLayout->addStretch(0);
    m_mainLayout->addWidget(centreWid);
//    m_mainLayout->addStretch(0);
    m_mainLayout->addWidget(rightWid);
//    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);

    m_mainLayout->setStretchFactor(letfWid,1);
    m_mainLayout->setStretchFactor(centreWid,1);
    m_mainLayout->setStretchFactor(rightWid,1);
    m_mainLayout->setContentsMargins(25,0,25,0);


    m_vmainLayout->addLayout(m_hmainLayout);
    m_vmainLayout->addLayout(m_mainLayout);
    m_vmainLayout->setContentsMargins(0,0,0,8);
    m_vmainLayout->setSpacing(0);

    this->setLayout(m_vmainLayout);


    //限制应用字体不随着主题变化
    QFont sizeFont;
    sizeFont.setPixelSize(14);
    playingLabel->setFont(sizeFont);
    timeLabel->setFont(sizeFont);
}

void PlaySongArea::initConnect()
{
    //
    connect(preBtn,&QPushButton::clicked,this,&PlaySongArea::slotPrevious);
    connect(playBtn,&QPushButton::clicked,this,&PlaySongArea::slotPlayClicked);
    connect(nextBtn,&QPushButton::clicked,this,&PlaySongArea::slotNext);
    connect(listBtn,&QPushButton::clicked,this,&PlaySongArea::listBtnClicked);
    connect(m_volSliderWid->vSlider,&QSlider::valueChanged,this,&PlaySongArea::slotVolumeChanged);
    connect(volumeBtn,&QPushButton::clicked,this,&PlaySongArea::slotVolSliderWidget);
    connect(favBtn,&QPushButton::clicked,this,&PlaySongArea::slotFav);
    connect(playModeBtn,&QPushButton::clicked,this,&PlaySongArea::slotPlayBackModeChanged);
    connect(m_playBackModeWid->loopBtn,&QToolButton::clicked,this,&PlaySongArea::slotLoopClicked);
    connect(m_playBackModeWid->randomBtn,&QToolButton::clicked,this,&PlaySongArea::slotRandomClicked);
    connect(m_playBackModeWid->sequentialBtn,&QToolButton::clicked,this,&PlaySongArea::slotSequentialClicked);
    connect(m_playBackModeWid->currentItemInLoopBtn,&QToolButton::clicked,this,&PlaySongArea::slotCurrentItemInLoopClicked);
    connect(&playController::getInstance(),&playController::singalChangePath,this,&PlaySongArea::slotSongInfo);
    connect(&playController::getInstance(),&playController::playerStateChange,this,&PlaySongArea::playerStateChange);
    connect(playController::getInstance().getPlayer(),SIGNAL(positionChanged(qint64)),this,SLOT(slotPositionChanged(qint64)));
    connect(playController::getInstance().getPlayer(),SIGNAL(durationChanged(qint64)),this,SLOT(slotDurationChanged(qint64)));
    connect(hSlider,SIGNAL(sliderPressed()),this,SLOT(slotSlidePressd()));
    connect(hSlider,SIGNAL(sliderReleased()),this,SLOT(slotSlideReleased()));
    connect(hSlider,&MusicSlider::valueChanged,this,&PlaySongArea::setPosition);
    connect(&playController::getInstance(),&playController::signalPlayMode,this,&PlaySongArea::setPlayMode);
    connect(&playController::getInstance(),&playController::signalNotPlaying,this,&PlaySongArea::slotNotPlaying);
}

void PlaySongArea::slotVolumeChanged(int values)
{
    playController::getInstance().setVolume(values);
    int volume = playController::getInstance().getVolume();
    if(volume == 0)
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-muted-symbolic"));
    }
    else if(volume > 0 && volume <= 33)
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-low-symbolic"));
    }
    else if(volume > 33 && volume <= 77)
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-medium-symbolic"));
    }
    else
    {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-high-symbolic"));
    }
}

void PlaySongArea::volumeIncrease()
{
    int value = m_volSliderWid->vSlider->value() + VOLUME;
    if(value > 100)
        value = 100;
    m_volSliderWid->vSlider->setValue(value);
}

void PlaySongArea::volumeReduce()
{
    int value = m_volSliderWid->vSlider->value() - VOLUME;
    if(value < 0)
        value = 0;
    m_volSliderWid->vSlider->setValue(value);
}

void PlaySongArea::slotVolSliderWidget()
{
    if(m_volSliderWid->isVisible())
    {
        m_volSliderWid->hide();
    }
    else
    {
        moveVolSliderWid();
        m_volSliderWid->show();
        m_volSliderWid->raise();
    }
}

void PlaySongArea::slotText(QString btnText)
{
    listName = btnText;
}

void PlaySongArea::slotFav()
{
    if(g_db->checkSongIsInFav(filePath))
    {
        QList<musicDataStruct> resList;
        int ref = g_db->getSongInfoListFromDB(resList, "我喜欢");
        int ret = g_db->delMusicFromPlayList(filePath,"我喜欢");
        if(ref == DB_OP_SUCC)
        {
//            emit signalAddFromFavButton("我喜欢");
            //根据歌单名title值查询对应歌单列表
//            int ref = g_db->getSongInfoListFromDB(resList, "我喜欢");
            if(ret == DB_OP_SUCC)
            {
                for(int i = 0; i < resList.size(); i++)
                {
                    if(resList.at(i).filepath == filePath)
                    {
                        playController::getInstance().removeSongFromCurList("我喜欢", i);
                        //刷新我喜欢界面
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
//            emit signalDelFromFavButton("我喜欢");
            if(listName == "我喜欢")
            {
                emit signalRefreshFav("我喜欢");
            }
        }
//        emit signalRefreshFav("我喜欢");
    }
    slotFavExixts();
//    if(favBtn->isVisible())
//    {
////        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
//        favBtn->setIcon(QIcon(":/img/clicked/love1.png"));
//    }
//    else
//    {
//        favBtn->setIcon(QIcon(":/img/default/loveblack2.png"));
//    }

////    favBtn->setIcon(QIcon(":/img/clicked/love1.png"));
}

void PlaySongArea::slotPlayBackModeChanged()
{
    if(m_playBackModeWid->isVisible())
    {
        m_playBackModeWid->hide();
    }
    else
    {
        movePlayModeWid();
        m_playBackModeWid->show();
        m_playBackModeWid->raise();
    }
}

void PlaySongArea::slotLoopClicked()
{
    playModeBtn->setIcon(QIcon::fromTheme("ukui-playlist-order-symbolic"));
    playModeBtn->setToolTip(tr("Loop"));
    playController::getInstance().setPlaymode(playController::Loop);
    m_playBackModeWid->hide();
}

void PlaySongArea::slotRandomClicked()
{
    playModeBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle-symbolic"));
    playModeBtn->setToolTip(tr("Random"));
    playController::getInstance().setPlaymode(playController::Random);
    m_playBackModeWid->hide();
}

void PlaySongArea::slotSequentialClicked()
{
    playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-symbolic"));
    playModeBtn->setToolTip(tr("Sequential"));
    playController::getInstance().setPlaymode(playController::Sequential);
    m_playBackModeWid->hide();
}

void PlaySongArea::slotCurrentItemInLoopClicked()
{
    playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one-symbolic"));
    playModeBtn->setToolTip(tr("CurrentItemInLoop"));
    playController::getInstance().setPlaymode(playController::CurrentItemInLoop);
    m_playBackModeWid->hide();
}

void PlaySongArea::setPlayMode(int playModel)
{
    switch (playModel) {
    case 1:
        playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one-symbolic"));
        playModeBtn->setToolTip(tr("CurrentItemInLoop"));
        playController::getInstance().setPlaymode(playController::CurrentItemInLoop);
        break;
//    case 2:
//        playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-symbolic"));
//        playModeBtn->setToolTip(tr("Sequential"));
//        playController::getInstance().setPlaymode(playController::Sequential);
//        break;
    case 3:
        playModeBtn->setIcon(QIcon::fromTheme("ukui-playlist-order-symbolic"));
        playModeBtn->setToolTip(tr("Loop"));
        playController::getInstance().setPlaymode(playController::Loop);
        break;
    case 4:
        playModeBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle-symbolic"));
        playModeBtn->setToolTip(tr("Random"));
        playController::getInstance().setPlaymode(playController::Random);
        break;
    default:
        break;
    }
}

void PlaySongArea::slotSongInfo(QString path)
{
    filePath = path.remove("file://");
    musicDataStruct musicStruct;
    g_db->getSongInfoFromDB(filePath, musicStruct);
    //使用库解析总时间
    m_time = musicStruct.time;
    if(musicStruct.title == "")
    {
        playingLabel->setText(tr("Music Player"));
        emit signalPlayingLab(tr("Music Player"));
    }
    else
    {
        hSlider->isPlaying(true);
        playingLabel->setText(musicStruct.title);
        emit signalPlayingLab(musicStruct.title);
    }
    slotFavExixts();
}

void PlaySongArea::playerStateChange(playController::PlayState newState)
{
    if(newState == playController::PlayState::PLAY_STATE)
    {
        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
    }
    else if(newState == playController::PlayState::PAUSED_STATE)
    {
        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                            "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                             "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
    }
    else if(newState == playController::PlayState::STOP_STATE)
    {
        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                            "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                             "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
    }
}

void PlaySongArea::slotPositionChanged(qint64 position)
{
    QTime duration(0, static_cast<int>(position) / 60000, static_cast<int>((position % 60000) / 1000.0));
    QString str_time = duration.toString("mm:ss");
    QString length = str_time + "/" + m_time;
    if(m_time == "")
    {
        timeLabel->setText("00:00/00:00");
        emit signalTimeLab("00:00/00:00");
    }
    else
    {
        timeLabel->setText(length);
        emit signalTimeLab(length);
    }

    if(hSlider->signalsBlocked())
    {
        return;
    }
    hSlider->blockSignals(true);
    hSlider->setValue(static_cast<int>(position));

    hSlider->blockSignals(false);
    hSlider->update();
}

void PlaySongArea::slotNotPlaying()
{
    //滑块可以点击，无法跳转
    hSlider->isPlaying(false);
    //禁用
    hSlider->setDisabled(true);
    playingLabel->setText(tr("Music Player"));
    timeLabel->setText("00:00/00:00");
}



void PlaySongArea::slotDurationChanged(qint64 duration)
{
    hSlider->setRange(0,static_cast<int>(duration));
    hSlider->setEnabled(duration>0);
    hSlider->setPageStep(static_cast<int>(duration)/10);
}

void PlaySongArea::slotSlidePressd()
{
    playController::getInstance().getPlayer()->pause();
}

void PlaySongArea::slotSlideReleased()
{
    playController::getInstance().getPlayer()->play();
}

void PlaySongArea::setPosition(int position)
{
//    qDebug() << "position" << position;
//    qDebug() << "playController::getInstance().getPlayer()->position()" << playController::getInstance().getPlayer()->position();
//    qDebug() << "playController::getInstance().getPlayer()->position() - position" << playController::getInstance().getPlayer()->position() - position;
    //判断播放的位置 - 滑块的位置是否大于0.1s
    if (qAbs(playController::getInstance().getPlayer()->position() - position) > 99)
       playController::getInstance().getPlayer()->setPosition(position);
}

void PlaySongArea::resizeEvent(QResizeEvent *event)
{
    moveVolSliderWid();
    movePlayModeWid();
    playingLabel->setFixedWidth(this->width()/3.6);
    QWidget::resizeEvent(event);
}

void PlaySongArea::moveVolSliderWid()
{
    QPoint volumePos = volumeBtn->mapToGlobal(volumeBtn->rect().center());
    m_volSliderWid->adjustSize();
    QSize size = m_volSliderWid->size();
    volumePos.setX(volumePos.x() - size.width() / 2);
    volumePos.setY(volumePos.y() - size.height() - 25);
    QSize volumSize = volumeBtn->size();
    int newPosX = volumePos.x() + size.width() / 2 - volumSize.width() / 2 + 1;
    int newPosY = volumePos.y() + 25 + size.height() - volumSize.height() / 2 + 1;
    m_volSliderWid->changeVolumePos(newPosX, newPosY, volumSize.width(), volumSize.height());
    m_volSliderWid->move(volumePos);
}

void PlaySongArea::movePlayModeWid()
{
    QPoint modePos = playModeBtn->mapToGlobal(playModeBtn->rect().center());
    m_playBackModeWid->adjustSize();
    QSize size = m_playBackModeWid->size();
    modePos.setX(modePos.x() - size.width() / 2);
    modePos.setY(modePos.y() - size.height() - 25);
    QSize modeSize = playModeBtn->size();
    int newPosX = modePos.x() + size.width() / 2 - modeSize.width() / 2 + 1;
    int newPosY = modePos.y() + 25 + size.height() - modeSize.height() / 2 + 1;
    m_playBackModeWid->changePlayModePos(newPosX, newPosY, modeSize.width(), modeSize.height());
    m_playBackModeWid->move(modePos);
}

void PlaySongArea::slotPlayClicked()
{
    if(playController::getInstance().getPlayer()->state() == MMediaPlayer::PlayingState)
    {
        playController::getInstance().getPlayer()->pause();
    }
    else if(playController::getInstance().getPlayer()->state() == MMediaPlayer::PausedState)
    {
        playController::getInstance().getPlayer()->play();
    }
    else if(playController::getInstance().getPlayer()->state() == MMediaPlayer::StoppedState)
    {
        playMeta();
    }
}

void PlaySongArea::playMeta()
{
    QString playListName = playController::getInstance().getPlayListName();
    QString playPath = playController::getInstance().getPath();
    QList<musicDataStruct> resList;
    QStringList filePaths;
    int index;
    int ret = g_db->getSongInfoListFromDB(resList, playListName);
    if(playPath != "")
    {
        if(ret == DB_OP_SUCC)
        {
            if(resList.size() == 0)
            {
                return;
            }
            for(int i = 0; i < resList.size(); i++)
            {
                filePaths << resList.at(i).filepath;
                if(resList.at(i).filepath == playPath)
                {
                    index = i;
                }
            }
            playController::getInstance().setCurPlaylist(playListName, filePaths);
            playController::getInstance().play(playListName, index);
        }
    }
    else
    {
        QList<musicDataStruct> resList;
        int ref = g_db->getSongInfoListFromDB(resList,ALLMUSIC);
        if(ref == DB_OP_SUCC)
        {
            if(resList.size() == 0)
            {
                return;
            }
            if(resList.size() >= 1)
            {
                QStringList paths;
                for(int i = 0; i < resList.size(); i++)
                {
                    paths << resList.at(i).filepath;
                }
                playController::getInstance().setCurPlaylist(ALLMUSIC,paths);
                playController::getInstance().play(ALLMUSIC,0);
            }
        }
    }
}

void PlaySongArea::slotPrevious()
{
    playController::getInstance().onPreviousSong();
}

void PlaySongArea::slotNext()
{
    playController::getInstance().onNextSong();
}

void PlaySongArea::listBtnClicked()
{
    emit showHistoryListBtnClicked();
}

void PlaySongArea::slotFavExixts()
{
    if(WidgetStyle::themeColor == 1)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            //        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
        else
        {
            //        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
            //                              "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
            //                              "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}"
            //                              "QPushButton::checked{border-image:url(:/img/clicked/love1.png);}");
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
    }
    else if(WidgetStyle::themeColor == 0)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            //        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
        else
        {
            //        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
            //                              "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
            //                              "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}"
            //                              "QPushButton::checked{border-image:url(:/img/clicked/love1.png);}");
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
    }
    emit signalFavBtnChange(filePath);
}

void PlaySongArea::slotFavExixtsDark()
{
    if(g_db->checkSongIsInFav(filePath))
    {
//        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
    }
    else
    {
//        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/icon_love2_b@2x.png);}");
//        favBtn->setIcon(QIcon(":/img/dark/icon_love2_b@2x.png"));
        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/ukui-play-love-symbolic-w.svg);}");
//        favBtn->setIcon(QIcon(":/img/dark/ukui-play-love-symbolic-w.svg"));
    }
    emit signalFavBtnChange(filePath);
}
void PlaySongArea::slotHistoryBtnChecked(bool checked)
{
    listBtn->setChecked(checked);
}
void PlaySongArea::slotFavIsExixts(QString filePath)
{
    if(WidgetStyle::themeColor == 1)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            if(playingLabel->text() == tr("Music Player"))
            {
                return;
            }
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
        else
        {
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.png);}");
        }
    }
    else if(WidgetStyle::themeColor == 0)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            if(playingLabel->text() == tr("Music Player"))
            {
                return;
            }
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
        else
        {
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.png);}");
        }
    }
}

void PlaySongArea::slotFavBtnChange(QString filePath)
{
    if(WidgetStyle::themeColor == 1)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            if(playingLabel->text() == tr("Music Player"))
            {
                return;
            }
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
        else
        {
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/dark/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.png);}");
        }
    }
    else if(WidgetStyle::themeColor == 0)
    {
        if(g_db->checkSongIsInFav(filePath))
        {
            if(playingLabel->text() == tr("Music Player"))
            {
                return;
            }
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.svg);}");
        }
        else
        {
            favBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::hover{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::pressed{border-image:url(:/img/hover/ukui-play-love-symbolic-w.svg);}"
                                  "QPushButton::checked{border-image:url(:/img/clicked/ukui-play-love-symbolic-w.png);}");
        }
    }
}

void PlaySongArea::playcolor()
{
    if(WidgetStyle::themeColor == 1)
    {
        this->setStyleSheet("#PlaySongArea{background-color:#252526;}");
        preBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/lastsong.png);}"
                              "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");

//        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
//                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
//                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        if(playController::getInstance().getState() == playController::PlayState::PLAY_STATE)
        {
            playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                   "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                   "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
        }
        else if(playController::getInstance().getState() == playController::PlayState::PAUSED_STATE)
        {
            playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        }
        else if(playController::getInstance().getState() == playController::PlayState::STOP_STATE)
        {
            playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        }

        nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

//        favBtn->setIcon(QIcon(":/img/dark/icon_love2_b@2x.png"));
//        favBtn->setStyleSheet("QPushButton{border-image:url(::/img/dark/icon_love2_b@2x.png);}"
//                              "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
//                              "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
        slotFavExixtsDark();

//        listBtn->setIcon(QIcon(":/img/dark/icon_songlist_h@2x.png"));
//        listBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/dark/icon_songlist_h@2x.png);}"
//                               "QPushButton::hover{border-image:url(:/img/clicked/songlist.png);}"
//                               "QPushButton::checked{border-image:url(:/img/clicked/songlist.png);}");

        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        playingLabel->setStyleSheet("font-size:14px;color:#F9F9F9;line-height:14px;");

        hSlider->initStyle();
    }
    else if(WidgetStyle::themeColor == 0)
    {
        this->setStyleSheet("#PlaySongArea{background-color:#FFFFFF;}");
        preBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/lastsong.png);}"
                              "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");

//        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
//                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
//                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        if(playController::getInstance().getState() == playController::PlayState::PLAY_STATE)
        {
            playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                   "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                   "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
        }
        else if(playController::getInstance().getState() == playController::PlayState::PAUSED_STATE)
        {
            playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        }
        else if(playController::getInstance().getState() == playController::PlayState::STOP_STATE)
        {
            playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        }

        nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

//        favBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
//                              "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
//                              "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}"
//                              "QPushButton::checked{border-image:url(:/img/clicked/love1.png);}");
        slotFavExixts();

//        listBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/songlist.png)}"
//                               "QPushButton::hover{border-image:url(:/img/clicked/songlist.png);}"
//                               "QPushButton::checked{border-image:url(:/img/clicked/songlist.png);}");

        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        playingLabel->setStyleSheet("line-height:14px;color:#303133;font-size:14px;");

        timeLabel->setStyleSheet("line-height:12px;color:#8F9399;font-size:12px;");

        hSlider->initStyle();

    }
}
