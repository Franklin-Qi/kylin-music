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

#include "customTreeView.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UIControl/base/musicDataBase.h"
#include "UIControl/player/player.h"
#include "UI/base/widgetstyle.h"

class TableHistory : public QDialog
{
    Q_OBJECT

public:
    explicit TableHistory(QWidget *parent = nullptr);

    MusicListModel *m_model = nullptr;
    CustomTreeView *m_view = nullptr;

    bool isHightLight = false;

    void initRightMenu();
    void initSetModel();
    void initConnect();
    void initStyle();
    void initTableStyle();

    void changeNumber();
    void setHighlight(int index);
    void refreshHistoryTable();
    void noRefreshHistory();
    void deleteAllClicked();
    void changePlayHistoryPos(int posX, int posY, int width, int height);

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void showHistroryPlayList();
    void slotPlayIndexChanged(int index, QString listname);
    void slotPlayPathChanged(QString songPath);
    void slotLableSetFontSize(int size);
    void playSongsDoubleClicked(QModelIndex index);
    void playSongs();
    void deleteSongs();
    void playNextRowClicked();
    void showRightMenu(const QPoint &pos);


Q_SIGNALS:
    void signalHistoryPlaying();
    void signalHistoryBtnChecked(bool checked);

private:
    QVBoxLayout *mainVLayout = nullptr;
    QHBoxLayout *titleHBoxLayout = nullptr;
    QLabel *listCountLabel = nullptr;
    QLabel *historyTitileLabel = nullptr;
    QToolButton *deleteAllBtn = nullptr;
    QWidget *historyTitileWidget = nullptr;

    QAction *playRow = nullptr;
    QAction *removeRow = nullptr;
    QAction *playNextRow = nullptr;
    QMenu *m_menu = nullptr;
    QString nowListName = HISTORY;
    QString nowPlayListName = "";
    int nowPlayIndex = -1;


    QWidget *nullPageWidget = nullptr;
    QVBoxLayout *nullPageLayout = nullptr;
    QLabel *nullIconLabel = nullptr;
    QLabel *nullTextLabel = nullptr;

    QHeaderView *horizonHeader = nullptr;
    QWidget *tableWidget = nullptr; // 歌曲列表Widget,用来包含tableHBoxLayout
    QHBoxLayout *tableHBoxLayout = nullptr; // 用来包含m_tableHistory

    int playHistoryPosX;
    int playHistoryPosY;
    int playHistoryPosWidth;
    int playHistoryPosHeight;
};

#endif // TABLEHISTORY_H
