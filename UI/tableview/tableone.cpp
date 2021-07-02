#include <QMimeData>
#include <QDirIterator>

#include "tableone.h"
#include "UI/mainwidget.h"

TableOne::TableOne(QString listName, QWidget *parent) : QWidget(parent)
{
    nowListName = listName;
    setObjectName("TableOne");
    setAcceptDrops(true);
    initUI();
    initRightMenu();
    initConnect();
    initStyle();
    tableView->setAlternatingRowColors(false);
    qDebug() << "歌曲列表初始化完成";
}

TableOne::~TableOne()
{

}

void TableOne::initStyle()
{
    listTitleLabel->setStyleSheet("font-size:24px;\
                                 font-weight: 600;\
                                 line-height: 24px;");

    listTotalNumLabel->setStyleSheet("font-weight: 400;color:#8C8C8C;\
                                        border:none;\
                                        line-height: 14px;");

    if (WidgetStyle::themeColor == 1)
    {
        this->setStyleSheet("#TableOne{background:red;border:none;}");
        titleWid->setStyleSheet("#titleWid{background:#252526;}");
        nullPageWidget->setStyleSheet("#nullPageWidget{background:#252526;}");
        tableView->setStyleSheet("#tableView{background:#252526;border:none;}");
        tableView->setAlternatingRowColors(false);
    }
    else if(WidgetStyle::themeColor == 0)
    {
        this->setStyleSheet("#TableOne{background:#FFFFFF;border:none;}");
        titleWid->setStyleSheet("#titleWid{background:#FFFFFF;}");
        nullPageWidget->setStyleSheet("#nullPageWidget{background:#FFFFFF;}");
        tableView->setStyleSheet("#tableView{background:#FFFFFF;border:none;}");
        tableView->setAlternatingRowColors(false);
    }
}

void TableOne::initTableViewStyle()
{
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->setColumnWidth(3,40);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableView->verticalHeader()->setVisible(false);// 垂直不可见
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    tableView->setAlternatingRowColors(false);
    horizonHeader = tableView->horizontalHeader();
    horizonHeader->setEnabled(false);
    m_model->m_model.setHorizontalHeaderLabels(m_model->titleList);
    horizonHeader->setHighlightSections(false);
    if(WidgetStyle::themeColor == 0) {
        horizonHeader->setStyleSheet("QHeaderView::section,QTableCornerButton::section {padding-left: 25px;\
                                                                border: none;border-bottom: 1px solid white;\
                                                                border-right: 1px solid white;border-bottom: 1px transparent;\
                                                                background-color:white;font-size:16px;color:#8F9399;}");
    } else {
        horizonHeader->setStyleSheet("QHeaderView::section,QTableCornerButton::section {padding-left: 25px;\
                                                        border: none;border-bottom: 1px solid #252526;\
                                                        border-right: 1px solid #252526;border-bottom: 1px transparent;\
                                                        background-color:#252526;font-size:16px;color:#8F9399;}");
    }

    horizonHeader->setDefaultAlignment(Qt::AlignLeft);

    tableView->verticalScrollBar()->setStyleSheet("width:25px;");
    if(m_model->count() <= showScrollbarNumber) {
        tableView->verticalScrollBar()->setVisible(false);

    } else {
        tableView->verticalScrollBar()->setVisible(true);
    }
}

