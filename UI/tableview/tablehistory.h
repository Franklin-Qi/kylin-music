#ifndef TABLEHISTORY_H
#define TABLEHISTORY_H

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
#include <QFrame>
#include <QGraphicsDropShadowEffect>

#include "tablebaseview.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UIControl/base/musicDataBase.h"
#include "UIControl/player/player.h"

class TableHistory : public QFrame
{
    Q_OBJECT
public:
    explicit TableHistory(QWidget *parent = nullptr);
    TableBaseView *m_tableHistory;
    MusicListModel *m_model;
    void changeNumber();
public slots:
    void showHistroryPlayList();
    void addMusicToHistoryListSlot();
signals:

private slots:
    void playSongs();
    void deleteSongs();
    void playNextRowClicked();
    void showRightMenu(const QPoint &pos);
private:
    void initRightMenu();
    QVBoxLayout *mainVLayout;
    QHBoxLayout *titleHBoxLayout;
    void initSetModel();
    void initConnect();
    QLabel *listCountLabel;
    QLabel *historyTitileLabel;
    QToolButton *deleteAllBtn;
    QWidget *historyTitileWidget;
    void initStyle();
    void deleteAllClicked();
    void initTableStyle();

    QAction *playRow;
    QAction *removeRow;
    QAction *playNextRow;
    QMenu *m_menu;
    QString nowListName = "historyList";

};

#endif // TABLEHISTORY_H
