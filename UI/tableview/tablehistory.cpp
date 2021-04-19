#include "tablehistory.h"

TableHistory::TableHistory(QWidget *parent) : QWidget(parent)
{
    initSetModel();
    initStyle();
    initConnect();
    initRightMenu();
    initTableStyle();
}

void TableHistory::initSetModel()
{

    mainVLayout = new QVBoxLayout();
//    hBooxLayout = new QHBoxLayout();
    m_tableHistory = new TableBaseView;
    m_tableHistory->setContextMenuPolicy(Qt::CustomContextMenu);
    m_model = new MusicListModel;
    QList<musicDataStruct> resList;
    g_db->getSongInfoListFromHistoryMusic(resList);
    m_model->add(resList);
    m_model->setView(*m_tableHistory);

    historyTitileLabel = new QLabel(this);
    historyTitileLabel->setText(tr("播放列表"));
    listCountLabel = new QLabel(this);
    int num = m_model->count();
    listCountLabel->setText(tr("共") + QString::number(num) + tr("首"));

    historyTitileWidget = new QWidget(this);

    deleteAllBtn = new QToolButton(this);
    //文字在图标旁边
    deleteAllBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    deleteAllBtn->setIconSize(QSize(16,16));
    deleteAllBtn->setIcon(QIcon(":/img/default/delete.png"));
    deleteAllBtn->setText(tr("清空"));

    titleHBoxLayout = new QHBoxLayout();
    historyTitileWidget->setLayout(titleHBoxLayout);
    titleHBoxLayout->addWidget(historyTitileLabel,Qt::AlignLeft);
    titleHBoxLayout->addWidget(listCountLabel,Qt::AlignLeft);
    titleHBoxLayout->addWidget(deleteAllBtn,Qt::AlignRight);
    mainVLayout->addWidget(historyTitileWidget);
    mainVLayout->addWidget(m_tableHistory);
    this->setLayout(mainVLayout);

//    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
//    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
//    effect->setColor(QColor(220,220,220));       //设置阴影颜色，也可以setColor(QColor(220,220,220))
//    effect->setBlurRadius(25);        //设定阴影的模糊半径，数值越大越模糊
//    this->setGraphicsEffect(effect);
    this->setFixedWidth(320);
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);

}
void TableHistory::initStyle()
{

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
//    QMessageBox msgBox;
//    msgBox.setText(tr("是否清空播放列表？"));
    QMessageBox msg(QMessageBox::Warning,tr("提示信息"),tr("是否清除播放列表？"),QMessageBox::Yes|QMessageBox::No,this);
    int ret = msg.exec();
    if(ret == QMessageBox::Yes)
    {
        m_model->clear();
        changeNumber();
    }
    else
    {
        return;
    }

}

void TableHistory::initTableStyle()
{
    m_tableHistory->hideColumn(2);
    m_tableHistory->hideColumn(4);
    m_tableHistory->hideColumn(5);
    m_tableHistory->hideColumn(6);
//    m_tableHistory->setStyleSheet("QTableView{border:none;}"
//                                  "QTableView::item:selected{color:black;background:#F0F0F0;border:none;}");
    m_tableHistory->setAlternatingRowColors(false);
}



void TableHistory::initConnect()
{
    connect(deleteAllBtn,&QToolButton::clicked,this,&TableHistory::deleteAllClicked);
    connect(m_tableHistory,&TableHistory::customContextMenuRequested,this,&TableHistory::showRightMenu);
    connect(m_tableHistory,&TableBaseView::doubleClicked,this,&TableHistory::playSongs);

}
void TableHistory::showHistroryPlayList()
{
    if(this->isHidden())
    {
        this->show();
    }
    else
    {
        this->hide();
    }

}
void TableHistory::changeNumber()
{
    int num = m_model->count();
    listCountLabel->setText(tr("共") + QString::number(num) + tr("首"));
}

void TableHistory::addMusicToHistoryListSlot()
{
    m_model->clear();
    QList<musicDataStruct> resList;
    g_db->getSongInfoListFromHistoryMusic(resList);
    m_model->add(resList);
    initTableStyle();
    changeNumber();
}
void TableHistory::initRightMenu()
{
    m_menu = new QMenu(m_tableHistory);
    playRow = new QAction("播放");
    removeRow = new QAction("删除");
    playNextRow = new QAction("下一首播放");

    m_menu->addAction(playRow);
    m_menu->addAction(removeRow);
    m_menu->addAction(playNextRow);
    connect(playRow,&QAction::triggered,this,&TableHistory::playSongs);
    connect(removeRow,&QAction::triggered,this,&TableHistory::deleteSongs);
    connect(playNextRow,&QAction::triggered,this,&TableHistory::playNextRowClicked);

//    m_tableHistory->installEventFilter(this);
}
void TableHistory::showRightMenu(const QPoint &pos)
{
    qDebug() << "鼠标位置 " << pos;
    m_menu->exec(QCursor::pos());
}
void TableHistory::playSongs()
{
    qDebug() << "播放";
    int index = m_tableHistory->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    qDebug() << "m_tableHistory->selectedIndexes();" << index << date.filepath;
    QStringList pathList;
    pathList = m_model->getPathList();
    playController::getInstance().setCurPlaylist(nowListName,pathList);
    playController::getInstance().play(nowListName,index);
    g_db->addMusicToHistoryMusic(date.filepath);
    addMusicToHistoryListSlot();
}
void TableHistory::deleteSongs()
{
    int index = m_tableHistory->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    g_db->delMusicFromHistoryMusic(date.filepath);
    addMusicToHistoryListSlot();
}

void TableHistory::playNextRowClicked()
{

}

