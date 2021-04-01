#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>

#include "UIControl/base/musicDataBase.h"
#include "UI/tableview/tableone.h"
#include "UIControl/tableview/scoreinfomodel.h"
#include "UI/tableview/tablebaseview.h"
#include "UI/base/widgetstyle.h"
#include "UI/sidebar/sidebarwidget.h"
#include "UI/player/playsongarea.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:

private slots:
private:

    QVBoxLayout *mainVBoxLayout;
    QHBoxLayout *mainHBoxLayout;

//    TableBaseView *musicListTable;
    TableOne *musicListTable;
//    TableOne *historyListTable;

    void initAllComponent();
    void allConnect();
    void showHistroryPlayList();

    void initGSettings();
    QGSettings *themeData = nullptr;
    SideBarWidget *sideBarWid;
    PlaySongArea *playSongArea;
};
#endif // WIDGET_H
