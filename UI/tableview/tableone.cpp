#include "tableone.h"

TableOne::TableOne(QString listName, QWidget *parent) : QWidget(parent)
{
    nowListName = listName;
    initUI();
    initRightMenu();
    initConnect();
    initStyle();

}

void TableOne::initStyle()
{
    listTitleLabel->setStyleSheet("width:96px;height:24px;"
                                 "font-size:24px;\
                                 font-weight: 600;\
                                 line-height: 24px;");

    listTotalNumLabel->setStyleSheet("font-weight: 400;\
                                        border:none;\
                                        line-height: 14px;");

}

void TableOne::initTableViewStyle()
{
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->setColumnWidth(3,50);

//    tableView->setStyleSheet("QTableView{border:none;}"
//                             "QTableView::item:selected{color:black;background:#F0F0F0;border:none;}");

    tableView->horizontalHeader()->setVisible(false);// 水平不可见
    tableView->verticalHeader()->setVisible(false);// 垂直不可见
    tableView->setAutoScroll(true);
    tableView->verticalScrollBarPolicy();
    tableView->hideColumn(4);
    tableView->hideColumn(5);
    tableView->hideColumn(6);
    tableView->show();
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    tableView->setAutoFillBackground(true);
    tableView->setAlternatingRowColors(false);
}

void TableOne::initUI()
{
    QVBoxLayout *tableLayout = new QVBoxLayout();
    this->setLayout(tableLayout);

    listTitleLabel = new MyLabel(this);
    listTotalNumLabel = new QLabel(this);
    addMusicButton = new QToolButton(this);
    listTitleHBoxLayout = new QHBoxLayout();

    QWidget *titleWid = new QWidget(this);
    titleWid->setLayout(listTitleHBoxLayout);
    listTitleHBoxLayout->setAlignment(Qt::AlignLeft);
    listTitleHBoxLayout->addWidget(listTitleLabel);
    listTitleHBoxLayout->addWidget(listTotalNumLabel);
    listTitleHBoxLayout->addStretch(0);
    listTitleHBoxLayout->addWidget(addMusicButton,1,Qt::AlignRight);
    listTitleHBoxLayout->setMargin(0);
    listTitleHBoxLayout->setSpacing(0);
    titleWid->setFixedHeight(40);

    addMusicButton->setText(tr("Add"));
    addMusicButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addMusicButton->setIconSize(QSize(16,16));
//    addMusicButton->setText(tr("   Add"));
    addMusicButton->setFixedSize(100,30);
    addMusicButton->setPopupMode(QToolButton::InstantPopup);
    add_menu = new QMenu(this);
    addMusicFileAction = new QAction(this);
    addDirMusicAction = new QAction(this);
    addMusicFileAction->setText("添加歌曲文件");
    addDirMusicAction->setText("添加文件夹");
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
    tableTitleLayout->addWidget(singerTitle,1);
    tableTitleLayout->addWidget(albumTitle,1);
    tableTitleLayout->addStretch(0);
    tableTitleLayout->addWidget(songtimeTitle,Qt::AlignRight);
    tableTitleLayout->addSpacing(10);
    tableTitleLayout->setSpacing(0);
    tableTitleLayout->setContentsMargins(3.5,0,2.5,0);
    songNameTitle->setStyleSheet("color:#8F9399;");
    singerTitle->setStyleSheet("color:#8F9399;");
    albumTitle->setStyleSheet("color:#8F9399;");
    songtimeTitle->setStyleSheet("color:#8F9399;");


    tableView = new TableBaseView();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置按ctrl键选中多个
    QList<musicDataStruct> resList;
    int ret;
    if(nowListName == tr("Song List"))
    {
        ret = g_db->getSongInfoListFromLocalMusic(resList);
    }
    else
    {
        ret = g_db->getSongInfoListFromPlayList(resList,nowListName);
    }
    m_model = new MusicListModel;
    m_model->add(resList);
    m_model->setView(*tableView);

    m_musicWidget = new QWidget(this);
    m_historyLayout = new QVBoxLayout();
    m_musicWidget->setLayout(m_historyLayout);
    m_historyLayout->addWidget(tableTitleWidget);
    m_historyLayout->addWidget(tableView);
    m_historyLayout->setSpacing(8);
    m_historyLayout->setMargin(0);

    nullPageWidget = new QWidget(this);
    nullPageHLayout = new QHBoxLayout();
    nullPageVLayout = new QVBoxLayout();
    nullPageIconLabel = new QLabel(this);
    nullPageTextLabel = new QLabel(this);
    n_addDirMusicButton = new QPushButton(this);
    n_addMusicButton = new QPushButton(this);
    nullPageTextLabel->setText(tr("There are no songs!"));
    n_addMusicButton->setText(tr("Add Local Songs"));
    n_addDirMusicButton->setText(tr("Add Local Folder"));
    nullPageIconLabel->setPixmap(QPixmap(":/img/default/pict1.png").scaled(164,164));

    nullPageWidget->setLayout(nullPageVLayout);
    nullPageVLayout->addStretch();
    nullPageVLayout->addWidget(nullPageIconLabel);
    nullPageIconLabel->setAlignment(Qt::AlignHCenter);
    nullPageVLayout->addWidget(nullPageTextLabel);
    nullPageTextLabel->setAlignment(Qt::AlignHCenter);
    nullPageVLayout->addLayout(nullPageHLayout);
    nullPageVLayout->addStretch();
    nullPageHLayout->addWidget(n_addMusicButton);
    nullPageHLayout->addWidget(n_addDirMusicButton);
    nullPageVLayout->setAlignment(Qt::AlignCenter);

    tableLayout->addWidget(titleWid,Qt::AlignTop);
    tableLayout->addSpacing(10);
    tableLayout->addWidget(m_musicWidget);
    tableLayout->addWidget(nullPageWidget);

    initTableViewStyle();
    listTitleLabel->setFixedWidth(120);
    listTotalNumLabel->setFixedWidth(120);
    listTitleLabel->setText(nowListName);
    changeNumber();
}

