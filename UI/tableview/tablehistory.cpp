#include "tablehistory.h"
#include "UI/mainwidget.h"

#include <QApplication>

#define PT_15 15

TableHistory::TableHistory(QWidget *parent) : QDialog(parent)
{
    initSetModel();
    initStyle();
    initConnect();
    initRightMenu();
    initTableStyle();
    qDebug() << "播放历史列表初始化完成.";
}

void TableHistory::initSetModel()
{
    //禁用界面拖拽
//    this->setProperty("useStyleWindowManager", false);
    this->setFixedWidth(320);
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);
//    this->setAutoFillBackground(true);
//    this->setBackgroundRole(QPalette::Base);

    m_view = new CustomTreeView(true);
    m_view->setObjectName("m_tableHistory");
    m_view->setIconSize(QSize(0, 0));

    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
//    m_tableHistory->setShowGrid(false);
//    m_tableHistory->verticalHeader()->setDefaultSectionSize(40);
    m_model = new MusicListModel;
    QList<musicDataStruct> resList;
    g_db->getSongInfoListFromHistoryMusic(resList);
    m_model->add(resList);
    m_model->setView(*m_view);

    historyTitileLabel = new QLabel(this);
    historyTitileLabel->setText(tr("History"));
    listCountLabel = new QLabel(this);
    deleteAllBtn = new QToolButton(this);
    deleteAllBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    deleteAllBtn->setIconSize(QSize(16,16));
    deleteAllBtn->setIcon(QIcon::fromTheme("edit-delete-symbolic"));
    deleteAllBtn->setText(tr("Empty"));
    titleHBoxLayout = new QHBoxLayout();
    titleHBoxLayout->addWidget(historyTitileLabel,0,Qt::AlignLeft);
    titleHBoxLayout->addWidget(listCountLabel,1,Qt::AlignBottom);
    titleHBoxLayout->addWidget(deleteAllBtn,Qt::AlignRight);
    titleHBoxLayout->setContentsMargins(16,16,16,12);

    historyTitileWidget = new QWidget(this);
    historyTitileWidget->setFixedHeight(64);
    historyTitileWidget->setLayout(titleHBoxLayout);


    nullPageWidget = new QWidget(this);
    nullIconLabel = new QLabel(this);
    if (WidgetStyle::themeColor == 1) {
        // black theme
        nullIconLabel->setPixmap(QPixmap(":/img/default/defaultIconDark.png").scaled(128, 128));
    } else {
        nullIconLabel->setPixmap(QPixmap(":/img/default/defaultIconLight.png").scaled(128, 128));
    }
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
    nullPageLayout->setAlignment(Qt::AlignCenter);
    nullPageWidget->setLayout(nullPageLayout);


    // 歌曲历史列表
    tableHBoxLayout = new QHBoxLayout();
    tableHBoxLayout->addWidget(m_view);
    tableHBoxLayout->setContentsMargins(16, 0, 16, 0);

    tableWidget = new QWidget(this);
    tableWidget->setLayout(tableHBoxLayout);

    mainVLayout = new QVBoxLayout();
    mainVLayout->addWidget(historyTitileWidget,Qt::AlignTop);
//    mainVLayout->addStretch(0);
    mainVLayout->addWidget(tableWidget);
    mainVLayout->addWidget(nullPageWidget);
    mainVLayout->setMargin(0);
    mainVLayout->setSpacing(0);
    this->setLayout(mainVLayout);

    changeNumber();
}
void TableHistory::initStyle()
{
    m_view->setStyleSheet("#m_tableHistory{border:none;}");
    historyTitileLabel->setStyleSheet("height:20px;\
                                 font-weight: 600;\
                                 line-height: 20px;");

    listCountLabel->setStyleSheet("font-weight: 400;color:#8C8C8C;\
                                  line-height: 14px;");
    deleteAllBtn->setStyleSheet("color:#8F9399;background:transparent;");
}

void TableHistory::slotLableSetFontSize(int size)
{
    //默认大小12px,换算成pt为9
    double lableBaseFontSize = PT_15;//魔鬼数字，自行处理
    double nowFontSize = lableBaseFontSize * double(size) / 11;//11为系统默认大小，魔鬼数字，自行处理
    QFont font;
    font.setPointSizeF(nowFontSize);
    historyTitileLabel->setFont(font);
}

void TableHistory::deleteAllClicked()
{
    QMessageBox msg(QMessageBox::Warning,tr("Prompt information"),tr("Clear the playlist?"),QMessageBox::Yes|QMessageBox::No,Widget::mutual);
    int ret = msg.exec();
    if(ret == QMessageBox::Yes) {
        for(int i = 0;i < m_model->count();i++) {
            playController::getInstance().removeSongFromCurList(HISTORY, 0);
        }

        int result = g_db->emptyHistoryMusic();
        if(result == DB_OP_SUCC) {
            m_model->clear();
            changeNumber();
        } else {
            return;
        }

    } else {
        return;
    }

}
void TableHistory::slotPlayIndexChanged(int index, QString listname)
{
    if(listname == HISTORY) {
        nowPlayListName = HISTORY;
        nowPlayIndex = index;
        noRefreshHistory();
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
    else if(songPath == "")
    {
        nowPlayIndex = -1;
        noRefreshHistory();
    }
    else
    {
        return;
    }
}

bool TableHistory::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result);

