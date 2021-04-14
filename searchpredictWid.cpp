#include"searchpredictWid.h"

searchPredictWid::searchPredictWid(QWidget* parent,QList<QString>& singerList,QList<QString>& albumList,QList<musicDataStruct>& songList)
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
    initListen();
}

void searchPredictWid::initTools (QWidget* parent)
{
    predictWid = new QWidget(parent);
    mainLayout = new QVBoxLayout(predictWid);
    predictSingerLab = new QLabel("Singer");
    predictAlbumLab = new QLabel("Album");
    predictSongLab = new QLabel("Song" );
    predictSingerLw = new QListWidget();
    predictAlbumLw = new QListWidget( );
    predictSongLw = new QListWidget( );
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
    qDebug()<<singerLwHeight<<"    "<<albumLwHeight<<"  "<<songLwHeight;
    qDebug()<<"the searchPredictHeight is:"<<searchPredictWidHeight;
    this->predictWid->setObjectName ("predictWid");
    /*
    //测试看效果
    this->predictWid->setStyleSheet("QListWidget{background-color:yellow;border:0px;}"
                                       "QListWidget::item:selected{background-color:#F0F0F0;}"
                                       "QListWidget::item:hover{background-color:blue;}"
                                              "QWidget#predictWid{background:#FFFACD;}"
                                                "QLabel{background:red;}"+str_itemHeight
                                              );
    */
    this->predictWid->setStyleSheet("QListWidget{background-color:#FFFACD;border:0px;}"
                                       "QListWidget::item:selected{background-color:#FFFACD;}"
                                       "QListWidget::item:hover{background-color:blue;}"
                                              "QWidget#predictWid{background:#FFFACD;}"
                                                +str_itemHeight
                                              );

    //predictWid->setFixedHeight (searchPredictWidHeight+100);
    //predictWid->setFixedHeight (600);
    //predictWid->setMaximumHeight (600);
    predictSingerLab->setFixedHeight (labelHeight);
    predictAlbumLab->setFixedHeight (labelHeight);
    predictSongLab->setFixedHeight (labelHeight);
    predictSingerLw->setFixedHeight (singerLwHeight);
    predictAlbumLw->setFixedHeight (albumLwHeight);
    predictSongLw->setFixedHeight (songLwHeight);

    QListIterator<musicDataStruct> i(songList);
    while(i.hasNext())
    {
        musicDataStruct music = i.next ();
        QListWidgetItem* item1 = new QListWidgetItem(predictSongLw);
        //myTitleBar->predictSingerLiswid->set
        QWidget* wid1 = new QWidget;
        QHBoxLayout* hlayout = new QHBoxLayout(wid1);
        QLabel* label1 = new QLabel(wid1);
        label1->setText (music.title);
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
        wid1->setFixedHeight (itemWidHeight);
        predictSingerLw->setItemWidget (item1,wid1);

    }
    QListIterator<QString> i2(singerList);
    while(i2.hasNext())
    {
        QString music = i2.next ();
        QListWidgetItem* item1 = new QListWidgetItem(predictAlbumLw);
        QWidget* wid1 = new QWidget;
        QHBoxLayout* hlayout = new QHBoxLayout(wid1);
        QLabel* label1 = new QLabel(wid1);
        label1->setText (music);
        wid1->setFixedHeight (itemWidHeight);
        predictAlbumLw->setItemWidget (item1,wid1);

    }

    int height = labelHeight;
    mainLayout->setAlignment(Qt::AlignTop);
    if(singerLwHeight>0){
        qDebug()<<"singerLwHeight is: "<<singerLwHeight;
        mainLayout->addWidget (predictSingerLab);
        mainLayout->addWidget (predictSingerLw);
        height += labelHeight+singerLwHeight;
    }
    if(albumLwHeight>0){
        mainLayout->addWidget (predictAlbumLab);
        mainLayout->addWidget (predictAlbumLw);
        height += labelHeight+albumLwHeight;
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
    qDebug()<<"the labelHeight is:"<<labelHeight;
    qDebug()<<"the itemHeight is:"<<itemHeight;
    qDebug()<<"the singerNum is:"<<singerNum;
    qDebug()<<"the albumNum is:"<<albumNum;
    qDebug()<<"the songNum is:"<<songNum;
}
