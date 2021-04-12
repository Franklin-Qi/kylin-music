#include "musicfileinformation.h"

MusicFileInformation::MusicFileInformation(QObject *parent) : QObject(parent)
{
    musicType << "mp3" << "ogg" << "wma" << "spx" << "flac";
}
void MusicFileInformation::addFile(const QStringList &addFile)
{
    int ret;
    if(!addFile.isEmpty())
    {
        resList.clear();
        for(int i = 0; i < addFile.size(); i++)
        {
            musicdataStruct.filepath = addFile.at(i);
            fileInfo.setFile(musicdataStruct.filepath);
            fileType(fileInfo);          //文件类型
            fileSize(fileInfo);      //文件大小
            if(musicType.indexOf(musicdataStruct.filetype) != -1)
            {
                fileInformation(musicdataStruct.filepath);//获取歌曲文件信息
                //            filepathHash(musicdataStruct.filepath);// 通过路径获取hash
                ret = g_db->addMusicToLocalMusic(musicdataStruct);
                if (ret == DB_OP_SUCC) {
                    qDebug() << "歌曲成功添加！";
                    resList.append(musicdataStruct);
                } else {
                    qDebug()<<"歌曲已存在";
                }
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

    QByteArray byteArray = filepath.toLocal8Bit();
    TagLib::FileRef f(byteArray.data());
//    if(f.isNull())
//    {
//        //can't read this music;
//        continue;
//    }
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
    if(properties == nullptr)
    {
        qDebug() << "添加文件失败";
        return songFiles;
    }

    int seconds = properties->length() % 60;
    int minutes = (properties->length() - seconds) / 60;
    QString musicTime = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
    musicdataStruct.title = musicName;
    musicdataStruct.singer = musicSinger;
    musicdataStruct.album = musicAlbum;
    musicdataStruct.time = musicTime;
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
