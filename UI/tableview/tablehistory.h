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
#include <QHeaderView>

#include "tablebaseview.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UIControl/base/musicDataBase.h"
#include "UIControl/player/player.h"
#include "UI/base/widgetstyle.h"

class TableHistory : public QDialog
{
    Q_OBJECT
public:
    explicit TableHistory(QWidget *parent = nullptr);
    TableBaseView *m_tableHistory;
    MusicListModel *m_model;
    void changeNumber();
    void setHighlight(int index);
    void initStyle();
    void initTableStyle();
    bool isHightLight = false;
    void refreshHistoryTable();
    void deleteAllClicked();
    void changePlayHistoryPos(int posX, int posY, int width, int height);
public slots:
    void showHistroryPlayList();
//    void addMusicToHistoryListSlot();
    void slotPlayIndexChanged(int index, QString listname);
    void slotPlayPathChanged(QString songPath);
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result)override;
signals:
    void signalHistoryPlaying();
    void signalHistoryBtnChecked(bool checked);
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

    QAction *playRow;
    QAction *removeRow;
    QAction *playNextRow;
    QMenu *m_menu;
    QString nowListName = HISTORY;
    QString nowPlayListName = "";
    int nowPlayIndex = -1;

    QWidget *nullPageWidget;
    QVBoxLayout *nullPageLayout;
    QLabel *nullIconLabel;
    QLabel *nullTextLabel;
    QHeaderView *horizonHeader;
private:
    int playHistoryPosX;
    int playHistoryPosY;
    int playHistoryPosWidth;
    int playHistoryPosHeight;
};

#endif // TABLEHISTORY_H
