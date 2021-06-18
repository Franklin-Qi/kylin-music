#include "tablehistory.h"
#include "UI/mainwidget.h"

TableHistory::TableHistory(QWidget *parent) : QWidget(parent)
{
    initSetModel();
    initStyle();
    initConnect();
    initRightMenu();
    initTableStyle();
    qDebug()<< "x" << m_tableHistory->alternatingRowColors();
}

void TableHistory::initSetModel()
{
    mainVLayout = new QVBoxLayout();
    m_tableHistory = new TableBaseView;
    qDebug() << m_tableHistory;
    m_tableHistory->setContextMenuPolicy(Qt::CustomContextMenu);
    m_tableHistory->setSelectionMode(QAbstractItemView::SingleSelection);
    m_model = new MusicListModel;
    QList<musicDataStruct> resList;
    g_db->getSongInfoListFromHistoryMusic(resList);
    m_model->add(resList);
    m_model->setView(*m_tableHistory);

    historyTitileLabel = new QLabel(this);
    historyTitileLabel->setText(tr("PlayList"));
    listCountLabel = new QLabel(this);
    historyTitileWidget = new QWidget(this);
    historyTitileWidget->setFixedHeight(64);
    deleteAllBtn = new QToolButton(this);
    deleteAllBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    deleteAllBtn->setIconSize(QSize(16,16));
    deleteAllBtn->setIcon(QIcon(":/img/default/delete.png"));
    deleteAllBtn->setText(tr("Empty"));
    titleHBoxLayout = new QHBoxLayout();
    historyTitileWidget->setLayout(titleHBoxLayout);
    titleHBoxLayout->addWidget(historyTitileLabel,Qt::AlignLeft);
    titleHBoxLayout->addWidget(listCountLabel,Qt::AlignLeft);
    titleHBoxLayout->addWidget(deleteAllBtn,Qt::AlignRight);

    nullPageWidget = new QWidget(this);
    nullIconLabel = new QLabel(this);
    nullIconLabel->setPixmap(QPixmap(":/img/default/null.png").scaled(122,122));
    nullTextLabel = new QLabel(this);
    nullTextLabel->setFixedHeight(30);
    nullTextLabel->setText(tr("The playlist has no songs"));
    nullPageLayout = new QVBoxLayout();
    nullPageLayout->addStretch(1);
    nullPageLayout->addWidget(nullIconLabel);
    nullIconLabel->setAlignment(Qt::AlignHCenter);
    nullPageLayout->addWidget(nullTextLabel);
    nullTextLabel->setAlignment(Qt::AlignHCenter);
    nullPageLayout->addStretch(1);
    nullPageLayout->setSpacing(20);
    nullPageLayout->setAlignment(Qt::AlignHCenter);
    nullPageWidget->setLayout(nullPageLayout);

    mainVLayout->addWidget(historyTitileWidget,Qt::AlignTop);
//    mainVLayout->addStretch(0);
    mainVLayout->addWidget(m_tableHistory);
    mainVLayout->addWidget(nullPageWidget);
    mainVLayout->setMargin(0);
    mainVLayout->setSpacing(0);
    this->setLayout(mainVLayout);
    this->setFixedWidth(320);
//    this->setAutoFillBackground(true);
//    this->setBackgroundRole(QPalette::Base);
    changeNumber();


}
void TableHistory::initStyle()
{
    if(WidgetStyle::themeColor == 0)
    {
        qDebug() << "color " << WidgetStyle::themeColor;
        this->setStyleSheet("background:#FAFAFA");
        historyTitileWidget->setStyleSheet("background:#FAFAFA");
        nullPageWidget->setStyleSheet("background:#FAFAFA");
        m_tableHistory->setStyleSheet("border:none;background-color:#FAFAFA;");

    }
    else if(WidgetStyle::themeColor == 1)
    {
        qDebug() << "color " << WidgetStyle::themeColor;
        this->setStyleSheet("background-color:#1F2022");
        historyTitileWidget->setStyleSheet("background:#1F2022");
        nullPageWidget->setStyleSheet("background:#1F2022");
        m_tableHistory->setStyleSheet("border:none;background-color:#1F2022;");
    }

    historyTitileLabel->setStyleSheet("width:96px;height:24px;"
                                 "font-size:24px;\
                                 font-weight: 600;\
                                 line-height: 24px;");

    listCountLabel->setStyleSheet("font-weight: 400;\
                                        border:none;\
                                        line-height: 14px;");

}

