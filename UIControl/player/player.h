#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGSettings>

class playController : public QObject {
    Q_OBJECT
public:
    enum PlayState {
        STOP_STATE = 0,
        PLAY_STATE = 1,
        PAUSED_STATE = 2
    };

    enum PlayMode {
        CurrentItemInLoop = 1,
        Sequential = 2,
        Loop = 3,
        Random = 4,
    };
public:
    static playController& getInstance()
    {
        static playController instance;
        return instance;
    }

private:
    playController(/* args */);
    playController(playController const&);
    playController& operator=(playController const&);
    ~playController();
    void init();

public:
    //新增接口 fyf
    bool playSingleSong(QString Path, bool isPlayNowOrNext);

    bool play(QString playlist, int index);

    bool play();
    bool pause();
    bool stop();

    int volume();

    void setPlayPosition(int pos);
    int playPosition();

    void setSongIndex(int index);
    int songIndex();

    int playmode();
    void setPlaymode(int mode);

    void curPlaylist();
    void setCurPlaylist(QString name, QStringList songPaths);
    void addSongToCurList(QString name, QString songPath);
    void removeSongFromCurList(QString name, int index);
//    void removeMedia(QString name, int index);
    PlayState getState();
    PlayMode mode() const;
    QMediaPlayer* getPlayer()
    {
        return m_player;
    }
    QMediaPlaylist* getPlaylist()
    {
        return m_playlist;
    }

    void setPosition(int position);
    //获取音量
    int getVolume();
    //设置音量
    void setVolume(int volume);
signals:
    void curPositionChanged(qint64);
    void curDurationChanged(qint64);
    void curIndexChanged(int index);
    void playerError(int error, QString errMsg);
    void playerStateChange(playController::PlayState newState);
    void singalChangePath(QString path);
    void currentIndexAndCurrentList(int index,QString listname);
    void signalPlayMode(int playModel);

    void signalNotPlaying();
public slots:
    void onCurrentIndexChanged();
    void onPositionChanged(double value);
    void onNextSong();
    void onPreviousSong();
    void setCurList(QString renameList);
private slots:
    void onError();
    void onMediaStatusChanged();
    //状态改变
    void slotStateChanged(QMediaPlayer::State newState);
    //播放模式改变
    void slotPlayModeChange(QMediaPlaylist::PlaybackMode mode);
    //获得当前播放的index
    void slotIndexChange(int index);

private:
    //当前播放列表名
    QString m_curList;
    //当前播放的索引
    int m_curIndex;
    QMediaPlayer* m_player;
    QMediaPlaylist* m_playlist;
    QGSettings *volumeSetting = nullptr;
    bool isInitialed = false;
    //在列表里歌曲（判断本地歌曲是否存在）没有播放的情况下，当前函数掉了多少次，要是歌曲在播放（找到本地路径存在，x重新计数
    int x = 0;
    int m_volume = 50;
};

#endif // PLAYER_H
