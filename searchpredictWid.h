#ifndef SEARCHPREDICTWID_H
#define SEARCHPREDICTWID_H
#include<QWidget>
#include<QListWidget>
#include<QListWidgetItem>
#include<QLabel>
#include<QVBoxLayout>
#include<QList>
#include<QString>
#include<QDebug>
#include<QObject>


#include"musicDataBase.h"



/*

*/
class searchPredictWid : public QWidget
{
    Q_OBJECT
public:
    searchPredictWid(QWidget* parent,QList<QString>& singerList,QList<QString>& albumList,QList<musicDataStruct>& songList);
    //控件
    QWidget* parent;
    QWidget* predictWid;
    QVBoxLayout* mainLayout;
    QLabel* predictSingerLab;
    QLabel* predictAlbumLab;
    QLabel* predictSongLab;
    QListWidget* predictSingerLw;
    QListWidget* predictAlbumLw;
    QListWidget* predictSongLw;
    int labelHeight = 30;
    int itemHeight = 35;
    int itemWidHeight = 35;
    int singerNum = 0;
    int albumNum = 0;
    int songNum = 0;

    //数据结构
    QList<QString> singerList;
    QList<QString> albumList;
    QList<musicDataStruct> songList;
    void initTools(QWidget* parent);
    void setLocation(int x,int y);
    //void setData(QList<QString> singerList,QList<QString> albumList,QList<musicDataStruct> songList);
    void initStyleSheet();
    void initListen();
    void initConnect();
    ~searchPredictWid();
public slots:
    void do_itemClicked(QListWidgetItem* );
signals:
    void searchForField(QString& field,int& choose,musicDataStruct& music);
};

#endif // SEARCHPREDICTWID_H