void TableOne::initUI()
{
    QVBoxLayout *tableLayout = new QVBoxLayout();
    this->setLayout(tableLayout);

    listTitleLabel = new QLabel(this);
    listTitleLabel->setMaximumWidth(192);
    listTotalNumLabel = new QLabel(this);
    listTotalNumLabel->setAlignment(Qt::AlignBottom);
    addMusicButton = new QToolButton(this);
    listTitleHBoxLayout = new QHBoxLayout();
    playAllButton = new QPushButton(this);


    titleWid = new QWidget(this);
    titleWid->setObjectName("titleWid");
    titleWid->setLayout(listTitleHBoxLayout);
    listTitleHBoxLayout->setAlignment(Qt::AlignLeft);
    listTitleHBoxLayout->addWidget(listTitleLabel);
    listTitleHBoxLayout->addSpacing(16);
    listTitleHBoxLayout->addWidget(listTotalNumLabel);
    listTitleHBoxLayout->addStretch(0);
    listTitleHBoxLayout->addWidget(playAllButton,1,Qt::AlignRight);
    listTitleHBoxLayout->addSpacing(16);
    listTitleHBoxLayout->addWidget(addMusicButton,0,Qt::AlignRight);
    listTitleHBoxLayout->setContentsMargins(25,20,25,30);
    listTitleHBoxLayout->setSpacing(0);
    titleWid->setFixedHeight(80);

    playAllButton->setText(tr("Play All"));
    playAllButton->setIconSize(QSize(16,16));
    playAllButton->setIcon(QIcon(":/img/default/play_w.png"));
//    playAllButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    playAllButton->setIconSize(QSize(16,16));
//    addMusicButton->setText(tr("   Add"));
    playAllButton->setFixedSize(112,36);
    playAllButton->setProperty("isImportant", true);
//    playAllButton->setPopupMode(QToolButton::InstantPopup);
//    playAllButton->setStyleSheet("QToolButton{padding-left:18px;background-color: #3790FA; color:#FFFFFF;border-radius: 6px;}"
//                                 "QToolButton::hover{background-color: #40A9FB;}"
//                                 "QToolButton::pressed{background-color: #296CD9;}");
//    playAllButton->setStyleSheet("QToolButton{background-color:#3790FA;padding-left:14px;color:#FFFFFF;border-radius: 6px;}");

    addMusicButton->setText(tr("Add"));
    addMusicButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addMusicButton->setIconSize(QSize(16,16));

    addMusicButton->setIcon(QIcon::fromTheme("list-add-symbolic"));
    addMusicButton->setProperty("useIconHighlightEffect", 0x8);
//    addMusicButton->setProperty("useIconHighlightEffect", 0x2);
//    addMusicButton->setFocusPolicy(Qt::NoFocus);
//    addMusicButton->setProperty("isImportant",true);

//    addMusicButton->setText(tr("   Add"));
    addMusicButton->setFixedSize(112,36);
    addMusicButton->setPopupMode(QToolButton::InstantPopup);
    add_menu = new QMenu(this);
    addMusicFileAction = new QAction(this);
    addDirMusicAction = new QAction(this);
    addMusicFileAction->setText(tr("Add local songs"));
    addDirMusicAction->setText(tr("Add local folders"));
    add_menu->addAction(addMusicFileAction);
    add_menu->addAction(addDirMusicAction);
    addMusicButton->setMenu(add_menu);

    QHBoxLayout *tableTitleLayout = new QHBoxLayout();
    QWidget *tableTitleWidget = new QWidget(this);
    tableTitleWidget->setLayout(tableTitleLayout);
    QLabel *songNameTitle = new QLabel(tr("Song"));
    QLabel *singerTitle = new QLabel(tr("Singer"));
    QLabel *albumTitle = new QLabel(tr("Album"));
    QLabel *songtimeTitle = new QLabel(tr("Time"));
    songtimeTitle->setAlignment(Qt::AlignRight);
    songtimeTitle->setFixedWidth(60);
    tableTitleLayout->setAlignment(Qt::AlignLeft);
    tableTitleLayout->addWidget(songNameTitle,1);
    tableTitleLayout->addSpacing(20);
    tableTitleLayout->addWidget(singerTitle,1);
    tableTitleLayout->addSpacing(20);
    tableTitleLayout->addWidget(albumTitle,1);
    tableTitleLayout->addStretch(0);
    tableTitleLayout->addWidget(songtimeTitle,Qt::AlignRight);
//    tableTitleLayout->addSpacing(10);
    tableTitleLayout->setSpacing(0);
    tableTitleLayout->setMargin(0);
//    tableTitleLayout->setContentsMargins(25,0,10,0);
    songNameTitle->setStyleSheet("color:#8F9399;");
    singerTitle->setStyleSheet("color:#8F9399;");
    albumTitle->setStyleSheet("color:#8F9399;");
    songtimeTitle->setStyleSheet("color:#8F9399;");


    tableView = new TableBaseView();
    tableView->setObjectName("tableView");
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置按ctrl键选中多个
    QList<musicDataStruct> resList;
    int ret;
    ret = g_db->getSongInfoListFromDB(resList,nowListName);
    m_model = new MusicListModel;
    m_model->add(resList);
    m_model->setView(*tableView);

    tableTitleWidget->hide();

//    m_musicWidget = new QWidget(this);
//    m_historyLayout = new QVBoxLayout();
//    m_musicWidget->setLayout(m_historyLayout);
//    m_historyLayout->addWidget(tableTitleWidget);
//    m_historyLayout->addWidget(tableView);
//    m_historyLayout->setSpacing(0);
//    m_historyLayout->setMargin(0);

//    m_historyLayout->setContentsMargins(0,0,20,0);

    nullPageWidget = new QWidget(this);
    nullPageWidget->setObjectName("nullPageWidget");

    nullPageHLayout = new QHBoxLayout();
    nullPageVLayout = new QVBoxLayout();
    nullPageIconLabel = new QLabel(this);
    nullPageTextLabel = new QLabel(this);
    n_addDirMusicButton = new QPushButton(this);
    n_addDirMusicButton->setFixedSize(134,36);
    n_addMusicButton = new QPushButton(this);
    n_addMusicButton->setFixedSize(134,36);
    nullPageTextLabel->setText(tr("There are no songs!"));
    nullPageTextLabel->setStyleSheet("color:#8F9399;");
    n_addMusicButton->setText(tr("Add Local Songs"));
    n_addDirMusicButton->setText(tr("Add Local Folder"));
    nullPageIconLabel->setPixmap(QPixmap(":/img/default/pict1.png").scaled(200,180));

    nullPageWidget->setLayout(nullPageVLayout);
    nullPageVLayout->addStretch();
    nullPageVLayout->addWidget(nullPageIconLabel);
    nullPageIconLabel->setAlignment(Qt::AlignHCenter);
    nullPageVLayout->addWidget(nullPageTextLabel);
    nullPageTextLabel->setAlignment(Qt::AlignHCenter);
    nullPageVLayout->addLayout(nullPageHLayout);
    nullPageVLayout->addStretch();

    nullPageHLayout->addWidget(n_addMusicButton);
    nullPageHLayout->setSpacing(16);
    nullPageHLayout->addWidget(n_addDirMusicButton);

    nullPageVLayout->addStretch();
    nullPageVLayout->setAlignment(Qt::AlignCenter);
    nullPageVLayout->setSpacing(10);

    tableLayout->addWidget(titleWid,Qt::AlignTop);
//    tableLayout->addSpacing(10);
    tableLayout->addWidget(tableView);
    tableLayout->addWidget(nullPageWidget);
    tableLayout->setMargin(0);
//    tableLayout->setContentsMargins(0,0,20,0);
    tableLayout->setSpacing(0);

    initTableViewStyle();
//    listTitleLabel->setFixedWidth(120);
    listTotalNumLabel->setFixedWidth(120);
    showTitleText(nowListName);
    changeNumber();

    //限制应用字体不随着主题变化
    QFont sizeFont;
    sizeFont.setPixelSize(14);
    add_menu->setFont(sizeFont);
    listTotalNumLabel->setFont(sizeFont);
    addMusicButton->setFont(sizeFont);
    playAllButton->setFont(sizeFont);
    n_addDirMusicButton->setFont(sizeFont);
    n_addMusicButton->setFont(sizeFont);
    nullPageTextLabel->setFont(sizeFont);
}
void TableOne::showTitleText(QString listName)
{
    QFontMetrics fontWidth(listTitleLabel->font());//得到每个字符的宽度
    QString elideNote = fontWidth.elidedText(listName, Qt::ElideRight, 192);//最大宽度150像素
    if(listName == ALLMUSIC) {
        listTitleLabel->setText(tr("Song List"));
    } else if(listName == FAV){
        listTitleLabel->setText(tr("I Love"));
    } else {
        listTitleLabel->setText(elideNote);
    }
    if(elideNote != listName) {
        listTitleLabel->setToolTip(listName);
    }
    else{
        listTitleLabel->setToolTip("");
    }
}
void TableOne::initConnect()
{
    connect(tableView,&TableBaseView::doubleClicked,this,&TableOne::playSongs);
    connect(tableView,&TableBaseView::customContextMenuRequested,this,&TableOne::showRightMenu);
    connect(addMusicFileAction,&QAction::triggered,this,&TableOne::addMusicSlot);
    connect(addDirMusicAction,&QAction::triggered,this,&TableOne::addDirMusicSlot);
    connect(this,&TableOne::countChanges,this,&TableOne::changeNumber);
    connect(&playController::getInstance(),&playController::currentIndexAndCurrentList,this,&TableOne::getHightLightIndex);
    connect(n_addMusicButton,&QPushButton::clicked,this,&TableOne::addMusicSlot);
    connect(n_addDirMusicButton,&QPushButton::clicked,this,&TableOne::addDirMusicSlot);
    connect(playAllButton,&QPushButton::clicked,this,&TableOne::playAllButtonClicked);
}

