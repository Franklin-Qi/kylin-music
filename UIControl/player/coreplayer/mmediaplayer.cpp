#include "mmediaplayer.h"

MMediaPlayer::MMediaPlayer(QObject *parent)
    : QObject(parent)
{
    createMvpplayer();
}

void MMediaPlayer::setPlaylist(MMediaPlaylist *playlist)
{
    m_playList = playlist;
    connect(this,&MMediaPlayer::playFinish,m_playList,&MMediaPlaylist::palyFinish,Qt::UniqueConnection);
    connect(m_playList,&MMediaPlaylist::autoPlay,this,&MMediaPlayer::autoPlay,Qt::UniqueConnection);
    connect(m_playList,&MMediaPlaylist::stop,this,&MMediaPlayer::stop,Qt::UniqueConnection);
}

void MMediaPlayer::truePlay(QString startTime)
{
    if (startTime.isEmpty()) {
        return;
    }

    const QByteArray c_filename = m_playList->getPlayFileName().toUtf8();

    if (c_filename == filenameBack && m_positionChangeed == false) {
        if (filenameBack != "") {
            pause();
        }
        return;
    }
    m_position = 0;
    m_positionChangeed = false;
    setProperty("start",startTime);
    const char *args[] = {"loadfile",c_filename, NULL};
    mpv_command_async(m_mpvPlayer, 0, args);

    if (c_filename == "") {
        changeState(StoppedState);
        return;
    }
    filenameBack = c_filename;
    changeState(PlayingState);
}

void MMediaPlayer::play()
{
    truePlay("0");
}

void MMediaPlayer::pause()
{
    // 获得mpv播放器的"暂停"状态
    QString pasued = getProperty("pause");
    // 根据"暂停"状态来选择暂停还是播放
    if(pasued == "no") {
        setProperty("pause", "yes");
        changeState(PausedState);
    } else if(pasued == "yes") {
        setProperty("pause", "no");
        changeState(PlayingState);
    }
}

void MMediaPlayer::stop()
{
    filenameBack = "";
    setProperty("pause", "no");
    const char *args[] = {"loadfile", "", NULL};
    mpv_command_async(m_mpvPlayer, 0, args);
    changeState(StoppedState);
}

MMediaPlayer::State MMediaPlayer::state() const
{
    return m_state;
}

qint64 MMediaPlayer::position() const
{
    return m_position;
}

void MMediaPlayer::setPosition(qint64 pos)
{
    qint64 sec = pos/1000;
    m_positionChangeed = true;
    bool restartPlay = false;
    if (m_state == PausedState) {
        restartPlay = true;
    }
    truePlay(QString::number(sec));
    if (restartPlay) {
        pause();
    }
}

void MMediaPlayer::setVolume(int vol)
{
    setProperty("volume",QString::number(vol));
}

qint64 MMediaPlayer::duration() const
{
    return m_duration;
}

void MMediaPlayer::setMedia(const MMediaContent &media)
{
    QUrl url =media.canonicalUrl();
    if (m_tmpPlayList != nullptr) {
        m_tmpPlayList->deleteLater();
    }
    m_tmpPlayList = new MMediaPlaylist(this);
    m_tmpPlayList->addMedia(url);
    setPlaylist(m_tmpPlayList);
    setProperty("pause", "yes");
    play();
}

bool MMediaPlayer::isAvailable() const
{
    return true;
}


void MMediaPlayer::onMpvEvents()
{
    // 处理所有事件，直到事件队列为空
    while (m_mpvPlayer)
    {
        mpv_event *event = mpv_wait_event(m_mpvPlayer, 0);
        if (event->event_id == MPV_EVENT_NONE)
            break;
        handle_mpv_event(event);
    }
}

void MMediaPlayer::handle_mpv_event(mpv_event *event)
{
    switch (event->event_id) {
        // 属性改变事件发生
        case MPV_EVENT_PROPERTY_CHANGE: {
            mpv_event_property *prop = (mpv_event_property *)event->data;
            if (strcmp(prop->name, "time-pos") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    if (m_state == StoppedState) {
                        changeState(PlayingState);
                    }
                    // 获得播放时间
                    double time = *(double *)prop->data;
                    m_position = time * 1000;//单位换算为毫秒
                    emit positionChanged(m_position);
                } else if (prop->format == MPV_FORMAT_NONE) {
                    //当前时长距离总时长不超过500毫秒判断播放结束
                    if ( m_duration!=0 && (m_duration - m_position < 500)) {
                        m_duration = 0;
                        m_position = 0;
                        //播放结束
                        emit playFinish();
                    } else {
                        //切歌
                        changeState(StoppedState);
                    }
                }
            }
        }
        break;
    case MPV_EVENT_PLAYBACK_RESTART:{
        m_duration = getProperty("duration").toDouble() *1000;//单位换算为毫秒
        emit durationChanged(m_duration);
        //初始化完成事件
    }
        break;
    case MPV_EVENT_END_FILE:{
        //播放完成
        if (m_position != 0) {
            m_duration = 0;
            m_position = 0;
            //播放结束
            emit playFinish();
        }
    }
        break;
        default: ;
    }
}

// 回调函数
static void wakeup(void *ctx)
{
    // 此回调可从任何mpv线程调用（但也可以从调用mpv API的线程递归地返回）
    // 只是需要通知要唤醒的Qt GUI线程（以便它可以使用mpv_wait_event（）），并尽快返回
    MMediaPlayer *mvpPlayer = (MMediaPlayer *)ctx;
    emit mvpPlayer->mpvEvents();
}

void MMediaPlayer::createMvpplayer()
{
    // 创建mpv实例
    setlocale(LC_NUMERIC,"C");
    m_mpvPlayer = mpv_create();
    if (m_mpvPlayer == nullptr) {
        qDebug()<<"创建播放模块失败！";
        this->deleteLater();
        return;
    }
    //禁用视频流
    setProperty("vid", "no");
    //接收事件
    connect(this, &MMediaPlayer::mpvEvents, this, &MMediaPlayer::onMpvEvents,
            Qt::QueuedConnection);
    mpv_set_wakeup_callback(m_mpvPlayer, wakeup, this);
    //绑定事件
    mpv_observe_property(m_mpvPlayer, 0, "time-pos", MPV_FORMAT_DOUBLE);
    // 判断mpv实例是否成功初始化
    if (mpv_initialize(m_mpvPlayer) < 0) {
        qDebug()<<"初始化失败！";
        this->deleteLater();
    }
}

void MMediaPlayer::setProperty(const QString &name, const QString &value)
{
    mpv_set_option_string(m_mpvPlayer, name.toLatin1().data(), value.toLatin1().data());
}

QString MMediaPlayer::getProperty(const QString &name) const
{
    return (QString)mpv_get_property_string(m_mpvPlayer, name.toLatin1().data());
}

void MMediaPlayer::changeState(MMediaPlayer::State stateNow)
{
    m_state = stateNow;
    emit stateChanged(m_state);
}

void MMediaPlayer::autoPlay(MMediaPlaylist::PlaybackMode playbackMode)
{
    if (playbackMode == MMediaPlaylist::PlaybackMode::Sequential) {
        truePlay("");
        return;
    }
    if (playbackMode == MMediaPlaylist::PlaybackMode::CurrentItemInLoop) {
        //播放完毕自动切歌（单曲循环借用播放点改变时间逻辑循环）
        m_positionChangeed = true;
    }
    truePlay("0");
}
