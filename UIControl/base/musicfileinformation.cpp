extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include "musicfileinformation.h"

MusicFileInformation::MusicFileInformation(QObject *parent) : QObject(parent)
{
    musicType << "mp3" << "ogg" << "wma" << "flac" << "wav" << "ape" << "amr" << "m4a" << "ac3" << "aac" << "mid";
}
void MusicFileInformation::addFile(const QStringList &addFile)
{
    int ret;
    resList.clear();
    if(!addFile.isEmpty())
    {
        for(int i = 0; i < addFile.size(); i++)
        {
            musicdataStruct.filepath = addFile.at(i);
            fileInfo.setFile(musicdataStruct.filepath);
            fileType(fileInfo);          //文件类型
            fileSize(fileInfo);      //文件大小
            if(musicType.indexOf(musicdataStruct.filetype) != -1)
            {
                fileInformation(musicdataStruct.filepath);//获取歌曲文件信息
                resList.append(musicdataStruct);
            }
            else
            {
                qDebug() << "添加文件失败";
            }
        }
    }
}

QString MusicFileInformation::filterTextCode(QString str)
{
    QByteArray strdata =str.toLocal8Bit();
    int len = strdata.length();
    int num = 0;
    int i = 0;
    while (i < len) {
        if ((strdata[i] & 0x80) == 0x00) {
            // 0XXX_XXXX
            i++;
            continue;
        }
        else if ((num = preNum(strdata[i])) > 2) {
            // 110X_XXXX 10XX_XXXX
            // 1110_XXXX 10XX_XXXX 10XX_XXXX
            // 1111_0XXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
            // 1111_10XX 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
            // 1111_110X 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
            // preNUm() 返回首个字节8个bits中首�?0bit前面1bit的个数，该数量也是该字符所使用的字节数
            i++;
            for(int j = 0; j < num - 1; j++) {
                //判断后面num - 1 个字节是不是都是10开
                if ((strdata[i] & 0xc0) != 0x80) {
                    return "";
                }
                i++;
            }
        } else {
            //其他情况说明不是utf-8
            return "";
        }
    }
    return str;
}

inline int MusicFileInformation::preNum(unsigned char byte) {
    unsigned char mask = 0x80;
    int num = 0;
    for (int i = 0; i < 8; i++) {
        if ((byte & mask) == mask) {
            mask = mask >> 1;
            num++;
        } else {
            break;
        }
    }
    return num;
}

QStringList MusicFileInformation::fileInformation(QString filepath)
{
    QStringList information;
    QByteArray byteArray = filepath.toLocal8Bit();
    TagLib::FileRef f(byteArray.data());
    if(f.isNull())
    {
        QString name = fileInfo.completeBaseName();
        QString singer = "未知歌手";
        QString album = "未知专辑";
        musicdataStruct.singer = singer;
        musicdataStruct.album = album;
        musicdataStruct.title  = name;

        AVFormatContext *pFormatCtx = NULL;
        avformat_open_input(&pFormatCtx, filepath.toStdString().c_str(), nullptr, nullptr);
        if(pFormatCtx)
        {
            avformat_find_stream_info(pFormatCtx, nullptr);
            qint64 duration = pFormatCtx->duration / 1000;
            if(duration > 0)
            {
                QTime me(0,(duration/60000) % 60,(duration / 1000) % 60);
                QString length = me.toString("mm:ss");
                musicdataStruct.time = length;
            }
        }
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        information << musicdataStruct.title << musicdataStruct.singer << musicdataStruct.album << musicdataStruct.time;
        return information;
    }
    TagLib::PropertyMap propertyMap = f.file() ->properties();

    QString musicName = propertyMap["TITLE"].toString().toCString(true);
    if(filterTextCode(musicName).isEmpty())
        musicName = fileInfo.completeBaseName();
    QString musicSinger = propertyMap["ARTIST"].toString().toCString(true);
    if(filterTextCode(musicSinger).isEmpty())
        musicSinger = "未知歌手";
    QString musicAlbum = propertyMap["ALBUM"].toString().toCString(true);
    if(filterTextCode(musicAlbum).isEmpty())
        musicAlbum = "未知专辑";
    TagLib::AudioProperties *properties = f.audioProperties();

    int seconds = properties->length() % 60;
    int minutes = (properties->length() - seconds) / 60;
    QTime time(0,minutes,seconds);
//    QString musicTime = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
    QString musicTime = time.toString("mm:ss");
    musicdataStruct.title = musicName;
    musicdataStruct.singer = musicSinger;
    musicdataStruct.album = musicAlbum;
    musicdataStruct.time = musicTime;
    qDebug() << "527 音频音频音频音频音频" << musicdataStruct.time;
    QStringList audioFileInformation;
    audioFileInformation << musicdataStruct.title << musicdataStruct.singer
                         << musicdataStruct.album << musicdataStruct.time;
    return audioFileInformation;
}

QString MusicFileInformation::fileSize(QFileInfo fileInfo)
{
    QString musicSize;
    qint64 size = fileInfo.size();   //文件大小
    if(size/1024)
    {
        if(size/1024/1024)
        {
            if(size/1024/1024/1024)
            {
                musicSize = QString::number(size/1024/1024/1024,10)+"GB";
            }
            else
                musicSize = QString::number(size/1024/1024,10)+"MB";
        }
        else
            musicSize = QString::number(size/1024,10)+"KB";
    }
    else
        musicSize = QString::number(size,10)+"B";
    musicdataStruct.size = musicSize;
    return musicdataStruct.size;
}

QString MusicFileInformation::fileType(QFileInfo fileInfo)
{
    QString musicType = fileInfo.suffix();        //文件类型
    musicdataStruct.filetype = musicType;
    return musicdataStruct.filetype;
}