void TableOne::initRightMenu()
{
    m_menu = new QMenu(tableView);

    playRow = new QAction(tr("Play"));
    removeRow = new QAction(tr("Delete from list"));
    removeLocalRow = new QAction(tr("Remove from local"));
    showInfoRow = new QAction(tr("View song information"));
    addToOtherListMenu = new QMenu(tr("Add to songlist"));
    connect(playRow,&QAction::triggered,this,&TableOne::playSongs);
    connect(removeRow,&QAction::triggered,this,&TableOne::isDeleteSongs);
    connect(removeLocalRow,&QAction::triggered,this,&TableOne::isDeleteLocalSongs);
    connect(showInfoRow,&QAction::triggered,this,&TableOne::showInfo);
    connect(addToOtherListMenu,&QMenu::triggered,this,&TableOne::addToOtherList);

    //限制应用字体不随着主题变化
    QFont sizeFont;
    sizeFont.setPixelSize(14);
    m_menu->setFont(sizeFont);
    addToOtherListMenu->setFont(sizeFont);

    tableView->installEventFilter(this);
}

void TableOne::showRightMenu(const QPoint &pos)
{
    QModelIndex index = tableView->indexAt(pos);
    if(index.row() < 0)
    {
        return;
    }
//    m_menu->move(0,0);
    m_menu->addAction(playRow);      //添加动作到菜单
    m_menu->addMenu(addToOtherListMenu);
    m_menu->addAction(removeRow);  //添加动作到菜单
    m_menu->addAction(removeLocalRow);
    m_menu->addAction(showInfoRow); //添加动作到菜单
    QMap<int,QString> map1 = getSelectedTaskIdList();
    addToOtherListMenu->clear();
    QStringList allList;
    int ret = g_db->getPlayList(allList);
    if(ret == 0)
    {
        for(int i = 0 ;i < allList.size() ;i++)
        {
            QAction *listNameAct = new QAction;
            addToOtherListMenu->addAction(listNameAct);
            if(allList[i] != FAV)
            {
                listNameAct->setText(allList[i]);
            }
            else
            {
                listNameAct->setText(tr("I Love"));
            }
        }
    }
    else
    {
        qDebug() << "获取所有歌单失败";
    }
    if(map1.size() > 1)
    {
        m_menu->removeAction(playRow);
        m_menu->removeAction(showInfoRow);
    }
    m_menu->exec(QCursor::pos());
}