void TableHistory::deleteAllClicked()
{
    QMessageBox msg(QMessageBox::Warning,tr("Prompt information"),tr("Clear the playlist?"),QMessageBox::Yes|QMessageBox::No,Widget::mutual);
    int ret = msg.exec();
    if(ret == QMessageBox::Yes)
    {
        int result = g_db->emptyHistoryMusic();
        if(result == DB_OP_SUCC) {
            m_model->clear();
            changeNumber();
        }
        else {
            return;
        }

    }
    else
    {
        return;
    }

}
void TableHistory::slotPlayIndexChanged(int index, QString listname)
{
    if(listname == HISTORY) {
        nowPlayListName = HISTORY;
        nowPlayIndex = index;
        refreshHistoryTable();
    }
    else {
        nowPlayListName = listname;
    }
}

void TableHistory::slotPlayPathChanged(QString songPath)
{
    if(nowPlayListName != HISTORY && songPath != "") {
        int ret = g_db->addMusicToHistoryMusic(songPath);
        if(ret == DB_OP_SUCC) {
            nowPlayIndex = 0;
            refreshHistoryTable();
        }
        else {
            return;
        }
    }
}

void TableHistory::initTableStyle()
{
    m_tableHistory->hideColumn(2);
    m_tableHistory->setColumnWidth(0,150);
    m_tableHistory->setColumnWidth(1,80);
    m_tableHistory->setColumnWidth(3,40);
    m_tableHistory->setAutoFillBackground(true);
    m_tableHistory->verticalHeader()->setVisible(false);// 垂直不可见
    m_tableHistory->horizontalHeader()->setVisible(false);
    m_tableHistory->setAlternatingRowColors(false);

}



