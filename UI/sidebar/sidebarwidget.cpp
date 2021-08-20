#include "sidebarwidget.h"
#include "UIControl/player/player.h"
#include "UIControl/base/musicDataBase.h"
#include "UI/base/widgetstyle.h"
#include "UI/mainwidget.h"

SideBarWidget::SideBarWidget(QWidget *parent) : QWidget(parent)
{
//    setFixedSize(210, 640);
    resize(210, 640);
    this->setObjectName("SideBarWidget");
    initWidget(); //初始化侧边栏界面
    initConnect(); //建立connect
    sidecolor();   //初始化样式

    getPlayListName();
}

void SideBarWidget::initWidget()
{
    //侧边栏界面样式
    QVBoxLayout *mainLayout = new QVBoxLayout();

    //logo
    QHBoxLayout *logoLayout = new QHBoxLayout();
    QLabel *logoNameLabel = new QLabel(this);
    logoNameLabel->setText(tr("Music Player"));
    logoPushButton = new QPushButton(this);
    logoPushButton->setFixedSize(24,24);
    logoPushButton->setIconSize(QSize(25,25));
    logoPushButton->setIcon(QIcon::fromTheme("kylin-music"));

    logoLayout->setSpacing(0);
    logoLayout->setMargin(8);
    logoLayout->addWidget(logoPushButton,Qt::AlignLeft);
    logoLayout->addSpacing(8);
    logoLayout->addWidget(logoNameLabel,0,Qt::AlignLeft);
    logoLayout->addStretch();

//    logoLayout->setContentsMargins(8,8,0,0);

    //音乐库
    QHBoxLayout *libraryLayout = new QHBoxLayout();
    libraryLabel = new QLabel(this);
    libraryLabel->setText(tr("Library"));
//    libraryLabel->setFixedSize(80,30);
    libraryLayout->addWidget(libraryLabel);
    libraryLayout->setContentsMargins(38,0,38,0);

    //歌曲列表
    QHBoxLayout *playListBtnLayout = new QHBoxLayout();
    playListBtn = new MyToolButton;
    playListBtn->setText(tr("Song List"));
//    playListBtn->setIcon(QIcon::fromTheme("ukui-folder-music-symbolic"));
    playListBtn->setStatusTip(IS_SELECT);
    playListBtn->buttonListName = ALLMUSIC;
    playListBtn->defaultStyle();
    playListBtnLayout->addWidget(playListBtn,Qt::AlignCenter);
//    playListBtnLayout->setContentsMargins(24,0,24,0);
    connect(playListBtn,&MyToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnCliced);

    //我的歌单
    QHBoxLayout *myPlayListLayout = new QHBoxLayout();
    myPlayListLabel = new QLabel(this);
    myPlayListBtn = new QPushButton(this);
    myPlayListLabel->setText(tr("My PlayList"));
    myPlayListBtn->setFixedSize(20,20);
    myPlayListBtn->setIcon(QIcon::fromTheme("list-add-symbolic"));
    myPlayListBtn->setProperty("isWindowButton", 0x1);
    myPlayListBtn->setProperty("useIconHighlightEffect", 0x2);
    myPlayListBtn->setFlat(true);

    myPlayListLayout->addWidget(myPlayListLabel);
    myPlayListLayout->addStretch();
    myPlayListLayout->addWidget(myPlayListBtn);
    myPlayListLayout->setContentsMargins(38,0,38,0);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(0);

    //歌单界面
    newPlayListWidget = new QWidget();
    newPlayListLayout = new QVBoxLayout();

    scrollArea->setWidgetResizable(true);
//    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setFrameShape(QFrame::NoFrame);


    newPlayListLayout->setAlignment(Qt::AlignTop);
    newPlayListWidget->setLayout(newPlayListLayout);
    newPlayListLayout->setContentsMargins(24,0,24,0);

//    mainLayout->addLayout(logoLayout);
//    mainLayout->addSpacing(16);

    mainLayout->addLayout(libraryLayout);
    mainLayout->addSpacing(6);
    mainLayout->addLayout(playListBtnLayout);
    mainLayout->addSpacing(16);
    mainLayout->addLayout(myPlayListLayout);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(newPlayListWidget);
    mainLayout->addStretch();
    mainLayout->setMargin(0);

    //新建歌单弹窗
    newSonglistPup = new AllPupWindow(this);
    newSonglistPup->titleLab->setText(tr("New Playlist"));
    newSonglistPup->hide();

    renameSongListPup = new AllPupWindow(this);
    renameSongListPup->titleLab->setText(tr("Rename"));
    renameSongListPup->hide();

    //侧边栏界面 嵌套 widget
    mainWid = new QWidget(this);
    mainWid->setLayout(mainLayout);
    mainWid->setObjectName("mainWid");
    scrollArea->setWidget(mainWid);
    scrollArea->verticalScrollBar()->setProperty("drawScrollBarGroove",false);

    logoWid = new QWidget(this);
    logoWid->setLayout(logoLayout);
    logoWid->setObjectName("logoWid");

    //放置活动条
    QVBoxLayout *scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(logoWid);
    scrollLayout->addSpacing(16);
    scrollLayout->addWidget(scrollArea);
    scrollLayout->setMargin(0);
    scrollLayout->setSpacing(0);

    // 真正的主widget
    sideWid = new QWidget(this);
    sideWid->setLayout(scrollLayout);
    sideWid->setObjectName("sideWid");

    QVBoxLayout *sideLayout = new QVBoxLayout(this);
    sideLayout->addWidget(sideWid);
    sideLayout->setMargin(0);
    sideLayout->setSpacing(0);


    this->setAutoFillBackground(true);
    //限制应用内字体固定大小
//    QFont sizeFont;
//    sizeFont.setPixelSize(14);
//    myPlayListBtn->setFont(sizeFont);
//    playListBtn->setFont(sizeFont);
//    logoPushButton->setFont(sizeFont);
//    libraryLabel->setFont(sizeFont);
//    myPlayListLabel->setFont(sizeFont);
//    logoNameLabel->setFont(sizeFont);

}