void TableOne::isDeleteSongs()
{
    //确认将选中的歌曲从歌单中删除？
    QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("Prompt information"),tr("Confirm that the selected song will be deleted from the song list?"),QMessageBox::Yes | QMessageBox::No);
//    warn->button(QMessageBox::Yes)->setText("确定");
//    warn->button(QMessageBox::No)->setText("取消");
    int result = warn->exec();
    if(result == QMessageBox::Yes)
    {
        deleteSongs();
        qDebug() << "QMessageBox::Yes";
    }
    else
    {
        qDebug() << "QMessageBox::No";
        return;
    }
}

void TableOne::isDeleteLocalSongs()
{
    //歌曲从本地删除后不可恢复，是否确定删除？
    QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("Prompt information"),tr("After the song is deleted from the local, it cannot be resumed. Is it sure to delete?"),QMessageBox::Yes | QMessageBox::No);
//    warn->button(QMessageBox::Yes)->setText("确定");
//    warn->button(QMessageBox::No)->setText("取消");
    int result = warn->exec();
    if(result == QMessageBox::Yes)
    {
        deleteLocalSongs();
        qDebug() << "QMessageBox::Yes";
    }
    else
    {
        qDebug() << "QMessageBox::No";
        return;
    }
}

void TableOne::deleteSongs()
{
    QMap<int,QString> map1 = getSelectedTaskIdList();
    while (!map1.empty()) {
        QMap<int,QString>::iterator iter;
        iter = map1.end();
        iter--;
        int ret;
        int index = -1;
        if(nowListName != nowPlayListName && nowListName == ALLMUSIC) {
            index = MusicFileInformation::getInstance().findIndexFromPlayList(nowPlayListName,iter.value());
        }
        if(nowListName != ALLMUSIC)
        {
           ret = g_db->delMusicFromPlayList(iter.value(),nowListName);
        }
        else
        {
           ret = g_db->delSongFromEveryWhere(iter.value());
        }
        if(ret == 0)
        {
//            m_model->remove(iter.key());
            if((nowListName != nowPlayListName) && nowListName == ALLMUSIC && index != -1) {
                playController::getInstance().removeSongFromCurList(nowPlayListName,index);
            } else {
                playController::getInstance().removeSongFromCurList(nowListName,iter.key());
            }
//            if(nowListName == tr("I Love"))

            if(nowListName == FAV)
            {
                emit removeILoveFilepathSignal(iter.value());
            }
//            emit countChanges();

            map1.remove(iter.key());
        }
        else
        {
            qDebug() << "delete failed" << iter.value();
        }
    }
    selectListChanged(nowListName);
    emit refreshHistoryListSignal();
}

