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

class TableOne : public QWidget
{
    Q_OBJECT
public:
    explicit TableOne(QString listName,QWidget *parent = nullptr);

    MusicListModel *m_model;
//    QTableView *tableView;
    TableBaseView *tableView;
    QLabel *listTitleLabel;
    QString nowListName;
    void changeNumber();

    QList<musicDataStruct> getMusicList();
    void addMusicToLocalOrPlayList();
private:
    void initUI();
    void initConnect();
    void initRightMenu();
    void tableViewDoubleClicked();

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


//    void mouseMoveEvent(QMouseEvent *event);
    TableViewDelegate *delegate;

    QHBoxLayout *listTitleHBoxLayout;
    QToolButton *addMusicButton;
    QLabel *listTotalNumLabel;
    void setHightLight(int index);
    void initStyle();
    void initTableViewStyle();
signals:
    void sendPathToPlayer(QString fp);

    void countChanges();
//    void hoverIndexChanged(QModelIndex index);
    void addMusicToHistoryListSignal();

public slots:
    void selectListChanged(QString listname);
    void playListRenamed(QString oldName,QString newName);
};

#endif // TableOne_H
