#include <QDebug>
#include "player.h"
#include "UIControl/base/musicDataBase.h"
#include "UIControl/base/musicfileinformation.h"
#include "UI/player/playsongarea.h"
#include "UI/base/widgetstyle.h"

bool playController::play(QString playlist, int index)
{
    if (playlist.compare(m_curList)==0)
    {
//        if (index == m_curIndex) {
//            if (m_player->state() == MMediaPlayer::State::PlayingState){
//                play();
//            }else {
//                pause();
//            }
//        }
//        else {
            stop();
            setSongIndex(index);
            play();
//        }
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
    if (m_player->state() == MMediaPlayer::State::PlayingState) {
        m_player->pause();
    } else {
        m_player->play();
        emit curIndexChanged(m_playlist->currentIndex());
    }

    return true;
}
bool playController::pause()
{
    if (m_player == nullptr) {
        return false;
    }
    m_player->pause();

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
    if(index < 0)
    {
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

    return static_cast<MMediaPlaylist::PlaybackMode>(m_playlist->playbackMode());
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
//        m_playlist->setPlaybackMode(MMediaPlaylist::PlaybackMode::CurrentItemOnce);
//    } else
        m_playlist->setPlaybackMode(static_cast<MMediaPlaylist::PlaybackMode>(mode));
}

void playController::curPlaylist()
{
    // Print Playlist information
    if (m_playlist == nullptr) {
        return;
    }

    for (auto i = 0; i < m_playlist->mediaCount(); i++) {
        MMediaContent content = m_playlist->media(i);
//        qDebug() << "   "
//                 << "media[" << i << "] is:" << content.canonicalUrl();
    }
}
void playController::setCurPlaylist(QString name, QStringList songPaths)
{
    if (m_curList.compare(name)==0)
    {
        qDebug() << "setCurPlaylist m_curList.compare(name)==0" << m_curList << name;
//        return ;
    }
    if (m_playlist == nullptr || m_player == nullptr) {
        return;
    }
    disconnect(m_playlist,&MMediaPlaylist::currentIndexChanged,this,&playController::slotIndexChange);
    m_curList = name;
    m_playlist->clear();

    for (auto path : songPaths) {
        m_playlist->addMedia(QUrl::fromLocalFile(path));
    }
    m_player->stop();
    m_player->setPlaylist(nullptr);
    m_player->setPlaylist(m_playlist);


    m_playlist->setCurrentIndex(-1);
    connect(m_playlist,&MMediaPlaylist::currentIndexChanged,this,&playController::slotIndexChange);
    isInitialed = true;
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
    if (name.compare(m_curList) != 0)
    {
        qDebug() << __FUNCTION__ << " the playlist to add is not Current playlist.";
        return;
    }
    if (m_playlist != nullptr) {
//        m_playlist->removeMedia(index);
        //判断删除后 播放歌曲的index    当前只判断了删除正在播放的歌曲    还没做删除正在播放之前的歌曲和之后的歌曲
            qDebug() << "m_playlist->currentIndex();" << m_playlist->currentIndex();
            int count = m_playlist->mediaCount();

            if(m_curIndex == index)
            {
                stop();
                if(m_curIndex == count - 1)
                {
                    m_curIndex = 0;
                    m_playlist->removeMedia(index);
                    if(m_playlist->mediaCount() == 0)
                    {
                        m_curIndex = -1;
                    }
                    setSongIndex(m_curIndex);
                }
                else
                {
                    m_playlist->removeMedia(index);
                    if(m_playlist->mediaCount() == 0)
                    {
                        m_curIndex = -1;
                    }
//                    setSongIndex(m_curIndex);
                    setSongIndex(m_curIndex);
                }
                m_player->play();
            }
            else if(m_curIndex > index)
            {
//                int position = 0;
//                if(m_player->state()==MMediaPlayer::PlayingState)
//                {
//                    position = m_player->position();
//                }
//                m_player->stop();
                m_playlist->removeMedia(index);
                m_curIndex = m_curIndex - 1;
                //只负责高亮歌曲
                setSongIndex(m_curIndex);
//                m_player->setPosition(position);
                //歌曲删除重新播放(跨函数调用)
//                PlaySongArea::getInstance().hSlider->setValue(position);
                //MPV setPosition已经设置播放，不用再次设置播放
//                m_player->play();
            }
            else if(m_curIndex < index)
            {
                m_playlist->removeMedia(index);
            }

            emit curIndexChanged(m_curIndex);
            emit currentIndexAndCurrentList(m_curIndex,m_curList);
            slotIndexChange(m_curIndex);
//        auto cr_index = m_playlist->currentIndex();
//        emit curIndexChanged(cr_index);
//        emit currentIndexAndCurrentList(cr_index,m_curList);
        //删除正在播放的歌曲时，正在播放的歌曲名和时长实时更新
//        slotIndexChange(cr_index);
//        if(name == ALLMUSIC)
//        {

//        }
    }
}


void playController::removeSongFromLocalList(QString name, int index)
{
    if (name.compare(m_curList) != 0)
    {
        qDebug() << __FUNCTION__ << " the playlist to add is not Current playlist.";
        return;
    }
    if (m_playlist != nullptr)
    {
            int count = m_playlist->mediaCount();

            if(m_curIndex == index)
            {
                stop();
                if(m_curIndex == count - 1)
                {
                    m_curIndex = 0;
                    m_playlist->removeMedia(index);
                    if(m_playlist->mediaCount() == 0)
                    {
                        m_curIndex = -1;
                    }
                    setSongIndex(m_curIndex);
                }
                else
                {
                    m_playlist->removeMedia(index);
                    if(m_playlist->mediaCount() == 0)
                    {
                        m_curIndex = -1;
                    }
                    setSongIndex(m_curIndex);
                }
                m_player->play();
            }
            else if(m_curIndex > index)
            {
                int position = 0;
                if(m_player->state()==MMediaPlayer::PlayingState)
                {
                    position = m_player->position();
                }
                m_player->stop();
                m_playlist->removeMedia(index);
                m_curIndex = m_curIndex - 1;
                setSongIndex(m_curIndex);
                m_player->setPosition(position);
                m_player->play();
            }
            else if(m_curIndex < index)
            {
                m_playlist->removeMedia(index);
            }

            emit curIndexChanged(m_curIndex);
            emit currentIndexAndCurrentList(m_curIndex,m_curList);
            slotIndexChange(m_curIndex);
    }
}

playController::PlayState playController::getState()
{
    if(m_player->state() == MMediaPlayer::State::PlayingState)
        return PlayState::PLAY_STATE;
    else if(m_player->state() == MMediaPlayer::State::PausedState)
        return PlayState::PAUSED_STATE;
    else if(m_player->state() == MMediaPlayer::State::StoppedState)
        return PlayState::STOP_STATE;
}

playController::playController()
    : m_curList(""),m_curIndex(-1)
{
    m_player = new MMediaPlayer(this);
    if (m_player == nullptr) {
        qDebug() << "failed to create player ";
        return;
    }
    m_playlist = new MMediaPlaylist(m_player);
    if (m_playlist == nullptr) {
        qDebug() << "failed to create laylist";
        return;
    }
    m_player->setPlaylist(m_playlist);
    init();
    m_playlist->setPlaybackMode(MMediaPlaylist::Loop);
    m_playlist->setCurrentIndex(-1);
    connect(m_playlist,&MMediaPlaylist::currentIndexChanged,this,&playController::slotIndexChange);
    connect(m_player,&MMediaPlayer::stateChanged,this,&playController::slotStateChanged);
    connect(m_playlist,&MMediaPlaylist::playbackModeChanged,this,&playController::slotPlayModeChange);
}

void playController::init()
{
    volumeSetting = new QGSettings(KYLINMUSIC);
    m_volume = volumeSetting->get("volume").toInt();
    m_player->setVolume(m_volume);
    playSetting = new QGSettings(KYLINMUSIC);
    m_playListName = playSetting->get("playlistname").toString();
    m_curList = m_playListName;
}

int playController::getVolume()
{
    return m_volume;
}

void playController::setVolume(int volume)
{
    if(volume > 100) {
        volume = 100;
    }
    if(volume < 0) {
        volume = 0;
    }

    m_volume = volume;
    volumeSetting->set("volume", volume);
    m_player->setVolume(volume);
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
    m_playlist->next();
    m_player->play();
    curPlaylist();
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

void playController::setCurList(QString renameList)
{
    m_curList = renameList;
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

void playController::slotStateChanged(MMediaPlayer::State newState)
{
    if(newState == MMediaPlayer::State::PlayingState)
        emit playerStateChange(playController::PlayState::PLAY_STATE);
    else if(newState == MMediaPlayer::State::PausedState)
        emit playerStateChange(playController::PlayState::PAUSED_STATE);
    else if(newState == MMediaPlayer::State::StoppedState)
        emit playerStateChange(playController::PlayState::STOP_STATE);

}

void playController::slotPlayModeChange(MMediaPlaylist::PlaybackMode mode)
{
    if(mode == MMediaPlaylist::PlaybackMode::CurrentItemInLoop)
        emit signalPlayMode(static_cast<MMediaPlaylist::PlaybackMode>(playController::PlayMode::CurrentItemInLoop));
    else if(mode == MMediaPlaylist::PlaybackMode::Sequential)
        emit signalPlayMode(static_cast<MMediaPlaylist::PlaybackMode>(playController::PlayMode::Sequential));
    else if(mode == MMediaPlaylist::PlaybackMode::Loop)
        emit signalPlayMode(static_cast<MMediaPlaylist::PlaybackMode>(playController::PlayMode::Loop));
    else if(mode == MMediaPlaylist::PlaybackMode::Random)
        emit signalPlayMode(static_cast<MMediaPlaylist::PlaybackMode>(playController::PlayMode::Random));
}

void playController::slotIndexChange(int index)
{
    if(index == -1)
    {
        emit signalNotPlaying();
        //当index == -1时，会调用positionChanged导致时长显示错误
        emit singalChangePath("");
        m_path = "";
        playSetting->set("playlistname", m_curList);
        playSetting->set("path", "");
        return;
    }
    m_curIndex = index;
    MMediaContent content = m_playlist->media(index);
    QString path = content.canonicalUrl().toString();
    QFileInfo file(path.remove("file://"));
    if(file.exists())
    {
        x = 0;
        emit currentIndexAndCurrentList(index,m_curList);
        emit singalChangePath(path);
        m_path = path;
        playSetting->set("playlistname", m_curList);
        playSetting->set("path", path);
    }
    else
    {
        x++;
        if(x > m_playlist->mediaCount())
        {
            x = 0;
            index = -1;
            m_playlist->setCurrentIndex(index);
            return;
        }
    }
}

void playController::setPosition(int position)
{
    if (qAbs(m_player->position() - position) > 99)
       m_player->setPosition(position);
}

QString playController::getPlayListName()
{
    return playSetting->get("playlistname").toString();
}

QString playController::getPath()
{
    return playSetting->get("path").toString();
}