void TableOne::deleteLocalSongs()
{
    QMap<int,QString> map1 = getSelectedTaskIdList();
    while (!map1.empty())
    {
        QMap<int,QString>::iterator iter;
        iter = map1.end();
        iter--;
        QFile file(iter.value());
//        QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("Prompt information"),tr("Are you sure you want to delete it locally?"),QMessageBox::Yes | QMessageBox::No);
//        warn->button(QMessageBox::Yes)->setText("确定");
//        warn->button(QMessageBox::No)->setText("取消");
//        int result = warn->exec();
//        if(result == QMessageBox::Yes)
//        {
//            qDebug() << "QMessageBox::Yes";
//        }
//        else
//        {
//            qDebug() << "QMessageBox::No";
//            return;
//        }
        if(file.exists())
        {
            //移入回收站(作为已知)
            deleteImage(iter.value());
            //删除到回收站不成功从磁盘删除
            file.remove();
        }
        int ret;
        int index = -1;
        if(nowListName != nowPlayListName && nowListName == ALLMUSIC)
        {
            index = MusicFileInformation::getInstance().findIndexFromPlayList(nowPlayListName,iter.value());
        }
        if(nowListName != ALLMUSIC)
        {
           ret = g_db->delMusicFromPlayList(iter.value(),nowListName);
        }
        else
        {
           ret = g_db->delSongFromEveryWhere(iter.value());
        }
        if(ret == 0)
        {
            if((nowListName != nowPlayListName) && nowListName == ALLMUSIC && index != -1)
            {
                playController::getInstance().removeSongFromCurList(nowPlayListName,index);
            }
            else
            {
                playController::getInstance().removeSongFromCurList(nowListName,iter.key());
            }

            if(nowListName == FAV)
            {
                emit removeILoveFilepathSignal(iter.value());
            }

            map1.remove(iter.key());
        }
        else
        {
            qDebug() << "delete failed" << iter.value();
        }
    }
    selectListChanged(nowListName);
    emit refreshHistoryListSignal();
}

void TableOne::deleteImage(const QString &savepath)
{
    _processStart("gio",QStringList() << "trash" << savepath);
}

void TableOne::_processStart(const QString &cmd, QStringList arguments)
{
    Widget::mutual->process->start(cmd,arguments);
    Widget::mutual->process->waitForStarted();
    Widget::mutual->process->waitForFinished();
}

void TableOne::processLog()
{
    qDebug()<<"*******process error*******\n"
           << QString::fromLocal8Bit(Widget::mutual->process->readAllStandardError())
           <<"\n*******process error*******";
}


void TableOne::playSongs()
{
    int index = tableView->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    playMusicforIndex(nowListName,index);
}
void TableOne::playMusicforIndex(QString listName, int index)
{
    QStringList pathList;
    pathList = m_model->getPathList(listName);
    playController::getInstance().setCurPlaylist(listName,pathList);
    playController::getInstance().play(listName,index);
}
void TableOne::showInfo()
{
    int index = tableView->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    infoDialog = new MusicInfoDialog(date);
    //将弹窗应用居中显示
    QRect availableGeometry = this->parentWidget()->parentWidget()->geometry();
    infoDialog->move(availableGeometry.center() - infoDialog->rect().center());
    infoDialog->exec();
}