void SideBarWidget::initConnect()
{
    connect(myPlayListBtn,&QPushButton::clicked,this,&SideBarWidget::addPlayList);
    connect(playListBtn,SIGNAL(playall(QString)),this,SLOT(playAll(QString)));
    connect(newSonglistPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(addItemToSongList()));
    connect(newSonglistPup->enterLineEdit,SIGNAL(returnPressed()),this,SLOT(addItemToSongList()));

    connect(renameSongListPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(renamePlayList()));
    connect(renameSongListPup->enterLineEdit,SIGNAL(returnPressed()),this,SLOT(renamePlayList()));
}

void SideBarWidget::sidecolor()
{
    if(WidgetStyle::themeColor == 1)
    {
        QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
        for(MyToolButton *tmp : list)
        {
            tmp->defaultStyle();
        }
        setStyleSheet("SideBarWidget{background: #1F2022;}");
        logoWid->setStyleSheet("width: 210px;\
                             background: #1F2022;");
        sideWid->setStyleSheet("#sideWid{width: 210px;\
                      background: #1F2022;\
                      }");
        mainWid->setStyleSheet("#mainWid{width: 210px;\
                      background: #1F2022;\
                      }");
        logoPushButton->setStyleSheet("QPushButton{border:0px;background:transparent;}"
                                      "QPushButton::hover{border:0px;background:transparent;}"
                                      "QPushButton::pressed{border:0px;background:transparent;}");
        myPlayListLabel->setStyleSheet("color: #8F9399;line-height: 14px;");
        libraryLabel->setStyleSheet("color: #8F9399;line-height: 14px;");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
        for(MyToolButton *tmp : list)
        {
            tmp->defaultStyle();
        }
        setStyleSheet("");
        logoWid->setStyleSheet("width: 210px;background: #F5F5F5;");
        sideWid->setStyleSheet("#sideWid{width: 210px;\
                      background: #F5F5F5;\
                      }");
        mainWid->setStyleSheet("#mainWid{width: 210px;\
                      background: #F5F5F5;\
                      }");
        logoPushButton->setStyleSheet("QPushButton{border:0px;background:transparent;}"
                                      "QPushButton::hover{border:0px;background:transparent;}"
                                      "QPushButton::pressed{border:0px;background:transparent;}");
        myPlayListLabel->setStyleSheet("color: #8F9399;line-height: 14px;");
        libraryLabel->setStyleSheet("color: #8F9399;line-height: 14px;");
    }
}

