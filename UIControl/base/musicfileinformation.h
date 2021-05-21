#ifndef MUSICFILEINFORMATION_H
#define MUSICFILEINFORMATION_H

#include <QDir>
#include <QFileInfo>
#include <QByteArray>
#include <QString>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QEventLoop>
#include <QMediaContent>
#include <QMediaPlayer>

//taglib
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/tag.h>
#include <taglib/taglib.h>
//#include <taglib/tpropertymap.h>
////#include <taglib/toolkit/tpropertymap.h>
#include "musicDataBase.h"


class MusicFileInformation : public QObject
{
    Q_OBJECT
public:
    explicit MusicFileInformation(QObject *parent = nullptr);
    static MusicFileInformation& getInstance()
    {
        static MusicFileInformation instance;
        return instance;
    }
    QList<musicDataStruct> resList;
    void addFile(const QStringList &addFile);
    int findIndexFromPlayList(QString listname, QString filepath);
public slots:
    void durationChange(qint64 duration);
signals:
    void durations();
private:
    QStringList updateSongInfoFromLocal(QString filepath);
private:
//    QList<musicDataStruct> resList;
    musicDataStruct musicdataStruct;
    QStringList AllDirList;
    QStringList songFiles;
    QString Dir;
    QStringList matchMp3Files;

    QFileInfo fileInfo;
    QString MD5Str;
    QStringList MD5List;
    QDir matchDir;

    QString fileType(QFileInfo fileInfo);
    QString fileSize(QFileInfo fileInfo);
    QStringList fileInformation(QString filepath);
    QString filterTextCode(QString str);
    int preNum(unsigned char byte);

    QStringList musicType;
    qint64 dur;
    bool success = false;
};

#endif // MUSICFILEINFORMATION_H
