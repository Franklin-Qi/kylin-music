#include "mainwidget.h"
#include "UI/base/xatom-helper.h"

Widget *Widget::mutual = nullptr;  //！！！！初始化，非常重要
Widget::Widget(QStringList str, QWidget *parent)
    : QWidget(parent)
{
    mutual = this;//！！！赋值，非常重要
    int res;
    res = g_db->initDataBase();
    if(res != DB_OP_SUCC)
    {
        qDebug() << "数据库加载失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
    }
    qDebug() << "数据库加载成功";
    initAllComponent();
    allConnect();
    //单例
    Single(str);
    //初始化dbus
    initDbus();
    initGSettings();
    if(!argName.isEmpty())
    {
        int num = argName.size();
        switch (num) {
        case 1:
            kylin_music_play_request(argName[0]);
            break;
        default:
            break;
        }
    }
    isFirstObject = false;

}

Widget::~Widget()
{
    m_miniWidget->deleteLater();
}

void Widget::Single(QStringList path)   //单例
{
    QString str;
    if(path.size() > 1)
    {
        str = path[1];
    }
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString lockPath = homePath.at(0) + "/.config/kylin-music-lock";
    int fd = open(lockPath.toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd < 0)
    {
        exit(1);
    }
    if (lockf(fd, F_TLOCK, 0))
    {
        qDebug() << "path.size" << path.size();
        QDBusInterface interface( "org.ukui.kylin_music", "/org/ukui/kylin_music","org.ukui.kylin_music.play", QDBusConnection::sessionBus());
        if(path.size() == 1)
            interface.call( "kylin_music_play_request", str);
        if(path.size() == 2)
            interface.call( "kylin_music_play_request", str);
        else if(path.size() == 4)
            interface.call( "kylin_music_play_request", str, path[2], path[3]);
        exit(0);
    }
    isFirstObject = true;//我是首个对象
    argName << str;
}

void Widget::initDbus()//初始化dbus
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if(sessionBus.registerService("org.ukui.kylin_music"))
    {
        sessionBus.registerObject("/org/ukui/kylin_music",this,
                                  QDBusConnection::ExportAllContents);
        qDebug()<<"初始化DBUS成功";
    }
    else
    {
        qDebug()<<"初始化DBUS失败";
    }
    //S3 S4策略
    QDBusConnection::systemBus().connect(QString("org.freedesktop.login1"),
                                         QString("/org/freedesktop/login1"),
                                         QString("org.freedesktop.login1.Manager"),
                                         QString("PrepareForShutdown"), this,
                                         SLOT(onPrepareForShutdown(bool)));
    QDBusConnection::systemBus().connect(QString("org.freedesktop.login1"),
                                         QString("/org/freedesktop/login1"),
                                         QString("org.freedesktop.login1.Manager"),
                                         QString("PrepareForSleep"), this,
                                         SLOT(onPrepareForSleep(bool)));

    QDBusConnection::systemBus().connect(QString(), QString("/"), "com.monitorkey.interface", "monitorkey", this, SLOT(client_get(QString)));
    QDBusConnection::sessionBus().connect(QString(), QString( "/"), "org.ukui.media", "DbusSingleTest",this, SLOT(inputDevice_get(QString)));
    QDBusConnection::sessionBus().connect(QString("org.gnome.SessionManager"),
                                          QString("/org/gnome/SessionManager"),
                                          QString("org.gnome.SessionManager"),
                                          QString("PrepareForSwitchuser"), this,
                                          SLOT(slotPrepareForSwitchuser()));
}

void Widget::onPrepareForShutdown(bool Shutdown)
{
    //目前只做事件监听，不处理
    qDebug()<<"onPrepareForShutdown"<<Shutdown;
}

void Widget::onPrepareForSleep(bool isSleep)
{
    //990
    //空指针检验
    //------此处空指针校验（如果用了指针）------
    qDebug() << "----睡眠";
    //系统事件
    if(isSleep)
    {
        qDebug() << "睡眠";
        playController::getInstance().pause();
    }
}

void Widget::client_get(QString str)
{
    qDebug()<<"MainWindow:"<<str;
    QString key = str.split(":").first();
    QString s = str.split(":").last();
    qDebug() << "----key----" << key;
    qDebug() << "----s----" << s;
    if(s == "1")
    {
        qDebug() << "----s----" << s;
        playController::getInstance().play();
    }
    else if(s == "2" || key == "163")
    {
        playController::getInstance().onNextSong();
    }
    else if(s == "3" || key == "165")
    {
        playController::getInstance().onPreviousSong();
    }
}