void SideBarWidget::getPlayListName()
{
    int ret = g_db->getPlayList(playListName);
    if(ret != DB_OP_SUCC)
    {
        qDebug() << "获取歌单名信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return;
    }
    QString listName;
    for(int i = 0; i < playListName.size();i++)
    {
        listName = playListName.at(i);
        if(listName == SEARCH)
        {
            return;
        }
        MyToolButton *newBtn = new MyToolButton;
        newPlayListLayout->setAlignment(Qt::AlignTop);
        newPlayListLayout->addWidget(newBtn);
    //    newPlayListLayout->setContentsMargins(24,0,24,0);
        newPlayListLayout->setSpacing(6);

        if(listName == FAV)
        {
            newBtn->setText(tr("I Love"));
            newBtn->buttonListName = FAV;
            newBtn->defaultStyle();
        }
        else
        {
            newBtn->setText(listName);
            newBtn->buttonListName = "";
            newBtn->defaultStyle();
        }
        connect(newBtn,SIGNAL(playall(QString)),this,SLOT(playAll(QString)));
        connect(newBtn,SIGNAL(renamePlayList(QString)),this,SLOT(rename(QString)));
        connect(newBtn,SIGNAL(removePlayList(QString)),this,SLOT(removePlayList(QString)));
        connect(newBtn,&MyToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnCliced);

        QString name = playController::getInstance().getPlayListName();
        if(listName == name )
        {
            m_lastBtn = newBtn;
        }
    }
    if(m_lastBtn != nullptr)
    {
        m_lastBtn->clicked();
    }
}

void SideBarWidget::addPlayList()
{
    newSonglistPup->enterLineEdit->clear();
    newSonglistPup->show();
    newSonglistPup->enterLineEdit->setFocus();
}

void SideBarWidget::addItemToSongList()
{
    MyToolButton *newBtn = new MyToolButton();
//    QIcon icon = QIcon(":/img/default/songlist.png");
//    newBtn->setIcon(QIcon(":/img/default/songlist.png"));
//    newBtn->setIcon(QIcon::fromTheme("stock_contact-list"));
    newPlayListLayout->setAlignment(Qt::AlignTop);
//    newPlayListLayout->setContentsMargins(24,0,24,0);
    newPlayListLayout->setSpacing(6);

    QString text = newSonglistPup->enterLineEdit->text();

//    QString showText = fm.elidedText(text, Qt::ElideRight, 200);

    if(text != "")
    {
        for(int i = 0;i< playListName.size();i++)
        {
            if(playListName.at(i) == text)
            {
                newSonglistPup->hide();
                QMessageBox::warning(Widget::mutual,tr("Prompt information"),tr("Single song name already exists!!!"),QMessageBox::Ok);
                return ;
            }
        }

        newBtn->setText(text);
        newBtn->buttonListName = "";
        newBtn->defaultStyle();
        newPlayListLayout->addWidget(newBtn);
        playListName.append(text);
        newSonglistPup->hide();
        g_db->createNewPlayList(text);
        emit playListAdded(text);
    }
    else
    {
        QString listName = newPlayListName();
        newBtn->setText(listName);
        newBtn->buttonListName = "";
        newBtn->defaultStyle();
        newPlayListLayout->addWidget(newBtn);
        playListName.append(listName);
        newSonglistPup->hide();
        g_db->createNewPlayList(listName);
        emit playListAdded(listName);
    }
    connect(newBtn,SIGNAL(playall(QString)),this,SLOT(playAll(QString)));
    connect(newBtn,SIGNAL(renamePlayList(QString)),this,SLOT(rename(QString)));
    connect(newBtn,SIGNAL(removePlayList(QString)),this,SLOT(removePlayList(QString)));
    connect(newBtn,&MyToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnCliced);

}

