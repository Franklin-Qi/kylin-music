#include <QDebug>
#include "player.h"

bool playController::play(QString playlist, int index)
{
    qDebug() << "正在播放"  << playlist << index;
    if (playlist.compare(m_curList)==0) {
        if (index == m_curIndex) {
            if (m_player->state() == QMediaPlayer::State::PlayingState){
                play();
            }else {
                pause();
            }
        }
        else {
            stop();
            setSongIndex(index);
            play();
        }
        return true;
    }
    return false;
}

bool playController::play()
{
    if (m_player == nullptr) {
        return false;
    }
    if (m_player->isAvailable() == false) { //存疑
        return false;
    }
    if (m_player->state() == QMediaPlayer::State::PlayingState) {
        m_player->pause();
    } else {
        m_player->play();
        emit curIndexChanged(m_playlist->currentIndex());
    }

    return true;
}
bool playController::pause()
{
    return true;
}
bool playController::stop()
{
    if (m_player == nullptr) {
        return false;
    }
    m_player->stop();

    return true;
}
void playController::setSongIndex(int index)
{
    if (m_playlist == nullptr) {
        qDebug() << "m_playlist is null";
        return;
    }

    if (index > m_playlist->mediaCount()) {
        return;
    }
    m_playlist->setCurrentIndex(index);
    emit curIndexChanged(index);
}
int playController::songIndex()
{
    if (m_playlist == nullptr) {
        qDebug() << "m_playlist is null";
        return -1;
    }

    return m_playlist->currentIndex();
}

int playController::playmode()
{
    if (m_playlist == nullptr) {
        qDebug() << "m_playlist is null";
        return -1;
    }

    return static_cast<QMediaPlaylist::PlaybackMode>(m_playlist->playbackMode());
}
void playController::setPlaymode(int mode)
{
    if (m_playlist == nullptr || m_player == nullptr) {
        return;
    }
    if (m_playlist == nullptr) {
        qDebug() << "m_playlist is null";
        return;
    }

//    if (mode < 0 || mode > 5) {
//        m_playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::CurrentItemOnce);
//    } else
        m_playlist->setPlaybackMode(static_cast<QMediaPlaylist::PlaybackMode>(mode));
    qDebug() << "mode" << mode;
    qDebug() << "m_playlist" << m_playlist->playbackMode();
}

void playController::curPlaylist()
{
    // Print Playlist information
    qDebug() << "Current playlist name:" << m_curList;
    if (m_playlist == nullptr) {
        return;
    }

    for (auto i = 0; i < m_playlist->mediaCount(); i++) {
        QMediaContent content = m_playlist->media(i);
        qDebug() << "   "
                 << "media[" << i << "] is:" << content.canonicalUrl();
    }
}
void playController::setCurPlaylist(QString name, QStringList songPaths)
{
    qDebug() << "进入函数 setCurPlaylist";
    if (m_curList.compare(name)==0)
    {
        qDebug() << "setCurPlaylist m_curList.compare(name)==0";
        return ;
    }

    if (m_playlist == nullptr || m_player == nullptr) {
        qDebug() << "m_playlist == nullptr || m_player == nullptr";
        return;
    }

    disconnect(m_playlist,&QMediaPlaylist::currentIndexChanged,this,&playController::slotIndexChange);
    m_curList = name;
    m_playlist->clear();

    for (auto path : songPaths) {
        m_playlist->addMedia(QUrl::fromLocalFile(path));
    }

    if (m_player != nullptr) {
        m_player->setPlaylist(m_playlist);
    }
    m_player->stop();
    m_playlist->setCurrentIndex(-1);
    connect(m_playlist,&QMediaPlaylist::currentIndexChanged,this,&playController::slotIndexChange);
    isInitialed = true;
    qDebug() << "完成函数 setCurPlaylist";
}
void playController::addSongToCurList(QString name, QString songPath)
{
    if (name.compare(m_curList) != 0) {
        qDebug() << __FUNCTION__ << " the playlist to add is not Current playlist.";
        return;
    }
    if (m_playlist != nullptr) {
        m_playlist->addMedia(QUrl::fromLocalFile(songPath));
    }
}
void playController::removeSongFromCurList(QString name, int index)
{
    if (name.compare(m_curList) != 0) {
        qDebug() << __FUNCTION__ << " the playlist to add is not Current playlist.";
        return;
    }
    if (m_playlist != nullptr) {
        m_playlist->removeMedia(index);
        //判断删除后 播放歌曲的index    当前只判断了删除正在播放的歌曲    还没做删除正在播放之前的歌曲和之后的歌曲
        auto cr_index = m_playlist->currentIndex();
        emit curIndexChanged(cr_index);
        emit currentIndexAndCurrentList(cr_index,m_curList);
        //删除正在播放的歌曲时，正在播放的歌曲名和时长实时更新
        slotIndexChange(cr_index);
    }
}