void TableOne::initConnect()
{
    connect(tableView,&TableBaseView::doubleClicked,this,&TableOne::playSongs);
    connect(tableView,&TableBaseView::customContextMenuRequested,this,&TableOne::showRightMenu);
//    connect(addMusicButton,&QToolButton::clicked,this,&TableOne::addMusicSlot);
    connect(addMusicFileAction,&QAction::triggered,this,&TableOne::addMusicSlot);
    connect(addDirMusicAction,&QAction::triggered,this,&TableOne::addDirMusicSlot);
    connect(this,&TableOne::countChanges,this,&TableOne::changeNumber);
    connect(&playController::getInstance(),&playController::currentIndexAndCurrentList,this,&TableOne::getHightLightIndex);
    connect(n_addMusicButton,&QPushButton::clicked,this,&TableOne::addMusicSlot);
    connect(n_addDirMusicButton,&QPushButton::clicked,this,&TableOne::addDirMusicSlot);
}

void TableOne::initRightMenu()
{
    m_menu = new QMenu(tableView);

    playRow = new QAction(tr("Play"));
    removeRow = new QAction(tr("Delete"));
    showInfoRow = new QAction(tr("View song information"));
    addToOtherListMenu = new QMenu(tr("Add to songlist"));
    connect(playRow,&QAction::triggered,this,&TableOne::playSongs);
    connect(removeRow,&QAction::triggered,this,&TableOne::deleteSongs);
    connect(showInfoRow,&QAction::triggered,this,&TableOne::showInfo);
    connect(addToOtherListMenu,&QMenu::triggered,this,&TableOne::addToOtherList);

    tableView->installEventFilter(this);
}

