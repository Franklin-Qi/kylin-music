#ifndef PLAYSONGAREA_H
#define PLAYSONGAREA_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVariant>
#include <QShortcut>
#include <QMenu>

#include "sliderwidget.h"
#include "playbackmodewidget.h"
#include "musicslider.h"
#include "UIControl/player/player.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UI/tableview/tableone.h"
#include "UI/base/mylabel.h"
#include "UI/player/sliderwidget.h"

class PlaySongArea : public QWidget
{
    Q_OBJECT
public:
    explicit PlaySongArea(QWidget *parent = nullptr);
public:
    static PlaySongArea& getInstance()
    {
        static PlaySongArea instance;
        return instance;
    }
public:
     void playcolor();
public:
      SliderWidget *m_volSliderWid;
      PlayBackModeWidget *m_playBackModeWid;
      //播放列表
      QPushButton *listBtn;
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

    //播放历史按钮选中状态改变
    void slotHistoryBtnChecked(bool checked);
    //字体
    void slotLableSetFontSize(int size);
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
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
    //QPushButton *playModeBtn;
    QToolButton *playModeBtn;
    //歌词
    QPushButton *lyricBtn;

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
    //歌单名
    QString listName;
    //播放模式菜单
    QMenu *m_playMode;
    QAction *loopAct;
    QAction *randomAct;
    QAction *currentItemInLoopAct;
};

#endif // PLAYSONGAREA_H
