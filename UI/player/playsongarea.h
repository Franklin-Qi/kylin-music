#ifndef PLAYSONGAREA_H
#define PLAYSONGAREA_H

#include <QWidget>
#include <QTimerEvent>
#include <QTimer>
#include <QEventLoop>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QBitmap>
#include <QPainter>
#include <QVariant>
#include <QShortcut>
#include <QMenu>
#include <QDebug>

#include "sliderwidget.h"
#include "playbackmodewidget.h"
#include "musicslider.h"
#include "UIControl/player/player.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UI/tableview/tableone.h"
#include "UI/base/customLabel.h"
#include "UI/player/sliderwidget.h"

class PlaySongArea : public QWidget
{
    Q_OBJECT

public:
    explicit PlaySongArea(QWidget *parent = nullptr);

    SliderWidget *m_volSliderWid = nullptr;
    PlayBackModeWidget *m_playBackModeWid = nullptr;

    QPushButton *listBtn = nullptr; //播放列表

    void playcolor();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

Q_SIGNALS:
    void showHistoryListBtnClicked();
    //发送信号播放区我喜欢按钮改变
    void signalFavBtnChange(QString filePath);
    //正在播放title
    void signalPlayingLab(QString playing);
    //正在播放时长
    void signalTimeLab(QString time);
    //刷新我喜欢界面
    void signalRefreshFav(QString listName);
//    //按钮添加到我喜欢刷新界面
//    void signalAddFromFavButton(QString listName);
//    //按钮从我喜欢删除刷新界面
//    void signalDelFromFavButton(QString listName);

public Q_SLOTS:
    //上一首
    void slotPrevious();
    void delayMsecond(unsigned int msec); // 延时函数，不阻塞线程
    //播放/暂停
    void slotPlayClicked();
    //下一首
    void slotNext();
    //播放通过路径判断我喜欢是否存在此歌曲
    void slotFavExixts();
    //深色模式图标样式
    void slotFavExixtsDark();
    //动态添加判断我喜欢是否存在
    void slotFavIsExixts(QString filePaths);
    //显示音量界面
    void slotVolSliderWidget();
    //音量调高
    void volumeIncrease();
    //音量调低
    void volumeReduce();
    //改变音量
    void slotVolumeChanged(int values);
    //系统音乐音量改变
    void slotVolume(int volume);
    //系统音乐音量是否静音（ mute = true 静音 ）
    void slotMute(bool mute);
    //我喜欢按钮
    void slotFav();
    //改变播放模式弹窗
    void slotPlayBackModeChanged();
    //判断当前点击的按钮是否为我喜欢（歌单名）
    void slotText(QString btnText);

    void slotLoopClicked();
    void slotRandomClicked();
    void slotSequentialClicked();
    void slotCurrentItemInLoopClicked();

    void setPlayMode(int playModel);
    //获取当前播放歌曲的路径
    void slotSongInfo(QString path);
    QPixmap setCoverPhotoPixmapRadius(QPixmap pixmap);
    void setCoverPhotoPixmap(QPixmap pixmap);
    void playerStateChange(playController::PlayState newState);
    //播放滑动条相关
    void slotPositionChanged(qint64 position);
    void slotDurationChanged(qint64 duration);
    void setPosition(int position);
    void slotSlidePressd();
    void slotSlideReleased();
    //接收mini窗口我喜欢按钮改变
    void slotFavBtnChange(QString filePath);

    //没有歌曲在播放
    void slotNotPlaying();
    //设置进度条归 0
    void slotSetValue();
    //播放历史按钮选中状态改变
    void slotHistoryBtnChecked(bool checked);
    //字体
    void slotLableSetFontSize(int size);

private:
    void initWidget();
    void initConnect();
    //移动音量
    void moveVolSliderWid();
    //计算新的播放模式
    void movePlayModeWid();
    //计算新的播放模式菜单
    void movePlayMenu();
    void listBtnClicked();
    void playMeta();

private:

    QHBoxLayout *m_mainLayout = nullptr;

    QPushButton *preBtn = nullptr;  //上一首
    QPushButton *playBtn = nullptr; //播放/暂停
    QPushButton *nextBtn = nullptr; //下一首
    QPushButton *volumeBtn = nullptr; //音量
    QPushButton *favBtn = nullptr; //我喜欢
    QToolButton *playModeBtn = nullptr; //播放模式
    QPushButton *lyricBtn = nullptr; //歌词

    QLabel *coverPhotoLabel = nullptr; //封面
    CustomLabel *playingLabel = nullptr; //正在播放
    QLabel *timeLabel = nullptr; //时长
    MusicSlider *hSlider = nullptr; //播放滑动条
    QString m_time = nullptr; //保存库解析总时间
    QString filePath = nullptr; //路径
    QString listName = nullptr; //歌单名

    QMenu *m_playMode = nullptr; //播放模式菜单
    QAction *loopAct = nullptr;
    QAction *randomAct = nullptr;
    QAction *currentItemInLoopAct = nullptr;
};

#endif // PLAYSONGAREA_H