void TableOne::addToOtherList(QAction *listNameAction)
{
    int successCount = 0;
    int failCount = 0;
    QString listName = listNameAction->text();
    if(listName == tr("I Love")) {
        listName = FAV;
    }
    QMap<int,QString> map1 = getSelectedTaskIdList();
    int allCount = map1.size();
    QMap<int,QString>::Iterator it = map1.begin();
    while(it != map1.end())
    {
        int ret = g_db->addMusicToPlayList(it.value(),listName);
        if(ret == DB_OP_SUCC)
        {
            successCount++;
            playController::getInstance().addSongToCurList(listName,it.value());
//            if(listName == tr("I Love"))
            if(listName == FAV)
            {
                emit addILoveFilepathSignal(it.value());
            }
        }else{
            failCount++;
        }
        it++;
    }
    importFinished(successCount, failCount, allCount);
}
void TableOne::addMusicSlot()
{
    QFileDialog *fileDialog = new QFileDialog(Widget::mutual);
//    fileDialog->setParent(Widget::mutual);
    QList<QUrl> list = fileDialog->sidebarUrls();
    int sidebarNum = 8;//最大添加U盘数，可以自己定义
    QString home = QDir::homePath().section("/", -1, -1);
    QString mnt = "/media/" + home + "/";
    QDir mntDir(mnt);
    mntDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList filist = mntDir.entryInfoList();
    QList<QUrl> mntUrlList;
    for (int i = 0; i < sidebarNum && i < filist.size(); ++i) {
        QFileInfo fi = filist.at(i);
        //华为990、9a0需要屏蔽最小系统挂载的目录
        if (fi.fileName() == "2691-6AB8")
             continue;
        mntUrlList << QUrl("file://" + fi.filePath());
    }

    QFileSystemWatcher fsw(fileDialog);
    fsw.addPath("/media/" + home + "/");
    connect(&fsw, &QFileSystemWatcher::directoryChanged, fileDialog,
    [=, &sidebarNum, &mntUrlList, &list, fileDialog](const QString path) {
        QDir wmntDir(path);
        wmntDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        QFileInfoList wfilist = wmntDir.entryInfoList();
        mntUrlList.clear();
        for (int i = 0; i < sidebarNum && i < wfilist.size(); ++i) {
                       QFileInfo fi = wfilist.at(i);
                       //华为990、9a0需要屏蔽最小系统挂载的目录
                       if (fi.fileName() == "2691-6AB8")
                            continue;
                 mntUrlList << QUrl("file://" + fi.filePath());
             }
             fileDialog->setSidebarUrls(list + mntUrlList);

             fileDialog->update();
        });

    connect(fileDialog, &QFileDialog::finished, fileDialog, [=, &list, fileDialog]() {
        fileDialog->setSidebarUrls(list);
    });
    fileDialog->setSidebarUrls(list + mntUrlList);
    fileDialog->setNameFilter("音乐文件(*.voc *.aiff *.au *.dts *.flv *.m4r *.mka *.mmf *.mp2 *.mp4 *.mpa *.wv *.voc *.mp3 *.ogg *.wma *.amr *.flac *.wav *.ape *.m4a *.ac3 *.aac)");
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setOption(QFileDialog::HideNameFilterDetails);
    QStringList songFiles;
    if(QFileDialog::Accepted == fileDialog->exec())
    {
        songFiles = fileDialog->selectedFiles();
    }
    else
    {
        return;
    }

    for( int i = 0; i < songFiles.length(); i++ )
    {
        songFiles.at(i);
    }
    addMusicToDatebase(songFiles);
}

