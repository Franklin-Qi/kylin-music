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
public slots:
    //上一首
    void slotPrevious();
    //下一首
    void slotNext();
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
    //获取当前播放歌曲的路径
    void slotSongInfo(QString path);
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
    QLabel *playingLabel;
    //时长
    QLabel *timeLabel;
};

#endif // PLAYSONGAREA_H
