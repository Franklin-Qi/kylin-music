#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "UIControl/tableview/musiclistmodel.h"
#include "UIControl/base/musicDataBase.h"

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

public:
    //新增接口 fyf
    bool playSingleSong(QString Path, bool isPlayNowOrNext);

    bool play(QString playlist, int index);

    bool play();
    bool pause();
    bool stop();

    void setVolume(int vol);
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

    PlayMode mode() const;
    QMediaPlayer* getPlayer()
    {
        return m_player;
    }
    QMediaPlaylist* getPlaylist()
    {
        return m_playlist;
    }
signals:
    void curPositionChanged(qint64);
    void curDurationChanged(qint64);
    void curIndexChanged(int index);
    void playerError(int error, QString errMsg);
    void playerStateChange(int state);
    void singalChangePath(QString path);
    void currentIndexAndCurrentList(int index,QString listname);
public slots:
    void onCurrentIndexChanged();
    void onPositionChanged(double value);
    void onNextSong();
    void onPreviousSong();

private slots:
    void onError();
    void onMediaStatusChanged();
    //获得当前播放的index
    void slotIndexChange(int index);
private:
    QString m_curList;
    int m_curIndex;
    QMediaPlayer* m_player;
    QMediaPlaylist* m_playlist;
    bool isInitialed = false;
};

#endif // PLAYER_H