QString SideBarWidget::newPlayListName()
{
    QMap<QString, QString> exitsName;
    for (auto &name : playListName)
    {
        exitsName.insert(name, name);
    }

    QString name = tr("New Playlist");
    if(!exitsName.contains(name))
    {
        return name;
    }

    int i = 1;
    for(i = 1; i < exitsName.size() + 1; i++)
    {
        QString newName = QString("%1 %2").arg(name).arg(i);
        if(!exitsName.contains(newName))
        {
            return newName;
        }
    }
    return QString("%1 %2").arg(name).arg(i);
}

void SideBarWidget::playAll(QString btnText)
{
    emit signalPlayAll(btnText);
}

void SideBarWidget::rename(QString text)
{
    btnText = text;
    renameSongListPup->enterLineEdit->clear();
    renameSongListPup->show();
    renameSongListPup->enterLineEdit->setFocus();
}

void SideBarWidget::renamePlayList()
{
    QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
    for(MyToolButton *tmp : list)
    {
        if(tmp->text() ==  btnText)
        {
            QString text = renameSongListPup->enterLineEdit->text();
            if(text != "")
            {
                for(int i = 0;i< playListName.size();i++)
                {
                    if(playListName.at(i) == text)
                    {
                        renameSongListPup->hide();
                        QMessageBox::warning(Widget::mutual,tr("Prompt information"),tr("Single song name already exists!!!"),QMessageBox::Ok);
                        return ;
                    }
                }

                tmp->setText(text);
                playController::getInstance().setCurList(text);
                for(int i = 0; i < playListName.size();i++)
                {
                    if(btnText == playListName[i])
                    {
                        playListName[i] = text;
                    }
                }
                g_db->renamePlayList(btnText,text); // 从数据库中重命名
                emit playListRenamed(btnText,text);   //fff
                renameSongListPup->hide();
            }
            else
            {
                QString listName = btnText;
                tmp->setText(listName);
                playController::getInstance().setCurList(listName);
                renameSongListPup->hide();
            }
        }
    }
}

void SideBarWidget::removePlayList(QString text)
{
    QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
    MyToolButton *tmp = nullptr;

     for(auto i = list.begin();i < list.end();i++)
     {
         if((*i)->text() == text)
         {
             if(i+1 == list.end())
             {
                tmp = *(i-1);
                tmp->click();
             }
             else
             {
                 tmp = *(i+1);
                 tmp->click();
             }
             //删除歌单时，删除改播放歌单的所有media
             int ret;
             QList<musicDataStruct> musicDateList;
             ret = g_db->getSongInfoListFromPlayList(musicDateList,text);
             if(ret == DB_OP_SUCC)
             {
                 for(int i = 0; i < musicDateList.size(); i++)
                 {
                     playController::getInstance().removeSongFromCurList(text, 0);
                 }
             }
             (*i)->deleteLater();
             g_db->delPlayList(text);
             playListName.removeOne(text);
         }
     }
    emit playListRemoved(text);
//    promptSongListPup->pupDialog->hide();
}

void SideBarWidget::slotListSearch()
{
    QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
    for(MyToolButton *tmp : list)
    {
        tmp->setStatusTip("");
        tmp->buttonListName = "";
        tmp->defaultStyle();
    }
}