void TableOne::addDirMusicSlot()
{
    QFileDialog *fileDialog = new QFileDialog(Widget::mutual);

    QList<QUrl> list = fileDialog->sidebarUrls();
    int sidebarNum = 8;//最大添加U盘数，可以自己定义
    QString home = QDir::homePath().section("/", -1, -1);
    QString mnt = "/media/" + home + "/";
    QDir mntDir(mnt);
    mntDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList filist = mntDir.entryInfoList();
    QList<QUrl> mntUrlList;
    for (int i = 0; i < sidebarNum && i < filist.size(); ++i) {
        QFileInfo fi = filist.at(i);
        //华为990、9a0需要屏蔽最小系统挂载的目录
        if (fi.fileName() == "2691-6AB8")
             continue;
        mntUrlList << QUrl("file://" + fi.filePath());
    }

    QFileSystemWatcher fsw(fileDialog);
    fsw.addPath("/media/" + home + "/");
    connect(&fsw, &QFileSystemWatcher::directoryChanged, fileDialog,
    [=, &sidebarNum, &mntUrlList, &list, fileDialog](const QString path) {
        QDir wmntDir(path);
        wmntDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        QFileInfoList wfilist = wmntDir.entryInfoList();
        mntUrlList.clear();
        for (int i = 0; i < sidebarNum && i < wfilist.size(); ++i) {
                       QFileInfo fi = wfilist.at(i);
                       //华为990、9a0需要屏蔽最小系统挂载的目录
                       if (fi.fileName() == "2691-6AB8")
                            continue;
                 mntUrlList << QUrl("file://" + fi.filePath());
             }
             fileDialog->setSidebarUrls(list + mntUrlList);

             fileDialog->update();
        });

        connect(fileDialog, &QFileDialog::finished, fileDialog, [=, &list, fileDialog]() {
            fileDialog->setSidebarUrls(list);
        });

    //自己QFileDialog的用法，这里只是列子
//    fileDialog->setNameFilter(QLatin1String("*.mp3 *.ogg *.wma *.amr *.flac *.wav *.ape *.m4a *.ac3 *.aac"));

    fileDialog->setSidebarUrls(list + mntUrlList);

    fileDialog->setFileMode(QFileDialog::Directory);
    QStringList m_fileNames;
    if (QFileDialog::Accepted == fileDialog->exec())
    {
        m_fileNames = fileDialog->selectedFiles();
    }
    else
    {
        return;
    }
    foreach (QString dirPath, m_fileNames) {
        QDir dir(dirPath);
        QStringList nameFilters;
        nameFilters << "*.voc" << "*.aiff" << "*.au" << "*.dts" << "*.flv" << "*.m4r" << "*.mka" << "*.mmf" << "*.mp2" << "*.mp4" << "*.mpa" << "*.wv" << "*.voc" << "*.mp3" << "*.ogg" << "*.wma" << "*.amr" << "*.flac" << "*.wav" << "*.ape" << "*.m4a" << "*.ac3" << "*.aac";
        QStringList fileNames = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
        for(int i = 0; i < fileNames.count(); i++)
        {
            fileNames[i] = QString(dirPath + "/" + fileNames[i]);
        }
        addMusicToDatebase(fileNames);
    }

}

void TableOne::addMusicToDatebase(QStringList fileNames)
{
    //成功数量
    int successCount = 0;
    //失败数量
    int failCount = 0;
    MusicFileInformation::getInstance().addFile(fileNames);
    int allCount = MusicFileInformation::getInstance().getCount();
    QList<musicDataStruct> resList;
    resList = MusicFileInformation::getInstance().resList;

    int ret;
    foreach (const musicDataStruct date, resList)
    {
        if(nowListName != ALLMUSIC)
        {
            ret = g_db->addNewSongToPlayList(date,nowListName);   //数据库接口
        }
        else{
            ret = g_db->addMusicToLocalMusic(date);

        }
        if(ret == DB_OP_SUCC)
        {
            successCount++;
            playController::getInstance().addSongToCurList(nowListName,date.filepath);
        }
        else
        {
            failCount++;
        }
    }
    selectListChanged(nowListName);
    importFinished(successCount, failCount, allCount);
}

void TableOne::importFinished(int successCount, int m_failCount, int allCount)
{
    if(successCount > 0)
    {
        if(allCount == 1)
        {
            return;
        }
        QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("Prompt information"),tr("Success add %1 songs").arg(successCount),QMessageBox::Yes,Widget::mutual);
        warn->button(QMessageBox::Yes)->setText("确定");
        warn->exec();
    }
    else
    {
        //此处逻辑待优化，双击打开不应该提示错误
        //if(allCount == 0)
        {
            return;
        }
        QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("Prompt information"),tr("Add failed"),QMessageBox::Yes,Widget::mutual);
        warn->button(QMessageBox::Yes)->setText("确定");
        warn->exec();
    }
}

QMap<int,QString> TableOne::getSelectedTaskIdList()
{
    QTableView *m_pTableView = tableView;
    QModelIndexList selected = m_pTableView->selectionModel()->selectedRows();
//    QList<int> rows;
    QMap<int,QString> map1;
    foreach( const QModelIndex & index, selected)
    {
//        rows.append( index.row() );
        musicDataStruct date = m_model->getItem(index.row());
        map1.insert(index.row(),date.filepath);
    }
    return map1;
}