//    return  QDialog::nativeEvent(eventType, message, result);

    if(eventType != "xcb_generic_event_t")
    {
        return false;
    }

    xcb_generic_event_t *event = (xcb_generic_event_t*)message;
    switch (event->response_type & ~0x80)
    {
        case XCB_FOCUS_OUT:
            // -playHistoryPosWidth (负数原因：因为计算按钮的右下角点)
            QRect rect(playHistoryPosX, playHistoryPosY, -playHistoryPosWidth, playHistoryPosHeight);
            if(rect.contains(QCursor::pos(), false)) {
                return false;
            }

            if (this->geometry().contains(QCursor::pos(), false)) {
                return false;
            }

            this->hide();
            Q_EMIT signalHistoryBtnChecked(false);
            break;
    }
    return false;
}

void TableHistory::playSongsDoubleClicked(QModelIndex index)
{
    int indexRow = index.row();
    QStringList pathList;

    m_model->getItem(indexRow);
    pathList = m_model->getPathList(nowListName);

    playController::getInstance().setCurPlaylist(nowListName,pathList);
    playController::getInstance().play(nowListName,indexRow);

    Q_EMIT signalHistoryPlaying();

}

void TableHistory::initTableStyle()
{
    m_view->hideColumn(2);
    m_view->setColumnWidth(0, 146);
    m_view->setColumnWidth(1, 78);
    m_view->setColumnWidth(3, 64);
    m_view->setAutoFillBackground(true);
    m_view->setHeaderHidden(true);
//    m_tableHistory->verticalHeader()->setVisible(false);// 垂直不可见
//    m_tableHistory->horizontalHeader()->setVisible(false);
//    m_tableHistory->setAlternatingRowColors(false);

}

void TableHistory::initConnect()
{
    connect(deleteAllBtn,&QToolButton::clicked,this,&TableHistory::deleteAllClicked);
    connect(m_view,&TableHistory::customContextMenuRequested,this,&TableHistory::showRightMenu);
    connect(m_view,&CustomTreeView::doubleClicked,this,&TableHistory::playSongs);
    connect(m_view,&CustomTreeView::doubleClickedSignal,this,&TableHistory::playSongsDoubleClicked);
    connect(&playController::getInstance(),&playController::currentIndexAndCurrentList,this,&TableHistory::slotPlayIndexChanged);
    connect(&playController::getInstance(),&playController::singalChangePath,this,&TableHistory::slotPlayPathChanged);


}
void TableHistory::showHistroryPlayList()
{
    if(this->isVisible())
    {
        this->hide();
        signalHistoryBtnChecked(false);
    }
    else
    {
        Widget::mutual->movePlayHistoryWid();
        this->show();
        signalHistoryBtnChecked(true);
        this->raise();
    }
//    m_tableHistory->setAlternatingRowColors(false);
}
void TableHistory::changeNumber()
{
    int num = m_model->count();
    listCountLabel->setText(QString::number(num) + tr(" songs"));
    if(num == 0) {
        nullPageWidget->show();
        tableWidget->hide();
        isHightLight = false;
    } else{
        nullPageWidget->hide();
        tableWidget->show();
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
    setHighlight(nowPlayIndex);
}
void TableHistory::noRefreshHistory()
{
    changeNumber();
    initTableStyle();
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

}
void TableHistory::showRightMenu(const QPoint &pos)
{
    QModelIndex index = m_view->indexAt(pos);
    if(index.row() < 0)
    {
        return;
    }
    m_menu->exec(QCursor::pos());
}


/**
 * @brief TableHistory::playSongs 播放音乐
 */
void TableHistory::playSongs()
{
    int index = m_view->currentIndex().row();
    QStringList pathList;

    m_model->getItem(index);
    pathList = m_model->getPathList(nowListName);

    playController::getInstance().setCurPlaylist(nowListName,pathList);
    playController::getInstance().play(nowListName,index);

    Q_EMIT signalHistoryPlaying();
}
void TableHistory::deleteSongs()
{
    int index = m_view->currentIndex().row();

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

void TableHistory::changePlayHistoryPos(int posX, int posY, int width, int height)
{
    playHistoryPosX = posX;
    playHistoryPosY = posY;
    playHistoryPosWidth = width;
    playHistoryPosHeight = height;
}


/**
 * @brief TableHistory::setHighlight 高亮显示已选择的列表项
 * 采用高亮显示： QApplication::palette().highlight().color())
 * @param index
 */
void TableHistory::setHighlight(int index)
{
    if (WidgetStyle::themeColor == 1) {
        // black theme
        nullIconLabel->setPixmap(QPixmap(":/img/default/defaultIconDark.png").scaled(128, 128));
    } else {
        nullIconLabel->setPixmap(QPixmap(":/img/default/defaultIconLight.png").scaled(128, 128));
    }

    //播放历史模块高亮
    if(m_model->count() == 0 ||  index >= m_model->count()) {
        return;
    }

    QColor highlightColor(QApplication::palette().highlight().color());

    if(WidgetStyle::themeColor == 0) {
        for (int i = 0 ;i<4 ;i++ ) {
            for(int j = 0; j < m_model->count() ; j++) {

                m_model->m_model.item(j,i)->setForeground(QBrush(QColor(Qt::black)));
            }
        }

        if(index != -1) {
            for (int i = 0 ; i<4 ;i++ ) {
                m_model->m_model.item(index,i)->setData(QBrush(highlightColor),Qt::ForegroundRole);
            }
        }

    }

    if(WidgetStyle::themeColor == 1) {
        for (int i = 0; i < 4; i++ ) {
            for(int j = 0; j < m_model->count() ; j++) {

                m_model->m_model.item(j,i)->setForeground(QBrush(QColor(Qt::white)));
            }
        }

        if(index != -1) {
            for (int i = 0 ; i<4 ;i++ ) {
                m_model->m_model.item(index,i)->setData(QBrush(highlightColor),Qt::ForegroundRole);
            }
        }
    }
}
