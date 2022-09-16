#include <ukui-log4qt.h>
#include <QApplication>
#include <QPixmap>

#include "playsongarea.h"
#include "UI/base/widgetstyle.h"
#include "UI/base/xatom-helper.h"
#include "UIControl/base/musicDataBase.h"
#define VOLUME 5
#define PT_9 9

PlaySongArea::PlaySongArea(QWidget *parent) : QWidget(parent)
{
    resize(750,88);
    setAttribute(Qt::WA_StyledBackground,true);
    this->setObjectName("PlaySongArea");
    initWidget();
    initConnect();
    initStyle();
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
    playBtn->setIconSize(QSize(12, 12));
    playBtn->setCursor(Qt::PointingHandCursor);
    playBtn->setToolTip(tr("Play"));

    nextBtn = new QPushButton;
    nextBtn->setFixedSize(18,18);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setToolTip(tr("Next"));

    volumeBtn = new QPushButton(this);
    volumeBtn->setFixedSize(36, 36);
    volumeBtn->setCursor(Qt::PointingHandCursor);
    volumeBtn->setToolTip(tr("Volume"));    //音量
    int volume = playController::getInstance().getVolume();
    if(volume == 0) {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-muted-symbolic"));
        volumeBtn->setProperty("isWindowButton", 0x1);
        volumeBtn->setProperty("useIconHighlightEffect", 0x2);
        volumeBtn->setFlat(true);
    } else if(volume > 0 && volume <= 33) {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-low-symbolic"));
        volumeBtn->setProperty("isWindowButton", 0x1);
        volumeBtn->setProperty("useIconHighlightEffect", 0x2);
        volumeBtn->setFlat(true);
    } else if(volume > 33 && volume <= 77) {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-medium-symbolic"));
        volumeBtn->setProperty("isWindowButton", 0x1);
        volumeBtn->setProperty("useIconHighlightEffect", 0x2);
        volumeBtn->setFlat(true);
    } else {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-high-symbolic"));
        volumeBtn->setProperty("isWindowButton", 0x1);
        volumeBtn->setProperty("useIconHighlightEffect", 0x2);
        volumeBtn->setFlat(true);
    }

    m_volSliderWid = new SliderWidget((QWidget *)parent());

    MotifWmHints hint;
    hint.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hint.functions = MWM_FUNC_ALL;
    hint.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(m_volSliderWid->winId(), hint);

    moveVolSliderWid();
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

    // 收藏
    favBtn = new QPushButton;
    favBtn->setFixedSize(36, 36);
    favBtn->setCursor(Qt::PointingHandCursor);
    favBtn->setFocusPolicy(Qt::NoFocus);
    favBtn->setToolTip(tr("Favourite"));
    favBtn->setProperty("isWindowButton", 0x1);
    favBtn->setProperty("useIconHighlightEffect", 0x2);
    favBtn->setFlat(true);

    // 播放模式
    m_orderBtn = new QPushButton;
    m_orderBtn->setFixedSize(36, 36);
    m_orderBtn->setCursor(Qt::PointingHandCursor);

    switch (playController::getInstance().mode()) {
    case 1:
        m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one-symbolic"));
        m_orderBtn->setProperty("isWindowButton", 0x1);
        m_orderBtn->setProperty("useIconHighlightEffect", 0x2);
        m_orderBtn->setFlat(true);
        m_orderBtn->setToolTip(tr("CurrentItemInLoop"));
        playController::getInstance().setPlaymode(playController::CurrentItemInLoop);
        break;
    case 3:
        m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-symbolic"));
        m_orderBtn->setProperty("isWindowButton", 0x1);
        m_orderBtn->setProperty("useIconHighlightEffect", 0x2);
        m_orderBtn->setFlat(true);
        m_orderBtn->setToolTip(tr("Loop"));
        playController::getInstance().setPlaymode(playController::Loop);
        break;
    case 4:
        m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle-symbolic"));
        m_orderBtn->setProperty("isWindowButton", 0x1);
        m_orderBtn->setProperty("useIconHighlightEffect", 0x2);
        m_orderBtn->setFlat(true);
        m_orderBtn->setToolTip(tr("Random"));
        playController::getInstance().setPlaymode(playController::Random);
        break;
    default:
        break;
    }

    //历史播放列表
    listBtn = new QPushButton;
    listBtn->setFixedSize(36, 36);
//    listBtn->setCheckable(true); //按钮是否是可点击状态，默认不点击 fix: #114350, 历史播放列表图标点击弹出列表后是没有焦点区域
//    listBtn->setChecked(false);    //只可检查按钮是否是点击状态，保存点击的状态
    listBtn->setCursor(Qt::PointingHandCursor);
    listBtn->setIcon(QIcon::fromTheme("ukui-play-list-symbolic"));
    listBtn->setProperty("isWindowButton", 0x1);
    listBtn->setProperty("useIconHighlightEffect", 0x2);
    listBtn->setFlat(true);
    listBtn->setToolTip(tr("Play List"));

    coverPhotoLabel = new QLabel(this);
    coverPhotoLabel->setFixedSize(40,40);

    playingLabel = new CustomLabel(this);
    playingLabel->setFixedHeight(28);
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
    vWidget->setFixedHeight(44);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addStretch();
    vLayout->addWidget(playingLabel,0,Qt::AlignVCenter);
//    vLayout->addSpacing(8);
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
    rightLayout->setSpacing(0);
    rightLayout->addStretch(0);
    rightLayout->addWidget(volumeBtn,Qt::AlignRight);
    rightLayout->addSpacing(2);
    rightLayout->addWidget(favBtn,Qt::AlignRight);
    rightLayout->addSpacing(2);
    rightLayout->addWidget(m_orderBtn,Qt::AlignRight);
    rightLayout->addSpacing(2);
    rightLayout->addWidget(listBtn,Qt::AlignRight);
    rightLayout->setContentsMargins(0,14,30,14);
    rightLayout->setMargin(0);

    letfWid->setLayout(leftLayout);
    centreWid->setLayout(playLayout);
    rightWid->setLayout(rightLayout);

    m_hmainLayout->addWidget(hSlider);
    m_hmainLayout->setMargin(0);
    m_hmainLayout->setSpacing(0);

    m_mainLayout->addWidget(letfWid);
    m_mainLayout->addWidget(centreWid);
    m_mainLayout->addWidget(rightWid);
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
//    QFont sizeFont;
//    sizeFont.setPixelSize(14);
//    playingLabel->setFont(sizeFont);
//    timeLabel->setFont(sizeFont);
}