void Widget::inputDevice_get(QString str)
{
    qDebug() << "str" << str;
    playController::getInstance().pause();
}

void Widget::slotPrepareForSwitchuser()
{
    qDebug() << "切换用户";
    playController::getInstance().pause();
}

int Widget::kylin_music_play_request(QString cmd1, QString cmd2, QString cmd3)
{
    //无参数，单例触发
    if(cmd1 == "")
    {
        KWindowSystem::forceActiveWindow(this->winId());
        return 0;
    }
    qDebug() << "---cmd1--" << cmd1;
    qDebug() << "---isFirstObject--" << isFirstObject;
    if(isFirstObject&&!QFileInfo::exists(cmd1))
    {
        qDebug() << "close" << cmd1;
        if(cmd1=="-c"||cmd1=="-close")
        {
            //------窗口关闭------
            this->close();
            exit(0);
        }
        isFirstObject = false;
        return 0;
    }

    //下一首
    if(cmd1 == "-n" || cmd1 == "-next")
    {
        //------下一首-------
        playController::getInstance().onNextSong();
        return 0;
    }
    //上一首
    if(cmd1 == "-b" || cmd1 == "-back")
    {
        //-------上一首------
        playController::getInstance().onPreviousSong();
        return 0;
    }
    //暂停
    if(cmd1 == "-p" || cmd1 == "-pause")
    {
        //------暂停------
        playController::getInstance().play();
        return 0;
    }
    //播放
    if(cmd1 == "-s" || cmd1 == "-start")
    {
        //------播放------
        playController::getInstance().play();
        return 0;
    }
    if(cmd1=="-i"||cmd1=="-increase")
    {
        //------调高音量------
        playSongArea->volumeIncrease();
        return 0;
    }
    if(cmd1=="-r"||cmd1=="-reduce")
    {
        //------减少音量------
        playSongArea->volumeReduce();
        return 0;
    }
    if(cmd1=="-S"||cmd1=="-Sequential")
    {
        //------顺序播放------
        playController::getInstance().getPlaylist()->setPlaybackMode(QMediaPlaylist::Sequential);
        return 0;
    }
    if(cmd1=="-C"||cmd1=="-CurrentItemInLoop")
    {
        //------单曲循环------
        playController::getInstance().getPlaylist()->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        return 0;
    }
    if(cmd1=="-L"||cmd1=="-Loop")
    {
        //------列表循环------
        playController::getInstance().getPlaylist()->setPlaybackMode(QMediaPlaylist::Loop);
        return 0;
    }
    if(cmd1=="-R"||cmd1=="-Random")
    {
        //------随机播放------
        playController::getInstance().getPlaylist()->setPlaybackMode(QMediaPlaylist::Random);
        return 0;
    }
    if(cmd1=="-m"||cmd1=="-move")
    {
        if(cmd2!=""&&cmd3!="")
        {
            //------窗口移动------
            moveWidget(cmd2, cmd3);
            return 0;
        }
    }
    if(cmd1=="-c"||cmd1=="-close")
    {
        //------窗口关闭------
        this->close();
        return 0;
    }
    QStringList list;
    list << cmd1;
    importFile(list);

    return 0;
}

void Widget::importFile(QStringList list)
{
    MusicFileInformation::getInstance().addFile(list);
    QList<musicDataStruct> resList;
    resList = MusicFileInformation::getInstance().resList;
    int ret;

    foreach (const musicDataStruct date, resList)
    {
        ret = g_db->addMusicToLocalMusic(date);
        if(ret == DB_OP_SUCC)
        {
            playController::getInstance().addSongToCurList(ALLMUSIC,date.filepath);
            musicListTable->selectListChanged(tr("Song List"));
            QStringList pathList = getPath(ALLMUSIC);
            playController::getInstance().setCurPlaylist(ALLMUSIC,pathList);
            int index = MusicFileInformation::getInstance().findIndexFromPlayList(ALLMUSIC,date.filepath);
            playController::getInstance().play(ALLMUSIC,index);
        }
    }
    int ref;
    ref = g_db->getSongInfoListFromDB(resList,ALLMUSIC);
    if(ref == DB_OP_SUCC)
    {
        for(int i = 0;i < list.size();i++)
        {
            if(resList.at(i).filepath == list.at(i))
            {
                QStringList pathList = getPath(ALLMUSIC);
                playController::getInstance().setCurPlaylist(ALLMUSIC,pathList);
                int index = MusicFileInformation::getInstance().findIndexFromPlayList(ALLMUSIC,resList.at(i).filepath);
                if(index == -1)
                {
                    return;
                }
                playController::getInstance().play(ALLMUSIC,index);
            }
        }
    }
}