void TableOne::showRightMenu(const QPoint &pos)
{
    QModelIndex index = tableView->indexAt(pos);
    if(index.row() < 0)
    {
        return;
    }
    qDebug() << "鼠标位置 " << pos << index.row();
//    m_menu->move(0,0);
    m_menu->addAction(playRow);      //添加动作到菜单
    m_menu->addMenu(addToOtherListMenu);
    m_menu->addAction(removeRow);  //添加动作到菜单
    m_menu->addAction(showInfoRow); //添加动作到菜单
    QMap<int,QString> map1 = getSelectedTaskIdList();
    addToOtherListMenu->clear();
    QStringList allList;
    int ret = g_db->getPlayList(allList);
    if(ret == 0)
    {
        for(int i = 0 ;i < allList.size() ;i++)
        {
            qDebug() << allList[i];
            QAction *listNameAct = new QAction;
            addToOtherListMenu->addAction(listNameAct);
            listNameAct->setText(allList[i]);
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

void TableOne::deleteSongs()
{
    QMap<int,QString> map1 = getSelectedTaskIdList();
    while (!map1.empty()) {
        QMap<int,QString>::iterator iter;
        iter = map1.end();
        iter--;
        qDebug() << "Iterator " << iter.key(); // 迭代器
        int ret;
        if(nowListName != tr("Song List"))
        {
           ret = g_db->delMusicFromPlayList(iter.value(),nowListName);
        }
        else
        {
//           ret = g_db->delMusicFromLocalMusic(iter.value());
           ret = g_db->delSongFromEveryWhere(iter.value());

        }
        if(ret == 0)
        {
//            m_model->remove(iter.key());
            playController::getInstance().removeSongFromCurList(nowListName,iter.key());
            qDebug() << "删除结果" << ret << "filepath" <<iter.value();
//            if(nowListName == tr("I Love"))
            if(nowListName == "我喜欢")
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
}

void TableOne::playSongs()
{
    qDebug() << "播放";
    int index = tableView->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    qDebug() << "tableView->selectedIndexes();" << index << date.filepath;
    QStringList pathList;
    pathList = m_model->getPathList();
    playController::getInstance().setCurPlaylist(nowListName,pathList);
    playController::getInstance().play(nowListName,index);
    g_db->addMusicToHistoryMusic(date.filepath);
    emit addMusicToHistoryListSignal();
}

void TableOne::showInfo()
{
    qDebug() << "显示歌曲信息";
    int index = tableView->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    infoDialog = new MusicInfoDialog(date);
    infoDialog->exec();
    qDebug() << "歌曲信息：" << date.title << date.filepath << date.size;
}

void TableOne::addToOtherList(QAction *listNameAction)
{
    QString listName = listNameAction->text();
    qDebug() << listName;
    QMap<int,QString> map1 = getSelectedTaskIdList();
    QMap<int,QString>::Iterator it = map1.begin();
    while(it != map1.end())
    {
        qDebug()<<it.key()<<"\t"<<it.value();
        int ret = g_db->addMusicToPlayList(it.value(),listName);
        if(ret == 0)
        {
            playController::getInstance().addSongToCurList(listName,it.value());
//            if(listName == tr("I Love"))
            qDebug() << "------------------- listName -------------" << listName;
            if(listName == "我喜欢")
            {
                qDebug() << "---------------- it.value() it.value() ------------" << it.value();
                emit addILoveFilepathSignal(it.value());
            }
        }else{
            QMessageBox::warning(this,tr("Prompt information"),tr("Add failed!"));
        }
        it++;
    }
}
void TableOne::addMusicSlot()
{
    QFileDialog *fileDialog = new QFileDialog;

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
                 mntUrlList << QUrl("file://" + fi.filePath());
             }
             fileDialog->setSidebarUrls(list + mntUrlList);

             fileDialog->update();
        });

    connect(fileDialog, &QFileDialog::finished, fileDialog, [=, &list, fileDialog]() {
        fileDialog->setSidebarUrls(list);
    });
    fileDialog->setSidebarUrls(list + mntUrlList);
    fileDialog->setNameFilter("音乐文件(*.mp3 *.ogg *.wma *.flac *.wav *.ape *.amr *.m4a *.ac3 *.aac *.mid)");
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

    for( int i = 0; i < songFiles.length(); i++ )
    {
        songFiles.at(i);
    }
    qDebug() << "songFiles" << songFiles;
    addMusicToDatebase(songFiles);
}

void TableOne::addDirMusicSlot()
{
    QFileDialog *fileDialog = new QFileDialog;

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
                 mntUrlList << QUrl("file://" + fi.filePath());
             }
             fileDialog->setSidebarUrls(list + mntUrlList);

             fileDialog->update();
        });

        connect(fileDialog, &QFileDialog::finished, fileDialog, [=, &list, fileDialog]() {
            fileDialog->setSidebarUrls(list);
        });

    //自己QFileDialog的用法，这里只是列子
