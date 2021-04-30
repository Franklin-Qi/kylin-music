#ifndef PLAYSONGAREA_H
#define PLAYSONGAREA_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVariant>
#include <QShortcut>

#include "sliderwidget.h"
#include "playbackmodewidget.h"
#include "musicslider.h"
#include "UIControl/player/player.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UI/tableview/tableone.h"
#include "UI/base/mylabel.h"

class PlaySongArea : public QWidget
{
    Q_OBJECT
public:
    explicit PlaySongArea(QWidget *parent = nullptr);
public:
     void playcolor();
public:
      SliderWidget *m_volSliderWid;
      PlayBackModeWidget *m_playBackModeWid;
signals:
    void showHistoryListBtnClicked();
    //发送信号播放区我喜欢按钮改变
    void signalFavBtnChange(QString filePath);
    //正在播放title
    void signalPlayingLab(QString playing);
    //正在播放时长
    void signalTimeLab(QString time);
//    //按钮添加到我喜欢刷新界面
//    void signalAddFromFavButton(QString listName);
//    //按钮从我喜欢删除刷新界面
//    void signalDelFromFavButton(QString listName);
public slots:
    //上一首
    void slotPrevious();
    //播放/暂停
    void slotPlayClicked();
    //下一首
    void slotNext();
    //播放通过路径判断我喜欢是否存在此歌曲
    void slotFavExixts();
    //深色模式图标样式
    void slotFavExixtsDark();
    //动态添加判断我喜欢是否存在
    void slotFavIsExixts(QString filePath);
    //显示音量界面
    void slotVolSliderWidget();
//    void increaseVolume();   //音量调高
//    void reduceVolume();     //音量调低
    //改变音量
    void slotVolumeChanged(int values);
    //我喜欢按钮
    void slotFav();
    //改变播放模式弹窗
    void slotPlayBackModeChanged();

    void slotLoopClicked();
    void slotRandomClicked();
    void slotSequentialClicked();
    void slotCurrentItemInLoopClicked();

    void setPlayMode(int playModel);
    //获取当前播放歌曲的路径
    void slotSongInfo(QString path);
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
protected:
    void resizeEvent(QResizeEvent *event)override;
private:
    void initWidget();
    void initConnect();
    //移动音量
    void moveVolSliderWid();
    //计算新的播放模式
    void movePlayModeWid();

    void listBtnClicked();
private:

    QHBoxLayout *m_mainLayout;
    //上一首
    QPushButton *preBtn;
    //播放/暂停
    QPushButton *playBtn;
    //下一首
    QPushButton *nextBtn;
    //音量
    QPushButton *volumeBtn;
    //我喜欢
    QPushButton *favBtn;
    //播放模式
    QPushButton *playModeBtn;
    //歌词
    QPushButton *lyricBtn;
    //播放列表
    QPushButton *listBtn;
    //封面
    QLabel *coverPhotoLabel;
    //正在播放
    MyLabel *playingLabel;
    //时长
    QLabel *timeLabel;
    //播放滑动条
    MusicSlider *hSlider;
    //保存库解析总时间
    QString m_time;
    //路径
    QString filePath;
};

#endif // PLAYSONGAREA_H