void TableOne::setHightLightAndSelect()
{
    //列表中歌曲为空时，跳过高亮判断
    if(m_model->count() == 0 ||  heightLightIndex >= m_model->count()) {
        return;
    }
    if(WidgetStyle::themeColor == 0)
    {
        for (int i=0 ;i < 4 ;i++ )
        {
            for(int j = 0; j < m_model->count() ; j++)
            {

                m_model->m_model.item(j,i)->setForeground(QBrush(QColor(Qt::black)));
            }
        }
        if(heightLightIndex != -1 && nowListName == nowPlayListName)
        {
            for (int i = 0 ; i<4 ;i++ )
            {
//                m_model->m_model.item(heightLightIndex,i)->setForeground(QBrush(QColor(55,144,250)));
                m_model->m_model.item(heightLightIndex,i)->setData(QBrush(QColor(55,144,250)),Qt::ForegroundRole);
            }
        }

    }
    if(WidgetStyle::themeColor == 1)
    {
        for (int i=0 ;i<4 ;i++ )
        {
            for(int j = 0; j < m_model->count() ; j++)
            {

                m_model->m_model.item(j,i)->setForeground(QBrush(QColor(Qt::white)));
            }
        }
        if(heightLightIndex != -1 && nowListName == nowPlayListName)
        {
            for (int i = 0 ; i<4 ;i++ )
            {
                m_model->m_model.item(heightLightIndex,i)->setData(QBrush(QColor(55,144,250)),Qt::ForegroundRole);
            }
        }
    }
}

void TableOne::changeNumber()
{
    int num = m_model->count();
//    listTotalNumLabel->setText(tr("共") + QString::number(num) + tr("首"));
    listTotalNumLabel->setText(tr("Total ") + QString::number(num) + tr(" songs"));
    if(num == 0) {
        tableView->hide();
        nullPageWidget->show();
        addMusicButton->hide();
        playAllButton->hide();
    } else {
        tableView->show();
        nullPageWidget->hide();
        addMusicButton->show();
        playAllButton->show();
    }
}
void TableOne::getMusicList()
{
    QList<musicDataStruct> resList;
    int ret;
    ret = g_db->getSongInfoListFromDB(resList,nowListName);
    if(ret == DB_OP_SUCC)
    {
        m_model->clear();
        m_model->add(resList);
        initTableViewStyle();
    }
}

void TableOne::selectListChanged(QString listname)
{
    m_model->clear();
    if(listname == tr("Song List")) {
        nowListName = ALLMUSIC;
    } else if(listname == tr("I Love")) {
        nowListName = FAV;
    } else {
        nowListName = listname;
    }
    getMusicList();
    changeNumber();
    showTitleText(listname);
    setHightLightAndSelect();
}

void TableOne::playListRenamed(QString oldName, QString newName)
{
    if(listTitleLabel->text() == oldName)
    {
        listTitleLabel->setText(newName);
        nowListName = newName;
    }
}

void TableOne::getHightLightIndex(int index, QString listName)
{
    nowPlayListName = listName;
    heightLightIndex = index;
    if(listName == nowListName){
        setHightLightAndSelect();
    } else {
        heightLightIndex = -1;
        setHightLightAndSelect();
        heightLightIndex = index;
    }
}

void TableOne::playAll(QString listName)
{
    if(listName == tr("Song List")) {
        listName = ALLMUSIC;
    } else if(listName == tr("I Love")) {
        listName = FAV;
    }
    QStringList list = m_model->getPathList(listName);
    if(list.size() == 0) {
        return;
    }
    playMusicforIndex(listName,0);
}

void TableOne::playAllButtonClicked()
{
    playAll(nowListName);
}

void TableOne::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
    QWidget::dragEnterEvent(event);
}

void TableOne::dropEvent(QDropEvent *event)
{
    auto urls = event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }
    QStringList localpath;
    for(auto &url : urls)
    {
        localpath << url.toLocalFile();
    }

    if(!localpath.isEmpty())
    {
        addMusicToDatebase(localpath);
    }


}

void TableOne::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index = tableView->indexAt(event->pos());
    int row = index.row();
    emit hoverIndexChanged(index);
}

void TableOne::resizeEvent(QResizeEvent *event)
{
    // 使用变量保存tableView一页最多可以完全显示多少条目，实时刷新，实现滑动条的显示和隐藏
    showScrollbarNumber = tableView->height()/42;
    initTableViewStyle();
}