//    fileDialog->setNameFilter(QLatin1String("*.mp3 *.ogg *.wma *.flac *.wav *.ape *.amr *.m4a *.ac3 *.aac *.mid"));

    fileDialog->setSidebarUrls(list + mntUrlList);

    fileDialog->setFileMode(QFileDialog::Directory);
    QStringList m_fileNames;
    if (fileDialog->exec())
    {
        m_fileNames = fileDialog->selectedFiles();
    }
    qDebug() << m_fileNames;
    foreach (QString dirPath, m_fileNames) {
        QDir dir(dirPath);
        QStringList nameFilters;
        nameFilters << "*.mp3" << "*.ogg" << "*.wma" << "*.flac" << "*.wav" << "*.ape" <<  "*.amr" << "*.m4a" << "*.ac3" << "*.aac" << "*.mid";
        QStringList fileNames = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
        for(int i = 0; i < fileNames.count(); i++)
        {
            fileNames[i] = QString(dirPath + "/" + fileNames[i]);
        }
        qDebug() << fileNames;
        addMusicToDatebase(fileNames);
    }

}

void TableOne::addMusicToDatebase(QStringList fileNames)
{
    MusicFileInformation::getInstance().addFile(fileNames);
    QList<musicDataStruct> resList;
    resList = MusicFileInformation::getInstance().resList;
    int ret;
    foreach (const musicDataStruct date, resList) {
        if(nowListName != tr("Song List")){
            ret = g_db->addNewSongToPlayList(date,nowListName);   //数据库接口
        }
        else{
            ret = g_db->addMusicToLocalMusic(date);

        }
        if(ret == 0){
            playController::getInstance().addSongToCurList(nowListName,date.filepath);
//            m_model->add(date);
            selectListChanged(nowListName);
//            emit countChanges();
        }
        else{
            QMessageBox msg;
            msg.setWindowTitle(tr("Prompt information"));
            msg.setText(date.filepath+tr("Failed to add song file!"));
            msg.exec();
            qDebug() << "添加数据库失败.";
        }
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
    if(WidgetStyle::themeColor == 0)
    {
        for (int i=0 ;i<4 ;i++ )
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
    if(num == 0 && nowListName == tr("Song List")) {
        m_musicWidget->hide();
        nullPageWidget->show();
        addMusicButton->hide();
    } else {
        m_musicWidget->show();
        nullPageWidget->hide();
        addMusicButton->show();
    }
}

QList<musicDataStruct> TableOne::getMusicList()
{
    QList<musicDataStruct> resList;
    int ret;
    if(nowListName == tr("Song List"))
    {
        ret = g_db->getSongInfoListFromLocalMusic(resList);

    }
    else
    {
        ret = g_db->getSongInfoListFromPlayList(resList,nowListName);
    }
    m_model->clear();
    m_model->add(resList);
    initTableViewStyle();

    return resList;
}
void TableOne::selectListChanged(QString listname)
{
    qDebug() << "歌单名" <<listname;
    m_model->clear();
    nowListName = listname;
    getMusicList();
    changeNumber();
    listTitleLabel->setText(listname);
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
    }
}

void TableOne::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index = tableView->indexAt(event->pos());
    int row = index.row();
    emit hoverIndexChanged(index);
}