playController::PlayState playController::getState()
{
    if(m_player->state() == QMediaPlayer::State::PlayingState)
        return PlayState::PLAY_STATE;
    else if(m_player->state() == QMediaPlayer::State::PausedState)
        return PlayState::PAUSED_STATE;
    else if(m_player->state() == QMediaPlayer::State::StoppedState)
        return PlayState::STOP_STATE;
}

playController::playController()
    : m_curList(""),m_curIndex(-1)
{
    qDebug() << "musicPlayer Create..";
    m_player = new QMediaPlayer(this);
    if (m_player == nullptr) {
        qDebug() << "failed to create player ";
        return;
    }
    m_playlist = new QMediaPlaylist(m_player);
    if (m_playlist == nullptr) {
        qDebug() << "failed to create laylist";
        return;
    }
    m_player->setPlaylist(m_playlist);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_playlist->setCurrentIndex(-1);
    connect(m_playlist,&QMediaPlaylist::currentIndexChanged,this,&playController::slotIndexChange);
    connect(m_player,&QMediaPlayer::stateChanged,this,&playController::slotStateChanged);
    connect(m_playlist,&QMediaPlaylist::playbackModeChanged,this,&playController::slotPlayModeChange);
}
void playController::onCurrentIndexChanged()
{
    qDebug() << "onCurrentIndexChanged";
}
void playController::onPositionChanged(double value)
{
    qDebug() << "onPositionChanged";
    if (m_player == nullptr) {
        return;
    }
    m_player->setPosition(m_player->duration() * value);
}
void playController::onNextSong()
{
    if (m_playlist == nullptr || m_player == nullptr) {
        qDebug() << "m_playlist or m_player is nullptr";
        return;
    }
    qDebug() << " 当前"<< m_playlist->currentIndex();
    m_playlist->next();
    m_player->play();
    curPlaylist();
    qDebug() << " next之后"<< m_playlist->currentIndex() << m_curIndex;
    auto index = m_playlist->currentIndex();
    emit curIndexChanged(index);
}
void playController::onPreviousSong()
{
    if (m_playlist == nullptr || m_player == nullptr) {
        qDebug() << "m_playlist or m_player is nullptr";
        return;
    }
    m_playlist->previous();
    m_player->play();
    auto index = m_playlist->currentIndex();
    emit curIndexChanged(index);
}
void playController::onError()
{
    qDebug() << "onError";
}
void playController::onMediaStatusChanged()
{
    qDebug() << "onMediaStatusChanged";
}
playController::~playController(/* args */)
{
    qDebug() << "musicPlayer destroy..";
    if (m_playlist != nullptr) {
        m_playlist->deleteLater();
    }
    if (m_player != nullptr) {
        m_player->deleteLater();
    }
}

bool playController::playSingleSong(QString Path, bool isPlayNowOrNext)
{

}

void playController::slotStateChanged(QMediaPlayer::State newState)
{
    if(newState == QMediaPlayer::State::PlayingState)
        emit playerStateChange(playController::PlayState::PLAY_STATE);
    else if(newState == QMediaPlayer::State::PausedState)
        emit playerStateChange(playController::PlayState::PAUSED_STATE);
    else if(newState == QMediaPlayer::State::StoppedState)
        emit playerStateChange(playController::PlayState::STOP_STATE);

}

void playController::slotPlayModeChange(QMediaPlaylist::PlaybackMode mode)
{
    if(mode == QMediaPlaylist::PlaybackMode::CurrentItemInLoop)
        emit signalPlayMode(static_cast<QMediaPlaylist::PlaybackMode>(playController::PlayMode::CurrentItemInLoop));
    else if(mode == QMediaPlaylist::PlaybackMode::Sequential)
        emit signalPlayMode(static_cast<QMediaPlaylist::PlaybackMode>(playController::PlayMode::Sequential));
    else if(mode == QMediaPlaylist::PlaybackMode::Loop)
        emit signalPlayMode(static_cast<QMediaPlaylist::PlaybackMode>(playController::PlayMode::Loop));
    else if(mode == QMediaPlaylist::PlaybackMode::Random)
        emit signalPlayMode(static_cast<QMediaPlaylist::PlaybackMode>(playController::PlayMode::Random));
}

void playController::slotIndexChange(int index)
{
    qDebug() << "-----index-----" << index;
    if(index == -1)
    {
        emit signalNotPlaying();
        //当index == -1时，会调用positionChanged导致时长显示错误
        emit singalChangePath("");
        return;
    }
    QMediaContent content = m_playlist->media(index);
    QString path = content.canonicalUrl().toString();
    emit currentIndexAndCurrentList(index,m_curList);
    emit singalChangePath(path);
}

void playController::setPosition(int position)
{
    if (qAbs(m_player->position() - position) > 99)
       m_player->setPosition(position);
}