void TableHistory::initConnect()
{
    connect(deleteAllBtn,&QToolButton::clicked,this,&TableHistory::deleteAllClicked);
    connect(m_tableHistory,&TableHistory::customContextMenuRequested,this,&TableHistory::showRightMenu);
    connect(m_tableHistory,&TableBaseView::doubleClicked,this,&TableHistory::playSongs);
    connect(&playController::getInstance(),&playController::currentIndexAndCurrentList,this,&TableHistory::slotPlayIndexChanged);
    connect(&playController::getInstance(),&playController::singalChangePath,this,&TableHistory::slotPlayPathChanged);


}
void TableHistory::showHistroryPlayList()
{
    qDebug() << m_tableHistory->alternatingRowColors();
    if(this->isHidden())
    {
        this->show();
    }
    else
    {
        this->hide();
    }
    qDebug() << m_tableHistory << m_tableHistory->alternatingRowColors();
    m_tableHistory->setAlternatingRowColors(false);
}
void TableHistory::changeNumber()
{
    int num = m_model->count();
    listCountLabel->setText(tr("Total ") + QString::number(num) + tr(" songs"));
    if(num == 0) {
        nullPageWidget->show();
        m_tableHistory->hide();
        isHightLight = false;
    } else{
        nullPageWidget->hide();
        m_tableHistory->show();
    }
}
void TableHistory::refreshHistoryTable()
{
    m_model->clear();
    QList<musicDataStruct> resList;
    g_db->getSongInfoListFromHistoryMusic(resList);
    m_model->add(resList);
    changeNumber();
    initTableStyle();
    qDebug() << "refreshHistoryTable" << nowPlayIndex;
    setHighlight(nowPlayIndex);
}
void TableHistory::initRightMenu()
{
    m_menu = new QMenu;
    playRow = new QAction(tr("Play"));
    removeRow = new QAction(tr("Delete"));
    playNextRow = new QAction(tr("Play the next one"));

    m_menu->addAction(playRow);
    m_menu->addAction(removeRow);
//    m_menu->addAction(playNextRow);
    connect(playRow,&QAction::triggered,this,&TableHistory::playSongs);
    connect(removeRow,&QAction::triggered,this,&TableHistory::deleteSongs);
    connect(playNextRow,&QAction::triggered,this,&TableHistory::playNextRowClicked);

//    m_tableHistory->installEventFilter(this);
}
void TableHistory::showRightMenu(const QPoint &pos)
{
    QModelIndex index = m_tableHistory->indexAt(pos);
    if(index.row() < 0)
    {
        return;
    }
    qDebug() << "鼠标位置 " << pos << index.row();
    m_menu->exec(QCursor::pos());
}
void TableHistory::playSongs()
{
    qDebug() << "播放";
    int index = m_tableHistory->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    qDebug() << "m_tableHistory->selectedIndexes();" << index << date.filepath;
    QStringList pathList;
    pathList = m_model->getPathList(nowListName);
    qDebug() << "调用播放接口，设置" << nowListName <<"为当前播放列表";
    playController::getInstance().setCurPlaylist(nowListName,pathList);
    qDebug() << "调用播放接口，设置" << index <<"为当前播放歌曲索引";
    playController::getInstance().play(nowListName,index);
    emit signalHistoryPlaying();
}
void TableHistory::deleteSongs()
{
    int index = m_tableHistory->currentIndex().row();
//    if(index == nowPlayIndex && nowPlayListName != HISTORY) {
//        QMessageBox::warning(Widget::mutual,tr("Prompt information"),tr("歌曲列表或歌单中正在播放此歌曲，无法删除~"));
//        return;
//    }
    musicDataStruct date = m_model->getItem(index);
//    g_db->delMusicFromPlayList(date.filepath,nowListName);
    playController::getInstance().removeSongFromCurList(nowListName,index);
    g_db->delMusicFromHistoryMusic(date.filepath);
    if(index == nowPlayIndex && nowPlayListName != HISTORY) {
        nowPlayIndex = -1;
    }
    refreshHistoryTable();
}

void TableHistory::playNextRowClicked()
{

}

void TableHistory::setHighlight(int index)
{
    qDebug() <<" 历史列表高亮" << index << m_model->count();
    if(m_model->count() == 0 ||  index >= m_model->count()) {
        qDebug() << "退出";
        return;
    }
    qDebug() << "-------------111";
    if(WidgetStyle::themeColor == 0)
    {
        for (int i = 0 ;i<4 ;i++ )
        {
            for(int j = 0; j < m_model->count() ; j++)
            {

                m_model->m_model.item(j,i)->setForeground(QBrush(QColor(Qt::black)));
            }
        }
        if(index != -1)
        {
            for (int i = 0 ; i<4 ;i++ )
            {
                m_model->m_model.item(index,i)->setData(QBrush(QColor(55,144,250)),Qt::ForegroundRole);
            }
        }

    }
    if(WidgetStyle::themeColor == 1)
    {
        for (int i = 0; i < 4; i++ )
        {
            for(int j = 0; j < m_model->count() ; j++)
            {

                m_model->m_model.item(j,i)->setForeground(QBrush(QColor(Qt::white)));
            }
        }
        if(index != -1)
        {
            for (int i = 0 ; i<4 ;i++ )
            {
//                m_model->m_model.item(heightLightIndex,i)->setForeground(QBrush(QColor(55,144,250)));
                m_model->m_model.item(index,i)->setData(QBrush(QColor(55,144,250)),Qt::ForegroundRole);
            }
        }
    }
}
