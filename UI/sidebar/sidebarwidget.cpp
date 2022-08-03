#include "sidebarwidget.h"
#include "UIControl/player/player.h"
#include "UIControl/base/musicDataBase.h"
#include "UI/base/widgetstyle.h"
#include "UI/mainwidget.h"
#include <ukui-log4qt.h>

SideBarWidget::SideBarWidget(QWidget *parent) : LeftsiderbarWidget(parent)
{
    this->resize(210, 640);
    this->setObjectName("SideBarWidget");

    initWidget();
    initConnect();
    sidecolor();

    getPlayListName();
}

void SideBarWidget::initAddButton()
{
    if (myPlayListBtn == nullptr) {
        return;
    }


    if (WidgetStyle::themeColor == 1) {
//        myPlayListBtn->setIcon(QIcon(":/img/list-add-symbolic/list-add-symbolic-default-white.png"));

#if 1
        myPlayListBtn->setStyleSheet("QPushButton{border-image: url(:/img/list-add-symbolic/list-add-symbolic-default-white.png);}"
                                     "QPushButton:hover{border-image: url(:/img/list-add-symbolic/list-add-symbolic-hover-white.png);}"
                                     "QPushButton:pressed{border-image: url(:/img/list-add-symbolic/list-add-symbolic-click-white.png);}"
                                     );
#endif


    } else {
//    myPlayListBtn->setIcon(QIcon(":/img/list-add-symbolic/list-add-symbolic-default-black.png"));
#if 1
        myPlayListBtn->setStyleSheet("QPushButton{border-image: url(:/img/list-add-symbolic/list-add-symbolic-default-black.png);}"
                                     "QPushButton:hover{border-image: url(:/img/list-add-symbolic/list-add-symbolic-hover-black.png);}"
                                     "QPushButton:pressed{border-image: url(:/img/list-add-symbolic/list-add-symbolic-click-black.png);}"
                                     );
#endif

    }

//    myPlayListBtn->setIconSize(QSize(20, 20));

}

