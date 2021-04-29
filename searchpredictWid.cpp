#include"searchpredictWid.h"

searchPredictWid::searchPredictWid(QWidget* parent,QList<QString>& singerList,QList<QString>& albumList,QList<musicDataStruct>& songList) :QWidget(parent)
{
    //this->setObjectName ("searchPredictWid");
    this->parent = parent;
    this->singerList = singerList;
    this->albumList = albumList;
    this->songList = songList;
    this->singerNum = singerList.count ();
    this->albumNum = albumList.count ();
    this->songNum = songList.count ();
    initTools(parent);
    initStyleSheet ();
    initConnect();
    initListen();
}

void searchPredictWid::initTools (QWidget* parent)
{
    predictWid = new QWidget(parent);
    mainLayout = new QVBoxLayout(predictWid);
    predictSingerLab = new QLabel("歌手");
    /*
    predictSingerLab->setText (
                QObject::tr("<font color = red>%1</font>").arg("abc")+
            QObject::tr("<font color = blue>%1</font>").arg("efg")+
            "hij");
    */

    predictAlbumLab = new QLabel("专辑");
    predictSongLab = new QLabel("歌曲" );
    predictSingerLw = new QListWidget();
    predictSingerLw->setObjectName ("predictSingerLw_lx");
    predictAlbumLw = new QListWidget( );
    predictAlbumLw->setObjectName("predictAlbumLw_lx");
    predictSongLw = new QListWidget( );
    predictSongLw->setObjectName("predictSongLw_lx");
    //mainLayout->addStretch (10);
//    mainLayout->setAlignment(Qt::AlignTop);
//    mainLayout->addWidget (predictSingerLab);
//    mainLayout->addWidget (predictSingerLw);
//    mainLayout->addWidget (predictAlbumLab);
//    mainLayout->addWidget (predictAlbumLw);
//    mainLayout->addWidget (predictSongLab);
//    mainLayout->addWidget (predictSongLw);
    //mainLayout->addStretch();

}

void searchPredictWid::setLocation (int x, int y)
{
    this->predictWid->setGeometry (x,y,450,200);
}

void searchPredictWid::initStyleSheet ()
{
    this->predictWid->setGeometry (270,42,450,200);

    int singerLwHeight = singerNum*itemHeight;
    int albumLwHeight = albumNum*itemHeight;
    int songLwHeight = songNum*itemHeight;
    QString str_itemHeight = QString("QListWidget::item{height:%1px;}").arg (QString::number (itemHeight));
    //QString str_labelHeight = QString("QLabel{height:%1px;}").arg (QString::number (labelHeight));
    int searchPredictWidHeight = 3*labelHeight+singerLwHeight+albumLwHeight+songLwHeight;
    //qDebug()<<singerLwHeight<<"    "<<albumLwHeight<<"  "<<songLwHeight;
    //qDebug()<<"the searchPredictHeight is:"<<searchPredictWidHeight;
    this->predictWid->setObjectName ("predictWid");
    mainLayout->setSpacing (0);
    //测试看效果
    /*
    this->predictWid->setStyleSheet("QListWidget{background-color:yellow;border:0px;}"
                                       "QListWidget::item:selected{background-color:#F0F0F0;}"
                                       "QListWidget::item:hover{background-color:blue;}"
                                              "QWidget#predictWid{background:#FFFACD;}"
                                                "QLabel{background:red;}"+str_itemHeight
                                              );
    //已经完成
    */
    this->predictWid->setStyleSheet("QListWidget{background-color:#F9F9F9;border:0px;}"
                                       "QListWidget::item:selected{background-color:#FFFACD;}"
                                       "QListWidget::item:hover{background-color:#40A9FB;border-radius:8px;}"
                                              "QWidget#predictWid{background:#F9F9F9;border-radius:8px;}"
                                                +str_itemHeight
                                              );

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
        shadow_effect->setOffset(0, 0);

        //阴影颜色
        shadow_effect->setColor(QColor(38, 78, 119, 127));

        //阴影半径
        shadow_effect->setBlurRadius(22);
    this->predictWid->setGraphicsEffect (shadow_effect);
    //predictWid->setFixedHeight (searchPredictWidHeight+100);
    //predictWid->setFixedHeight (600);
    //predictWid->setMaximumHeight (600);
    predictSingerLab->setFixedHeight (labelHeight);
    predictAlbumLab->setFixedHeight (labelHeight);
    predictSongLab->setFixedHeight (labelHeight);
    predictSingerLw->setFixedHeight (singerLwHeight);
    predictAlbumLw->setFixedHeight (albumLwHeight);
    predictSongLw->setFixedHeight (songLwHeight);
    //设置字体
    QFont font;
    font.setPointSize (12);
    predictSingerLab->setFont (font);
    predictAlbumLab->setFont (font);
    predictSongLab->setFont (font);

    QListIterator<musicDataStruct> i(songList);
    while(i.hasNext())
    {
        musicDataStruct music = i.next ();
        QListWidgetItem* item1 = new QListWidgetItem(predictSongLw);
        //myTitleBar->predictSingerLiswid->set
        QWidget* wid1 = new QWidget;
        QHBoxLayout* hlayout = new QHBoxLayout(wid1);
        QLabel* label1 = new QLabel;
        label1->setText (music.title);
        hlayout->addWidget (label1,277,Qt::AlignLeft);
        wid1->setFixedHeight (itemWidHeight);
        //wid1->setFixedHeight (10);
//            if(time%2==0){
//                wid1->setStyleSheet("background:#FF1493;");
//            }
//            else{
//                wid1->setStyleSheet("background:blue;");
//            }


        predictSongLw->setItemWidget (item1,wid1);

    }
    QListIterator<QString> i1(singerList);
    while(i1.hasNext())
    {
        QString music = i1.next ();
        QListWidgetItem* item1 = new QListWidgetItem(predictSingerLw);
        QWidget* wid1 = new QWidget;
        QHBoxLayout* hlayout = new QHBoxLayout(wid1);
        QLabel* label1 = new QLabel(wid1);
        label1->setText (music);
        hlayout->addWidget (label1,277,Qt::AlignLeft);
        wid1->setFixedHeight (itemWidHeight);
        predictSingerLw->setItemWidget (item1,wid1);

    }
    QListIterator<QString> i2(albumList);
    while(i2.hasNext())
    {
        QString music = i2.next ();
        QListWidgetItem* item1 = new QListWidgetItem(predictAlbumLw);
        QWidget* wid1 = new QWidget;
        QHBoxLayout* hlayout = new QHBoxLayout(wid1);
        QLabel* label1 = new QLabel(wid1);
        hlayout->addWidget (label1,277,Qt::AlignLeft);
        label1->setText (music);
        wid1->setFixedHeight (itemWidHeight);
        predictAlbumLw->setItemWidget (item1,wid1);

    }

    int height = labelHeight;
    mainLayout->setAlignment(Qt::AlignTop);
    if(singerLwHeight>0){
        //qDebug()<<"singerLwHeight is: "<<singerLwHeight;
        mainLayout->addWidget (predictSingerLab );
        mainLayout->addWidget (predictSingerLw);
        height += labelHeight+singerLwHeight;
        QLabel* fixLine = new QLabel;
        fixLine->setFixedHeight (3);
        fixLine->setStyleSheet ("background:black;");
        mainLayout->addSpacing (2);
        mainLayout->addWidget (fixLine);
    }
    if(albumLwHeight>0){

        mainLayout->addWidget (predictAlbumLab );
        mainLayout->addWidget (predictAlbumLw);
        height += labelHeight+albumLwHeight;
        QLabel* fixLine = new QLabel;
        fixLine->setFixedHeight (3);
        fixLine->setStyleSheet ("background:black;");
        mainLayout->addSpacing (2);
        mainLayout->addWidget (fixLine);
    }
    if(songLwHeight>0){

        mainLayout->addWidget (predictSongLab);
        mainLayout->addWidget (predictSongLw);
        height += labelHeight+songLwHeight;
    }

    predictWid->setFixedHeight (height);
}