QStringList Widget::getPath(QString playListName)
{
    QStringList path;
    int ret;
    QList<musicDataStruct> resList;
    ret = g_db->getSongInfoListFromDB(resList,playListName);
    foreach (const musicDataStruct date, resList) {
        path.append(date.filepath);
    }
    return path;
}

void Widget::initAllComponent()
{
//    this->setWindowFlag(Qt::FramelessWindowHint);
    setMinimumSize(960,640);
    this->setWindowTitle(tr("Music Player"));
    this->setObjectName("mainWidget");
//    this->setWindowIcon(QIcon(":/img/kylin-music.png"));
    //窗体显示在中间
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    this->move((availableGeometry.width() - this->width())/2, (availableGeometry.height() - this->height())/2);
    mainVBoxLayout = new QVBoxLayout();

//    musicListTable = new TableBaseView();
    musicListTable = new TableOne(ALLMUSIC,this);
    playSongArea = new PlaySongArea(this);
    m_titleBar = new TitleBar(this);

    MotifWmHints hintt;
    hintt.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hintt.functions = MWM_FUNC_ALL;
    hintt.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(this->winId(), hintt);

    m_miniWidget = new miniWidget();
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(m_miniWidget->winId(), hints);

    rightVWidget = new QWidget(this);
    rightVWidget->setLayout(mainVBoxLayout);
    mainVBoxLayout->addWidget(m_titleBar);
    mainVBoxLayout->addSpacing(6);
    mainVBoxLayout->addWidget(musicListTable);
    mainVBoxLayout->addWidget(playSongArea,0,Qt::AlignBottom);
    mainHBoxLayout = new QHBoxLayout();
    sideBarWid = new SideBarWidget(this);
    sideBarWid->setFixedWidth(210);
    mainHBoxLayout->addWidget(sideBarWid);
    mainHBoxLayout->addWidget(rightVWidget);
    mainHBoxLayout->setSpacing(0);
    mainHBoxLayout->setMargin(0);
    mainVBoxLayout->setSpacing(0);
    mainVBoxLayout->setMargin(0);
    this->resize(960,640);
    this->setLayout(mainHBoxLayout);
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);

    historyListTable = new TableHistory(this);
    MotifWmHints hint;
    hint.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hint.functions = MWM_FUNC_ALL;
    hint.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(historyListTable->winId(), hint);
    historyListTable->hide();
}