void PlaySongArea::slotLableSetFontSize(int size)
{
    //默认大小12px,换算成pt为9
    double lableBaseFontSize = PT_9;//魔鬼数字，自行处理
    double nowFontSize = lableBaseFontSize * double(size) / 11;//11为系统默认大小，魔鬼数字，自行处理
    QFont font;
    font.setPointSizeF(nowFontSize);
    timeLabel->setFont(font);
}

void PlaySongArea::initConnect()
{
    connect(preBtn,&QPushButton::clicked,this,&PlaySongArea::slotPrevious);
    connect(playBtn,&QPushButton::clicked,this,&PlaySongArea::slotPlayClicked);
    connect(nextBtn,&QPushButton::clicked,this,&PlaySongArea::slotNext);
    connect(listBtn,&QPushButton::clicked,this,&PlaySongArea::listBtnClicked);
    connect(m_volSliderWid->vSlider,&QSlider::valueChanged,this,&PlaySongArea::slotVolumeChanged);
    connect(volumeBtn,&QPushButton::clicked,this,&PlaySongArea::slotVolSliderWidget);
    connect(favBtn,&QPushButton::clicked,this,&PlaySongArea::slotFav);
    connect(m_orderBtn,&QPushButton::clicked,this,&PlaySongArea::slotPlayModeClicked);
    connect(&playController::getInstance(),&playController::singalChangePath,this,&PlaySongArea::slotSongInfo);
    connect(&playController::getInstance(),&playController::playerStateChange,this,&PlaySongArea::playerStateChange);
    connect(playController::getInstance().getPlayer(),SIGNAL(positionChanged(qint64)),this,SLOT(slotPositionChanged(qint64)));
    connect(playController::getInstance().getPlayer(),SIGNAL(durationChanged(qint64)),this,SLOT(slotDurationChanged(qint64)));
    connect(hSlider,SIGNAL(sliderPressed()),this,SLOT(slotSlidePressd()));
    connect(hSlider,SIGNAL(sliderReleased()),this,SLOT(slotSlideReleased()));
    connect(hSlider,&MusicSlider::valueChanged,this,&PlaySongArea::setPosition);
    connect(&playController::getInstance(),&playController::signalPlayMode,this,&PlaySongArea::setPlayMode);
    connect(&playController::getInstance(),&playController::signalNotPlaying,this,&PlaySongArea::slotNotPlaying);
    connect(&playController::getInstance(),&playController::signalSetValue,this,&PlaySongArea::slotSetValue);
    connect(&playController::getInstance(),&playController::signalVolume,this,&PlaySongArea::slotVolume);
    connect(&playController::getInstance(),&playController::signalMute,this,&PlaySongArea::slotMute);
}