void searchPredictWid::initListen()
{
    //qDebug()<<"the labelHeight is:"<<labelHeight;
    //qDebug()<<"the itemHeight is:"<<itemHeight;
    //qDebug()<<"the singerNum is:"<<singerNum;
    //qDebug()<<"the albumNum is:"<<albumNum;
    //qDebug()<<"the songNum is:"<<songNum;
}

void searchPredictWid::initConnect (){

    connect(predictSingerLw,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(do_itemClicked(QListWidgetItem*)));
    connect (predictAlbumLw,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(do_itemClicked(QListWidgetItem*)));
    connect (predictSongLw,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(do_itemClicked(QListWidgetItem*)));
}

void searchPredictWid::do_itemClicked (QListWidgetItem *item){
    //qDebug()<<"call do_itemClicked function";
    //qDebug()<<"the objectName is: "<<item->listWidget ()->objectName ();
    if(item->listWidget ()->objectName () == "predictSingerLw_lx"){
        QList<QLabel*>labels =  predictSingerLw->itemWidget (item)->findChildren<QLabel*>();
        foreach(QLabel* label,labels){
            QString field = label->text ();
            //qDebug()<<"the text in label is:"<<field;
            //emit 0 就是singer，1是album，2song，
            int choose = 0;
            musicDataStruct music;
            emit searchForField (field,choose,music);
        }
    }
    else if(item->listWidget ()->objectName () == "predictAlbumLw_lx"){
        QList<QLabel*>labels =  predictAlbumLw->itemWidget (item)->findChildren<QLabel*>();
        foreach(QLabel* label,labels){
            QString field = label->text ();
            //qDebug()<<"the text in label is:"<<field;
            //emit 0 就是singer，1是album，2song，
            int choose = 1;
            musicDataStruct music;
            emit searchForField (field,choose,music);
        }
    }
    else if(item->listWidget ()->objectName () == "predictSongLw_lx"){
        QList<QLabel*>labels =  predictSongLw->itemWidget (item)->findChildren<QLabel*>();
        foreach(QLabel* label,labels){
            QString field = label->text ();
            //qDebug()<<"the text in label is:"<<field;
            //emit 0 就是singer，1是album，2song，
            int choose = 2;
            musicDataStruct music = this->songList.at (predictSongLw->currentRow ());
            emit searchForField (field,choose,music);
        }
    }

    /*
    QList<QLabel*>labels =  predictSingerLw->itemWidget (item)->findChildren<QLabel*>();
    foreach(QLabel* label,labels){
        QString field = label->text ();
        //qDebug()<<"the text in label is:"<<field;
        //emit 0 就是singer，1是album，2song，
        int choose = 0;
        emit searchForField (field,choose);
    }
    */
}

searchPredictWid::~searchPredictWid (){
    delete predictSingerLab;
    delete predictAlbumLab;
    delete predictSongLab;
    delete predictSongLw;
    delete predictAlbumLw;
    delete predictSingerLw;
    delete mainLayout;
    delete predictWid;
}