void Widget::allConnect()
{
    connect(sideBarWid,&SideBarWidget::playListBtnCliced,musicListTable,&TableOne::selectListChanged);
    connect(playSongArea,&PlaySongArea::showHistoryListBtnClicked,historyListTable,&TableHistory::showHistroryPlayList);
    connect(musicListTable,&TableOne::addMusicToHistoryListSignal,historyListTable,&TableHistory::addMusicToHistoryListSlot);
    connect(sideBarWid,&SideBarWidget::playListRenamed,musicListTable,&TableOne::playListRenamed);
    connect(sideBarWid,&SideBarWidget::signalPlayAll,musicListTable,&TableOne::playAll);

    connect(m_titleBar->miniBtn,&QPushButton::clicked,this,&Widget::slotShowMiniWidget);
    connect(m_titleBar->closeBtn,&QPushButton::clicked,this,&Widget::slotClose);
    connect(m_titleBar->minimumBtn,&QPushButton::clicked,this,&Widget::slotShowMinimized);
    connect(m_titleBar->maximumBtn,&QPushButton::clicked,this,&Widget::slotShowMaximized);

    //迷你模式槽函数绑定
    connect(m_miniWidget->m_recoveryWinBtn,&QPushButton::clicked,this,&Widget::slotRecoverNormalWidget);
//    connect(m_miniWidget->m_nextBtn,&QPushButton::clicked,this,&Widget::slotPlayNext);
//    connect(m_miniWidget->m_playStateBtn,&QPushButton::clicked,this,&Widget::slotPlaySong);
//    connect(m_miniWidget->m_preBtn,&QPushButton::clicked,this,&Widget::slotPlayPre);
//    connect(m_miniWidget->m_orderBtn,&QPushButton::clicked,this,&Widget::slotPlayModeChanged);
    connect(m_miniWidget->m_closeBtn,&QPushButton::clicked,this,&Widget::slotCloseMiniWidget);
//    connect(m_miniWidget->m_loveBtn,&QPushButton::clicked,this,&Widget::slotAddLike);

    connect(musicListTable,&TableOne::addILoveFilepathSignal,playSongArea,&PlaySongArea::slotFavIsExixts);
    connect(musicListTable,&TableOne::removeILoveFilepathSignal,playSongArea,&PlaySongArea::slotFavIsExixts);
//    connect(playSongArea,&PlaySongArea::signalAddFromFavButton,musicListTable,&TableOne::selectListChanged);
//    connect(playSongArea,&PlaySongArea::signalDelFromFavButton,musicListTable,&TableOne::selectListChanged);
    connect(m_miniWidget->m_preBtn,&QPushButton::clicked,playSongArea,&PlaySongArea::slotPrevious);
    connect(m_miniWidget->m_playStateBtn,&QPushButton::clicked,playSongArea,&PlaySongArea::slotPlayClicked);
    connect(m_miniWidget->m_nextBtn,&QPushButton::clicked,playSongArea,&PlaySongArea::slotNext);
    connect(musicListTable,&TableOne::addILoveFilepathSignal,m_miniWidget,&miniWidget::slotFavIsExixts);
    connect(musicListTable,&TableOne::removeILoveFilepathSignal,m_miniWidget,&miniWidget::slotFavIsExixts);
    connect(m_miniWidget,&miniWidget::signalFavBtnChange,playSongArea,&PlaySongArea::slotFavBtnChange);
    connect(playSongArea,&PlaySongArea::signalFavBtnChange,m_miniWidget,&miniWidget::slotFavBtnChange);
    connect(playSongArea,&PlaySongArea::signalPlayingLab,m_miniWidget,&miniWidget::slotPlayingLab);
    connect(playSongArea,&PlaySongArea::signalTimeLab,m_miniWidget,&miniWidget::slotTimeLab);
    connect(playSongArea,&PlaySongArea::signalRefreshFav,musicListTable,&TableOne::selectListChanged);
}

void Widget::initGSettings()//初始化GSettings
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
        themeData = new QGSettings(FITTHEMEWINDOW);
        if(themeData->get("style-name").toString() == "ukui-dark" || themeData->get("style-name").toString() == "ukui-black"){
            WidgetStyle::themeColor = 1;
            changeDarkTheme();
        }
        else
        {
            WidgetStyle::themeColor = 0;
            changeLightTheme();
        }

        connect(themeData,&QGSettings::changed,this,[=]()
        {
            qDebug() << "主题颜色" << themeData->get("style-name").toString();
            if(themeData->get("style-name").toString() == "ukui-dark" || themeData->get("style-name").toString() == "ukui-black"){
                WidgetStyle::themeColor = 1;
                changeDarkTheme();
            }
            else
            {
                WidgetStyle::themeColor = 0;
                changeLightTheme();
            }
        });
    }
    qDebug()<<"初始化GSettings成功";
}

void Widget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resizeEvent" << this->width() << this->height() << qApp->primaryScreen()->size();
    historyListTable->resize(320,this->height() - playSongArea->height()+20);
    int x = this->width()-historyListTable->width();
    int max_w = qApp->primaryScreen()->size().width();
    int max_h = qApp->primaryScreen()->size().height()-46;
//    int y = this->height()-playSongArea->height()-historyListTable->height();
    historyListTable->move(x,2);
    if(this->width() >= max_w && this->height()>= max_h)
    {
        m_titleBar->maximumBtn->setIcon(QIcon::fromTheme("window-restore-symbolic"));
        Minimize = true;
        m_titleBar->maximumBtn->setToolTip(tr("reduction"));
    }
    else if(this->isMaximized() == false || (this->width() < (max_w-20)) || this->height() < (max_h-20))
    {
        Minimize = false;
        m_titleBar->maximumBtn->setIcon(QIcon::fromTheme("window-maximize-symbolic"));
        m_titleBar->maximumBtn->setToolTip(tr("maximize"));
    }
    QWidget::resizeEvent(event);
}