void PlaySongArea::slotVolumeChanged(int values)
{
    playController::getInstance().setVolume(values);
    int volume = playController::getInstance().getVolume();
    KyInfo() << "volume = " << volume;

    if(volume == 0) {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-muted-symbolic"));
    } else if(volume > 0 && volume <= 33) {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-low-symbolic"));
    } else if(volume > 33 && volume <= 77) {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-medium-symbolic"));
    } else {
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
    if(m_volSliderWid->isVisible()) {
        m_volSliderWid->hide();
    } else {
        moveVolSliderWid();
        m_volSliderWid->show();
        m_volSliderWid->raise();
    }
}

void PlaySongArea::slotVolume(int volume)
{
    m_volSliderWid->vSlider->setValue(volume);
}

void PlaySongArea::slotMute(bool mute)
{
    if (mute) {
        volumeBtn->setIcon(QIcon::fromTheme("audio-volume-muted-symbolic"));
    }
}

void PlaySongArea::slotText(QString btnText)
{
    listName = btnText;
}

void PlaySongArea::slotFav()
{
    if(g_db->checkSongIsInFav(filePath)) {
        QList<musicDataStruct> resList;
        int ref = g_db->getSongInfoListFromDB(resList, FAV);
        int ret = g_db->delMusicFromPlayList(filePath,FAV);

        if(ref == DB_OP_SUCC && ret == DB_OP_SUCC) {
            for(int i = 0; i < resList.size(); i++) {
                if(resList.at(i).filepath == filePath) {
                    playController::getInstance().removeSongFromCurList(FAV, i);

                    //刷新我喜欢界面
                    if(listName == tr("I Love")) {
                        Q_EMIT signalRefreshFav(FAV);
                    }
                    break;
                }
            }
        }
    } else {
        int ref = g_db->addMusicToPlayList(filePath,FAV);

        if(ref == DB_OP_SUCC) {
            playController::getInstance().addSongToCurList(FAV, filePath);
            if(listName == tr("I Love")) {
                Q_EMIT signalRefreshFav(FAV);
            }
        }
    }

    slotFavExixts();
}

void PlaySongArea::slotPlayModeClicked()
{
    int playMode = playController::getInstance().playmode();
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
    playController::getInstance().setMode(static_cast<playController::PlayMode>(playMode));

}


void PlaySongArea::setPlayMode(int playModel)
{
    switch (playModel) {
    case 1:
        m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one-symbolic"));
        m_orderBtn->setToolTip(tr("CurrentItemInLoop"));
        playController::getInstance().setPlaymode(playController::CurrentItemInLoop);
        break;
    case 3:
        m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-symbolic"));
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

    playController::getInstance().setMode(static_cast<playController::PlayMode>(playModel));
}

void PlaySongArea::slotSongInfo(QString path)
{
    musicDataStruct musicStruct;
    if(!path.isEmpty()) {
        filePath = path.remove("file://");
        g_db->getSongInfoFromDB(filePath, musicStruct);
    }
    //使用库解析总时间
    m_time = musicStruct.time;
    if(musicStruct.title == "")
    {
        playingLabel->setText(tr("Music Player"));
        coverPhotoLabel->setPixmap(QIcon(":/img/fengmian.png").pixmap(QSize(40,40)));
        Q_EMIT signalPlayingLab(tr("Music Player"));
    }
    else
    {
        hSlider->isPlaying(true);
        playingLabel->setText(musicStruct.title);
        QPixmap pix = MusicFileInformation::getInstance().getCoverPhotoPixmap(filePath);
        setCoverPhotoPixmap(pix);
        Q_EMIT signalPlayingLab(musicStruct.title);
    }
    slotFavExixts();
}

/**
 * @brief PlaySongArea::setCoverPhotoPixmapRadius 修改图片，形成圆角
 * @param pixmap
 * @return
 */
QPixmap PlaySongArea::setCoverPhotoPixmapRadius(QPixmap pixmap)
{
    int width = coverPhotoLabel->width();
    int height = coverPhotoLabel->height();
    QSize size(width, height);

    QBitmap mask(size);

    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 10, 10); //修改这个值，可以改弧度，和直径相等就是圆形

    QPixmap image = pixmap.scaled(size);
    image.setMask(mask);

    return image;
}

void PlaySongArea::setCoverPhotoPixmap(QPixmap pixmap)
{
    if(pixmap.isNull())
    {
        pixmap = QIcon(":/img/fengmian.png").pixmap(QSize(40,40));
    }
    else
    {
        pixmap = setCoverPhotoPixmapRadius(pixmap);
        pixmap = pixmap.scaled(QSize(40,40),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    coverPhotoLabel->setPixmap(pixmap);
}


/**
 * @brief PlaySongArea::playerStateChange 切换播放、暂停按钮样式
 * 使用 background-color: palette(highlight); 来实现主题高亮色切换
 * QApplication::palette().highlight().color() 也是实现主题高亮色切换的方式
 * @param newState
 */
void PlaySongArea::playerStateChange(playController::PlayState newState)
{
    QString playDefaultIcon(":/img/default/media-playback-start-symbolic.svg");
    QString pauseDefaultIcon(":/img/default/media-playback-pause-symbolic.svg");

    // 设置按钮高亮色的鼠标划过透明度
    QColor highlightColor(QApplication::palette().highlight().color());
    qreal r,g,b,a;
    r = highlightColor.redF();
    g = highlightColor.greenF();
    b = highlightColor.blueF();
    a = 0.8;
    QColor color = QColor::fromRgbF(r, g, b, a);
    QString hoverColor = QString("rgba(%1, %2, %3, %4)").arg(color.red()) .arg(color.green()) .arg(color.blue()) .arg(color.alpha());

    if(newState == playController::PlayState::PLAY_STATE) {
        // 这两个图标一样大小，都是QSize(12, 16)，但不清楚 media-playback-start-symbolic.svg 会变形，因此可以通过QPixmap设置大小，而不是直接setStyleSheet设置背景
        QPixmap pixmap(pauseDefaultIcon);
        QPixmap fitpixmap = pixmap.scaled(30, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        playBtn->setIcon(QIcon(fitpixmap));
        playBtn->setIconSize(QSize(30, 30));
        playBtn->setToolTip(tr("Play"));
        playBtn->setStyleSheet(QString("QPushButton{background-color: palette(highlight); border-radius:17px;}"
                                       "QPushButton::hover{background-color: %1;}"
                                       "QPushButton::pressed{background-color: palette(highlight);}").arg(hoverColor));
    } else if((newState == playController::PlayState::PAUSED_STATE)
              ||(newState == playController::PlayState::STOP_STATE)) {
        QPixmap pixmap(playDefaultIcon);
        QPixmap fitpixmap = pixmap.scaled(12 ,16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        playBtn->setIcon(QIcon(fitpixmap));
        playBtn->setIconSize(QSize(12, 16));
        playBtn->setToolTip(tr("Pause"));
        playBtn->setStyleSheet(QString("QPushButton{background-color: palette(highlight); border-radius:17px;}"
                                       "QPushButton::hover{background-color: %1;}"
                                       "QPushButton::pressed{background-color: palette(highlight);}").arg(hoverColor));
    }
}

void PlaySongArea::slotPositionChanged(qint64 position)
{
    QString str_time;
    int pos = position / 1000;
    int hour = static_cast<int>(pos / 3600);
    int minutes = static_cast<int>(pos % 3600 / 60);
    int seconds = static_cast<int>(pos % 60);

    QTime duration(hour, minutes, seconds);
    QStringList s = m_time.split(":");
    if(s.size() == 3) {
        str_time = duration.toString("hh:mm:ss");
    }
    else {
        str_time = duration.toString("mm:ss");
    }

    QString length = str_time + "/" + m_time;
    if(m_time == "")
    {
        timeLabel->setText("00:00/00:00");
        Q_EMIT signalTimeLab("00:00/00:00");
    }
    else
    {
        timeLabel->setText(length);
        Q_EMIT signalTimeLab(length);
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
    hSlider->setValue(0);
    playingLabel->setText(tr("Music Player"));
    timeLabel->setText("00:00/00:00");
}

void PlaySongArea::slotSetValue()
{
    hSlider->setValue(0);
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
    playingLabel->setFixedWidth(this->width()/3.6);
    QWidget::resizeEvent(event);
}

void PlaySongArea::moveVolSliderWid()
{
    QPoint volumePos = volumeBtn->mapToGlobal(volumeBtn->rect().center());
    qDebug() << "volumePos: " << volumePos;

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


void PlaySongArea::slotPlayClicked()
{
    KyInfo() << QString("%1 %2")
                .arg("slotPlayCliked: ")
                .arg(playController::getInstance().getPlayer()->state());

    if(playController::getInstance().getPlayer()->state() == MMediaPlayer::PlayingState) {
        playController::getInstance().getPlayer()->pause();
    } else if(playController::getInstance().getPlayer()->state() == MMediaPlayer::PausedState) {
        playController::getInstance().getPlayer()->play();

        int currentVolume = playController::getInstance().getVolume();
        KyInfo() << "currentVolume = " << currentVolume
                 << "playState = " << playController::getInstance().getPlayer()->state();

        delayMsecond(100);
        playController::getInstance().setVolume(currentVolume);
    } else if(playController::getInstance().getPlayer()->state() == MMediaPlayer::StoppedState) {
        playMeta();

        int currentVolume = playController::getInstance().getVolume();
        KyInfo() << "currentVolume = " << currentVolume
                 << "playState = " << playController::getInstance().getPlayer()->state();

        delayMsecond(100);
        playController::getInstance().setVolume(currentVolume);
    }
}

void PlaySongArea::playMeta()
{
    QString playListName = playController::getInstance().getPlayListName();
    QString playPath = playController::getInstance().getPath();
    QList<musicDataStruct> resList;
    QStringList filePaths;

    int ret = g_db->getSongInfoListFromDB(resList, playListName);

    if(playPath != "") {
        if(ret == DB_OP_SUCC) {
            if(resList.size() == 0) {
                return;
            }

            int index;
            for(int i = 0; i < resList.size(); i++) {
                filePaths << resList.at(i).filepath;
                if(resList.at(i).filepath == playPath) {
                    index = i;
                }
            }

            playController::getInstance().setCurPlaylist(playListName, filePaths);
            playController::getInstance().play(playListName, index);
        }
    } else {
        QList<musicDataStruct> resMusicList;
        int ref = g_db->getSongInfoListFromDB(resMusicList,ALLMUSIC);

        if(ref == DB_OP_SUCC) {
            if(resMusicList.size() == 0) {
                return;
            }

            if(resMusicList.size() >= 1) {
                QStringList paths;

                for(int i = 0; i < resMusicList.size(); i++) {
                    paths << resMusicList.at(i).filepath;
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

void PlaySongArea::delayMsecond(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void PlaySongArea::slotNext()
{
    playController::getInstance().onNextSong();
}

void PlaySongArea::listBtnClicked()
{
    Q_EMIT showHistoryListBtnClicked();
}

void PlaySongArea::slotFavExixts()
{
    if (g_db->checkSongIsInFav(filePath)) {
        favBtn->setIcon(QIcon::fromTheme("favorite-new-symbolic"));
        favBtn->setProperty("useIconHighlightEffect", 0x4);
    } else {
        favBtn->setIcon(QIcon::fromTheme("ukui-love-symbolic"));
        favBtn->setProperty("useIconHighlightEffect", 0x2);
    }

    Q_EMIT signalFavBtnChange(filePath);
}

void PlaySongArea::slotFavExixtsDark()
{
    if(g_db->checkSongIsInFav(filePath)) {
        favBtn->setIcon(QIcon::fromTheme("favorite-new-symbolic"));
        favBtn->setProperty("useIconHighlightEffect", 0x4);
    } else {
        favBtn->setIcon(QIcon::fromTheme("ukui-love-symbolic"));
        favBtn->setProperty("useIconHighlightEffect", 0x2);
    }

    Q_EMIT signalFavBtnChange(filePath);
}
void PlaySongArea::slotHistoryBtnChecked(bool checked)
{
    listBtn->setChecked(checked);
}
void PlaySongArea::slotFavIsExixts(QString filePaths)
{
    if(g_db->checkSongIsInFav(filePaths)) {
        if(playingLabel->text() == tr("Music Player")) {
            return;
        }

        //用于判断添加到我喜欢的歌曲是否是当前播放的歌曲， 如果是刷新， 如果不是不做操作
        if(filePath != filePaths) {
            return;
        }

        favBtn->setIcon(QIcon::fromTheme("favorite-new-symbolic"));
        favBtn->setProperty("useIconHighlightEffect", 0x4);
    } else {
        //用于判断添加到我喜欢的歌曲是否是当前播放的歌曲， 如果是刷新， 如果不是不做操作
        if(filePath != filePaths) {
            return;
        }

        favBtn->setIcon(QIcon::fromTheme("ukui-love-symbolic"));
        favBtn->setProperty("useIconHighlightEffect", 0x2);
    }
}

void PlaySongArea::slotFavBtnChange(QString filePath)
{
    if(g_db->checkSongIsInFav(filePath))
    {
        if(playingLabel->text() == tr("Music Player"))
        {
            return;
        }
        favBtn->setIcon(QIcon::fromTheme("favorite-new-symbolic"));
        favBtn->setProperty("useIconHighlightEffect", 0x4);
    }
    else
    {
        favBtn->setIcon(QIcon::fromTheme("ukui-love-symbolic"));
        favBtn->setProperty("useIconHighlightEffect", 0x2);
    }
}

void PlaySongArea::initStyle()
{
    playerStateChange(playController::getInstance().getState());

    if(WidgetStyle::themeColor == 1) {
        this->setStyleSheet("#PlaySongArea{background-color:#1d1d1d;}");
        preBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/lastsong.png);}"
                              "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");


        nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

        slotFavExixtsDark();

        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        playingLabel->setStyleSheet("color:#F9F9F9;line-height:14px;");

        hSlider->initStyle();
    } else if(WidgetStyle::themeColor == 0) {
        this->setStyleSheet("#PlaySongArea{background-color:#FFFFFF;}");
        preBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/lastsong.png);}"
                              "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");


        nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

        slotFavExixts();

        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        playingLabel->setStyleSheet("line-height:14px;color:#303133;");

        timeLabel->setStyleSheet("line-height:12px;color:#8F9399;");

        hSlider->initStyle();

    }
}
