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
#include <QFileSystemWatcher>
#include <QStandardPaths>

#include "UIControl/base/musicDataBase.h"
#include "UIControl/tableview/musiclistmodel.h"
#include "UIControl/player/player.h"
#include "tableviewdelegate.h"
#include "tablebaseview.h"
#include "UIControl/base/musicfileinformation.h"
#include "UI/base/mylabel.h"
#include "UI/base/widgetstyle.h"
#include "UI/base/allpupwindow.h"

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

    void playMusicforIndex(QString listName,int index);
    //通过列表名和索引值播放相应歌曲
    void showTitleText(QString listName);
private:
    void initUI();  //初始化ui
    void initConnect();  //信号绑定
    void initRightMenu();  //初始化右键菜单
    void tableViewDoubleClicked();  //双击播放

    void showRightMenu(const QPoint &pos);
    void deleteSongs();
    void playSongs();
    void showInfo();
    MusicInfoDialog *infoDialog;
    void addToOtherList(QAction *listNameAction);

    void addMusicSlot(); // 添加歌曲文件槽函数
    void addDirMusicSlot();  //添加文件夹槽函数
    void addMusicToDatebase(QStringList fileNames);

    void importSongs(QString path);
    QMenu *m_menu;  //新建一个Menu属性
    QSqlQueryModel *tableModel;

    QAction * playRow;
    QAction * removeRow;
    QAction * showInfoRow;
    QMenu * addToOtherListMenu;
    QMap<int,QString> getSelectedTaskIdList();

    QMenu *add_menu;
    QAction *addMusicFileAction;
    QAction *addDirMusicAction;

    TableViewDelegate *delegate;

    QHBoxLayout *listTitleHBoxLayout;
    QToolButton *addMusicButton;
    QLabel *listTotalNumLabel;
    void initStyle();
    int heightLightIndex = -1;
    QString nowPlayListName;  //后端播放器中正在播放的列表名
signals:
    void sendPathToPlayer(QString fp);
    void countChanges();
    void hoverIndexChanged(QModelIndex index);
    void addMusicToHistoryListSignal();
//    void heightIndexChanged(int index);
    void addILoveFilepathSignal(QString filePath);  //传递我喜欢歌单中添加歌曲的信号
    void removeILoveFilepathSignal(QString filePath);  //传递我喜欢歌单中删除歌曲的信号

public slots:
    void selectListChanged(QString listname);  //切换歌单
    void playListRenamed(QString oldName,QString newName);  //歌单重命名
    void getHightLightIndex(int index, QString listName); //获得正在播放的歌曲索引和歌单名

protected:
    void dragEnterEvent(QDragEnterEvent *event)Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event)Q_DECL_OVERRIDE;
private:
    QWidget *m_musicWidget;
    QVBoxLayout *m_historyLayout;
    QWidget *nullPageWidget;
    QVBoxLayout *nullPageVLayout;
    QHBoxLayout *nullPageHLayout;
    QPushButton *n_addMusicButton;
    QPushButton *n_addDirMusicButton;
    QLabel *nullPageIconLabel;
    QLabel *nullPageTextLabel;
};

#endif // TableOne_H
