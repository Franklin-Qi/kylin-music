#ifndef TableOne_H
#define TableOne_H

#include <QWidget>
#include <QListView>
#include <QLabel>
#include <QToolButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QVBoxLayout>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>

#include "UIControl/base/musicDataBase.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UIControl/player/player.h"
#include "tableviewdelegate.h"
#include "tablebaseview.h"
#include "UIControl/base/musicfileinformation.h"
#include "UI/base/mylabel.h"
#include "UI/base/widgetstyle.h"

class TableOne : public QWidget
{
    Q_OBJECT
public:
    explicit TableOne(QString listName,QWidget *parent = nullptr);

    MusicListModel *m_model;
//    QTableView *tableView;
    TableBaseView *tableView;
    MyLabel *listTitleLabel;
    QString nowListName;
    void changeNumber();
    void initTableViewStyle();
    void setHightLightAndSelect();

    QList<musicDataStruct> getMusicList();
    void addMusicToLocalOrPlayList();
private:
    void initUI();  //初始化ui
    void initConnect();  //信号绑定
    void initRightMenu();  //初始化右键菜单
    void tableViewDoubleClicked();  //双击播放

    void showRightMenu(const QPoint &pos);
    void deleteSongs();
    void playSongs();
    void showInfo();
    void addToOtherList(QAction *listNameAction);
    QMenu *m_menu;  //新建一个Menu属性
    QSqlQueryModel *tableModel;

    QAction * playRow;
    QAction * removeRow;
    QAction * showInfoRow;
    QMenu * addToOtherListMenu;
    QMap<int, QString> getSelectedTaskIdList();


    void mouseMoveEvent(QMouseEvent *event)override;
    TableViewDelegate *delegate;

    QHBoxLayout *listTitleHBoxLayout;
    QToolButton *addMusicButton;
    QLabel *listTotalNumLabel;
    void initStyle();
    int heightLightIndex = -1;
    QString nowPlayListName;
signals:
    void sendPathToPlayer(QString fp);

    void countChanges();
    void hoverIndexChanged(QModelIndex index);
    void addMusicToHistoryListSignal();
//    void heightIndexChanged(int index);

public slots:
    void selectListChanged(QString listname);  //切换歌单
    void playListRenamed(QString oldName,QString newName);  //歌单重命名
    void getHightLightIndex(int index, QString listName); //获得正在播放的歌曲索引和歌单名
};

#endif // TableOne_H
