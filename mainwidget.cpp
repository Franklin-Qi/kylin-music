#include "mainwidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    int res;
    res = g_db->initDataBase();
    if(res != DB_OP_SUCC)
    {
        qDebug() << "数据库加载失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
    }
    qDebug() << "数据库加载成功";
    initAllComponent();
    allConnect();
    initGSettings();
}

Widget::~Widget()
{
}



void Widget::initAllComponent()
{

    mainVBoxLayout = new QVBoxLayout();

//    musicListTable = new TableBaseView();
    musicListTable = new TableOne(tr("Song List"),this);
    playSongArea = new PlaySongArea;
    QWidget *rightVWidget = new QWidget(this);
    rightVWidget->setLayout(mainVBoxLayout);
    mainVBoxLayout->addWidget(musicListTable);
    mainVBoxLayout->addWidget(playSongArea,0,Qt::AlignBottom);
    mainHBoxLayout = new QHBoxLayout();
    sideBarWid = new SideBarWidget(this);
    sideBarWid->setFixedWidth(210);
    mainHBoxLayout->addWidget(sideBarWid);
    mainHBoxLayout->addWidget(rightVWidget);
    mainHBoxLayout->setSpacing(0);
    mainHBoxLayout->setMargin(0);

    this->resize(960,640);
    this->setLayout(mainHBoxLayout);
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);
}

void Widget::allConnect()
{

    connect(sideBarWid,&SideBarWidget::playListBtnCliced,musicListTable,&TableOne::selectListChanged);
}


void Widget::showHistroryPlayList()
{
//    historyListTable->show();
}
void Widget::initGSettings()//初始化GSettings
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
        themeData = new QGSettings(FITTHEMEWINDOW);
        if(themeData->get("style-name").toString() == "ukui-dark" || themeData->get("style-name").toString() == "ukui-black"){
            WidgetStyle::themeColor = 1;
//            changeDarkTheme();
        }
        else
        {
            WidgetStyle::themeColor = 0;
//            changeLightTheme();
        }

        connect(themeData,&QGSettings::changed,this,[=]()
        {
            qDebug() << "主题颜色" << themeData->get("style-name").toString();
            if(themeData->get("style-name").toString() == "ukui-dark" || themeData->get("style-name").toString() == "ukui-black"){
                WidgetStyle::themeColor = 1;
//                changeDarkTheme();
            }
            else
            {
                WidgetStyle::themeColor = 0;
//                changeLight Theme();
            }
        });
    }
    qDebug()<<"初始化GSettings成功";
}