void SideBarWidget::initWidget()
{
    this->setProperty("useSystemStyleBlur", true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    //侧边栏界面样式
    QVBoxLayout *mainLayout = new QVBoxLayout();

    //logo
    QHBoxLayout *logoLayout = new QHBoxLayout();
    QLabel *logoNameLabel = new QLabel(this);
    logoNameLabel->setText(tr("Music Player"));
    logoNameLabel->setFixedHeight(28);

    QLabel *logoIconLabel = new QLabel(this);
    logoIconLabel->setFixedSize(QSize(25, 25));
    logoIconLabel->setPixmap(QIcon::fromTheme("kylin-music").pixmap(logoIconLabel->size()));

    logoLayout->setSpacing(0);
    logoLayout->setMargin(8);
    logoLayout->addWidget(logoIconLabel,Qt::AlignLeft);
    logoLayout->addSpacing(8);
    logoLayout->addWidget(logoNameLabel,0,Qt::AlignLeft);
    logoLayout->addStretch();
    logoLayout->setContentsMargins(8,8,0,0);

    //音乐库
    QHBoxLayout *libraryLayout = new QHBoxLayout();
    libraryLabel = new QLabel(this);
    libraryLabel->setText(tr("Library"));
    libraryLabel->setFixedHeight(28);
//    libraryLabel->setFixedSize(80,30);
    libraryLayout->addWidget(libraryLabel);
    libraryLayout->setContentsMargins(38,0,38,0);

    //歌曲列表
    QHBoxLayout *playListBtnLayout = new QHBoxLayout();
    playListBtn = new CustomToolButton;
    playListBtn->setText(tr("Song List"));
//    playListBtn->setIcon(QIcon::fromTheme("ukui-folder-music-symbolic"));
    playListBtn->setStatusTip(IS_SELECT);
    playListBtn->buttonListName = ALLMUSIC;
    playListBtn->defaultStyle();
    playListBtnLayout->addWidget(playListBtn,Qt::AlignCenter);
//    playListBtnLayout->setContentsMargins(24,0,24,0);
    connect(playListBtn,&CustomToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnClicked);

    //我的歌单
    QHBoxLayout *myPlayListLayout = new QHBoxLayout;
    myPlayListLabel = new QLabel(this);
    myPlayListBtn = new QPushButton(this);

    initAddButton();
    myPlayListLabel->setText(tr("My PlayList"));
    myPlayListLabel->setFixedHeight(28);
    myPlayListBtn->setFixedSize(36, 36);

//    myPlayListBtn->setIcon(QIcon::fromTheme("list-add-symbolic"));
//    myPlayListBtn->setProperty("isWindowButton", 0x1);
//    myPlayListBtn->setProperty("useIconHighlightEffect", 0x2);
//    myPlayListBtn->setFlat(true);

    myPlayListLayout->addWidget(myPlayListLabel);
    myPlayListLayout->addStretch();
    myPlayListLayout->addWidget(myPlayListBtn);
    myPlayListLayout->setContentsMargins(38,0,38,0);

    scrollArea = new CustomSiderBarScrollArea(this);
    scrollArea->setFrameStyle(0);

    //歌单界面
    newPlayListWidget = new QWidget();
    newPlayListLayout = new QVBoxLayout();

    scrollArea->setWidgetResizable(true);
//    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);


    newPlayListLayout->setAlignment(Qt::AlignTop);
    newPlayListWidget->setLayout(newPlayListLayout);
    newPlayListLayout->setContentsMargins(24,0,24,0);

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
    newSonglistPup = new PopupDialog(this);
    newSonglistPup->titleLab->setText(tr("New Playlist"));
    newSonglistPup->hide();

    renameSongListPup = new PopupDialog(this);
    renameSongListPup->titleLab->setText(tr("Rename"));
    renameSongListPup->hide();

    //侧边栏界面 嵌套 widget
    mainWid = new QWidget(this);
    mainWid->setLayout(mainLayout);
    mainWid->setObjectName("mainWid");
    scrollArea->setWidget(mainWid);
    scrollArea->verticalScrollBar()->setProperty("drawScrollBarGroove",false);

    logoWid = new QWidget(this);
    logoWid->setFixedHeight(40);
    logoWid->setLayout(logoLayout);
    logoWid->setObjectName("logoWid");
    //放置活动条
    QVBoxLayout *scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(logoWid);
    scrollLayout->addSpacing(16);
    scrollLayout->addWidget(scrollArea);
    scrollLayout->setMargin(0);
    scrollLayout->setSpacing(0);

    this->setLayout(scrollLayout);

    this->setAutoFillBackground(true);

//    // 真正的主widget
//    sideWid = new QWidget(this);
//    sideWid->setLayout(scrollLayout);
//    sideWid->setObjectName("sideWid");

//    QVBoxLayout *sideLayout = new QVBoxLayout(this);
//    sideLayout->addWidget(sideWid);
//    sideLayout->setMargin(0);
//    sideLayout->setSpacing(0);


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
    myPlayListLabel->setStyleSheet("line-height: 14px;");
    libraryLabel->setStyleSheet("line-height: 14px;");

    initAddButton();

    if(WidgetStyle::themeColor == 1) {
        QPalette pal(palette());
        pal.setColor(QPalette::Background, QColor(38, 38, 38));
        setAutoFillBackground(true);
        setPalette(pal);

        QList<CustomToolButton *> list = this->findChildren<CustomToolButton *>();

        for(CustomToolButton *tmp : list) {
            tmp->defaultStyle();
        }
    } else if(WidgetStyle::themeColor == 0) {
        QPalette pal(palette());
        pal.setColor(QPalette::Background, QColor(255, 255, 255));
        setAutoFillBackground(true);
        setPalette(pal);

        QList<CustomToolButton *> list = this->findChildren<CustomToolButton *>();

        for(CustomToolButton *tmp : list) {
            tmp->defaultStyle();
        }
    }
}


/**
 * @brief SideBarWidget::getPlayListName 获取歌单名称
 */
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
            continue;
        }
        CustomToolButton *newBtn = new CustomToolButton;
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
        connect(newBtn,&CustomToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnClicked);

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