//键盘F1响应唤出用户手册
void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F1) {
            emit signalShowGuide();
        }
    QWidget::keyPressEvent(event);
}

//void Widget::mousePressEvent(QResizeEvent *event)
//{
//    qDebug() << QCursor::pos();
//}

void Widget::slotShowMiniWidget()
{
    // 添加过渡动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    connect(animation, &QPropertyAnimation::valueChanged, [&](QVariant value){
        update();
    });
    connect(animation, &QPropertyAnimation::finished, [&](){
        hide();
        setWindowOpacity(1);
    });

    QPropertyAnimation *animation_mini = new QPropertyAnimation(m_miniWidget, "windowOpacity");
    animation_mini->setDuration(200);
    animation_mini->setStartValue(0);
    animation_mini->setEndValue(1);
    connect(animation_mini, &QPropertyAnimation::valueChanged, [&](QVariant value){
        m_miniWidget->update();
    });

    m_miniWidget->setWindowOpacity(0);
    m_miniWidget->showNormal();
    m_miniWidget->activateWindow();
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    animation_mini->start(QAbstractAnimation::DeleteWhenStopped);
}

void Widget::moveWidget(QString newWidth, QString newHeight)
{
    int newX = newWidth.toInt();
    int newY = newHeight.toInt();
    this->move(newX, newY);
}

void Widget::slotClose()
{
    this->close();
}

void Widget::slotShowMinimized()
{
    this->showMinimized();
    if(Minimize == true)
    {
        this->showMaximized();
    }
    else
    {
        this->showNormal();
    }
}

void Widget::slotShowMaximized()
{
    if(Minimize)
    {
        showNormal();
        Minimize = false;
        m_titleBar->maximumBtn->setIcon(QIcon::fromTheme("window-maximize-symbolic"));
        m_titleBar->maximumBtn->setToolTip(tr("maximize"));
        m_titleBar->maximumBtn->setProperty("isWindowButton", 0x1);
        m_titleBar->maximumBtn->setProperty("useIconHighlightEffect", 0x2);
        m_titleBar->maximumBtn->setFlat(true);
    }
    else
    {
        showMaximized();
        Minimize = true;
        m_titleBar->maximumBtn->setIcon(QIcon::fromTheme("window-restore-symbolic"));
//                maximumBtn->setToolTip(tr("还原"));
        m_titleBar->maximumBtn->setToolTip(tr("reduction"));
        m_titleBar->maximumBtn->setProperty("isWindowButton", 0x1);
        m_titleBar->maximumBtn->setProperty("useIconHighlightEffect", 0x2);
        m_titleBar->maximumBtn->setFlat(true);
    }
}

void Widget::slotRecoverNormalWidget()
{
    if(Minimize == true)
    {
        this->showMaximized();
    }
    else
    {
        this->showNormal();
    }
    m_miniWidget->hide();
}

void Widget::slotCloseMiniWidget()
{
    this->close();
}

//切换深色主题
void Widget::changeDarkTheme()
{
    sideBarWid->newSonglistPup->dlgcolor();
    sideBarWid->renameSongListPup->dlgcolor();
    sideBarWid->sidecolor();
    playSongArea->playcolor();
    m_miniWidget->minicolor();
    m_titleBar->titlecolor();
    musicListTable->initTableViewStyle();
    musicListTable->setHightLightAndSelect();
//    historyListTable->initStyle();
//    historyListTable->initTableStyle();

}

//切换浅色主题
void Widget::changeLightTheme()
{
    sideBarWid->newSonglistPup->dlgcolor();
    sideBarWid->renameSongListPup->dlgcolor();
    sideBarWid->sidecolor();
    playSongArea->playcolor();
    m_miniWidget->minicolor();
    m_titleBar->titlecolor();
    musicListTable->initTableViewStyle();
    musicListTable->setHightLightAndSelect();
//    historyListTable->initStyle();
//    historyListTable->setHighlight(-1);
}
