#ifndef TableOne_H
#define TableOne_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
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
#include <QScrollArea>
#include <QScrollBar>
#include <QProcess>

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
    ~TableOne();
public:
    MusicListModel *m_model;
//    QTableView *tableView;
    TableBaseView *tableView;
    QLabel *listTitleLabel;
    QString nowListName;
    void changeNumber();
    void initTableViewStyle();
    void setHightLightAndSelect();

    void getMusicList();

    void playMusicforIndex(QString listName,int index);
    //通过列表名和索引值播放相应歌曲
    void showTitleText(QString listName);

    void initStyle();
    void addMusicToDatebase(QStringList fileNames);
public slots:
    void slotSearchTexts(QString text);
private:
    void initUI();  //初始化ui
    void initConnect();  //信号绑定
    void initRightMenu();  //初始化右键菜单
    void tableViewDoubleClicked();  //双击播放

    static void deleteImage(const QString &savepath);
    static void _processStart(const QString &cmd , QStringList arguments = QStringList());
    void processLog();

    //成功添加多少首歌曲
    void importFinished(int successCount, int failCount, int allCount);

    void showRightMenu(const QPoint &pos);

    void isDeleteSongs(); //是否从歌单中删除歌曲
    void isDeleteLocalSongs(); //是否从本地删除歌曲
    void deleteSongs();  //从歌单中删除歌曲
    void deleteLocalSongs(); //从本地以及歌单中删除歌曲

    void playSongs();
    void showInfo();
    MusicInfoDialog *infoDialog;
    void addToOtherList(QAction *listNameAction);

    void addMusicSlot(); // 添加歌曲文件槽函数
    void addDirMusicSlot();  //添加文件夹槽函数
//    void addMusicToDatebase(QStringList fileNames);

//    void importSongs(QString path);
    QMenu *m_menu;  //新建一个Menu属性
    QSqlQueryModel *tableModel;

    QAction * playRow;
    QAction * removeRow;
    QAction * removeLocalRow;
    QAction * showInfoRow;
    QMenu * addToOtherListMenu;
    QMap<int,QString> getSelectedTaskIdList();

    QMenu *add_menu;
    QAction *addMusicFileAction;
    QAction *addDirMusicAction;

    TableViewDelegate *delegate;

    QHBoxLayout *listTitleHBoxLayout;
    QToolButton *addMusicButton;  //添加歌曲按钮
    QPushButton *playAllButton;

    QLabel *listTotalNumLabel;
    int heightLightIndex = -1;
    QString nowPlayListName;  //后端播放器中正在播放的列表名
signals:
    void sendPathToPlayer(QString fp);
    void countChanges();
    void hoverIndexChanged(QModelIndex index);
    void refreshHistoryListSignal();
//    void heightIndexChanged(int index);
    void addILoveFilepathSignal(QString filePath);  //传递我喜欢歌单中添加歌曲的信号
    void removeILoveFilepathSignal(QString filePath);  //传递我喜欢歌单中删除歌曲的信号
    void signalListSearch();
public slots:
    void selectListChanged(QString listname);  //切换歌单
    void playListRenamed(QString oldName,QString newName);  //歌单重命名
    void getHightLightIndex(int index, QString listName); //获得正在播放的歌曲索引和歌单名

    void playAll(QString listName);  //播放全部歌曲
    void playAllButtonClicked();
    void slotReturnText(QString text);
protected:
    void dragEnterEvent(QDragEnterEvent *event)Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event)Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
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

    QHeaderView *horizonHeader;
    QWidget *titleWid;

    int showScrollbarNumber = 10;
    QString m_text;
};

#endif // TableOne_H