/**
 * @brief SideBarWidget::addPlayList 添加歌单弹窗输入框
 */
void SideBarWidget::addPlayList()
{
    newSonglistPup->enterLineEdit->clear();
    newSonglistPup->show();
    newSonglistPup->enterLineEdit->setFocus();
}

/**
 * @brief SideBarWidget::addItemToSongList 添加歌单
 */
void SideBarWidget::addItemToSongList()
{
    CustomToolButton *newBtn = new CustomToolButton();
    newPlayListLayout->setAlignment(Qt::AlignTop);
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
        Q_EMIT playListAdded(text);
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
        Q_EMIT playListAdded(listName);
    }

    connect(newBtn,SIGNAL(playall(QString)),this,SLOT(playAll(QString)));
    connect(newBtn,SIGNAL(renamePlayList(QString)),this,SLOT(rename(QString)));
    connect(newBtn,SIGNAL(removePlayList(QString)),this,SLOT(removePlayList(QString)));
    connect(newBtn,&CustomToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnClicked);

}


/**
 * @brief SideBarWidget::newPlayListName 输入为空添加歌单
 * @return
 */
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


/**
 * @brief SideBarWidget::playAll 歌单播放
 * @param btnText
 */
void SideBarWidget::playAll(QString btnText)
{
    Q_EMIT signalPlayAll(btnText);
}

void SideBarWidget::rename(QString text)
{
    btnText = text;
    renameSongListPup->enterLineEdit->clear();
    renameSongListPup->show();
    renameSongListPup->enterLineEdit->setFocus();
}


/**
 * @brief SideBarWidget::renamePlayList 歌单重命名
 */
void SideBarWidget::renamePlayList()
{
    QList<CustomToolButton *> list = this->findChildren<CustomToolButton *>();
    for(CustomToolButton *tmp : list)
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
                Q_EMIT playListRenamed(btnText,text);   //fff
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


/**
 * @brief SideBarWidget::removePlayList 删除歌单
 * @param text
 */
void SideBarWidget::removePlayList(QString text)
{
    QList<CustomToolButton *> list = this->findChildren<CustomToolButton *>();
    CustomToolButton *tmp = nullptr;

     for(auto i = list.begin();i < list.end();i++) {
         if((*i)->text() == text) {
             if(i+1 == list.end()) {
                tmp = *(i-1);
                tmp->click();
             } else {
                 tmp = *(i+1);
                 tmp->click();
             }

             //删除歌单时，删除改播放歌单的所有media
             int ret;
             QList<musicDataStruct> musicDateList;
             ret = g_db->getSongInfoListFromPlayList(musicDateList,text);

             if(ret == DB_OP_SUCC) {
                 for(int j=0; j<musicDateList.size(); j++) {
                     playController::getInstance().removeSongFromCurList(text, 0);
                 }
             }

             (*i)->deleteLater();
             g_db->delPlayList(text);
             playListName.removeOne(text);
         }
     }

     Q_EMIT playListRemoved(text);
}


/**
 * @brief SideBarWidget::slotListSearch 取消侧边栏所有按钮的选中状态
 */
void SideBarWidget::slotListSearch()
{
    QList<CustomToolButton *> list = this->findChildren<CustomToolButton *>();

    for(CustomToolButton *tmp : list) {
        tmp->setStatusTip("");
        tmp->buttonListName = "";
        tmp->defaultStyle();
    }
}


/**
 * @brief SideBarWidget::slotSongListHigh 高亮歌曲列表按钮
 */
void SideBarWidget::slotSongListHigh()
{
    QList<CustomToolButton *> list = this->findChildren<CustomToolButton *>();
    for(CustomToolButton *tmp : list)
    {
        tmp->setStatusTip("");
        tmp->buttonListName = "";
        tmp->defaultStyle();
    }
    playListBtn->setStatusTip(IS_SELECT);
    playListBtn->buttonListName = ALLMUSIC;
    playListBtn->defaultStyle();
}
