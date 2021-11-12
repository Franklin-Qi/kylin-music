#include "UI/mainwidget.h"
#include "UI/base/xatom-helper.h"
#include <QApplication>
#include <QDebug>
#include <ukui-log4qt.h>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString strMsg("");
    switch (type)
    {
    case QtDebugMsg:
    default:
        strMsg = QString("Debug:");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:");
        break;

    }

    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString strMessage = QString(QString() +
                                 "Message:%1"
                                 // +"        File:%2\n"
                                 // +"        Line:%3\n"
                                 // +"        Function:%4\n"
                                 // +"        DateTime:%5"
                                 )
                             .arg(localMsg.constData());
    // .arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);

    // 输出信息至文件中（读写、追加形式）
    QString dirPath = "/tmp/kylin-music/log";
    QDir dir;
    QFile file;
    if (dir.mkpath(dirPath)) {
        dirPath = dirPath + "/" + strDateTime + ".txt" ;
        file.setFileName(dirPath);
    }

    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    //加在最上面的原因：防止QApplication将(-title)参数吞掉，导致拿不到
    QStringList strList;
    QString str1;
    for(int i = 0; i < argc;i++)
    {
        str1 = argv[i];
        strList << str1;
    }

    initUkuiLog4qt("kylin-music");
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//    //高清屏幕自适应
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    // 适配4K屏
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    #endif

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    #endif
//    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//    //高清屏幕自适应
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon::fromTheme("kylin-music"));

    QTranslator app_trans;
    QTranslator qt_trans;
    QString locale = QLocale::system().name();

    QString trans_path;
    if (QDir("/usr/share/kylin-music/translations").exists()) {
        trans_path = "/usr/share/kylin-music/translations";
    }
    else {
        trans_path = qApp->applicationDirPath() + "/translations";
    }
    QString qt_trans_path;
    qt_trans_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);// /usr/share/qt5/translations

    if(!app_trans.load("kylin-music_" + locale + ".qm", trans_path))
        qDebug() << "Load translation file："<< "kylin-music_" + locale + ".qm from" << trans_path << "failed!";
    else
        a.installTranslator(&app_trans);

    if(!qt_trans.load("qt_" + locale + ".qm", qt_trans_path))
        qDebug() << "Load translation file："<< "qt_" + locale + ".qm from" << qt_trans_path << "failed!";
    else
        a.installTranslator(&qt_trans);

//    QString locale = QLocale::system().name();
//    QTranslator trans_global, trans_menu;
//    if(locale == "zh_CN"){
//        trans_global.load(":/translations/kylin-music_side.qm");
//        trans_menu.load(":/translations/qt_zh_CN.qm");
//        a.installTranslator(&trans_global);
//        a.installTranslator(&trans_menu);
//    }

//    qApp->setProperty("noChangeSystemFontSize", true);

    QTranslator app_trans_peony;
    app_trans_peony.load("/usr/share/libpeony-qt/libpeony-qt_"+QLocale::system().name());
    a.installTranslator(&app_trans_peony);
//#ifndef QT_NO_TRANSLATION
//    QString translatorFileName = QLatin1String("qt_");
//    translatorFileName += QLocale::system().name();
//    QTranslator *translator = new QTranslator();
//    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
//        a.installTranslator(translator);
//    else
//        qDebug() << "Failed to load Chinese translation file.";
//#endif

    QString str = "";
    QString str2 = "";
    QString str3 = "";
    if(strList.size() > 1)
    {
        str = argv[1];
        if(strList.size() == 4)
        {
            str2 = argv[2];
            str3 = argv[3];
        }
    }
    if (str != "")
    {
        if(str == "--help"||str == "--h")//帮助
        {
            qDebug()<<"\nkylin-music [cmd]\n"
                      "-b -back  上一首\n"
                      "-n -next  下一首\n"
                      "-p -pause  暂停\n"
                      "-s -start  播放\n"
                      "-i -increase  调高音量\n"
                      "-r -reduce    调低音量\n"
                      "-sta -state            当前播放状态\n"
                      "-t -title              当前播放的歌曲名\n"
                      "-init -initialization  初始化音乐\n"
                      "-C -CurrentItemInLoop    单曲循环\n"
                      "-L -Loop                 列表循环\n"
                      "-R -Random               随机播放\n"
                      "-m    int int 窗口移动\n"
                      "-move int int 窗口移动\n"
                      "-c -close     关闭窗口\n";
            return 0;
        }
        //如果参数不是命令也不是文件路径，则退出
        if(!QFileInfo::exists(str)&&str!="-b"&&str!="-back"&&str!="-n"&&str!="-next"&&str!="-p"&&str!="-pause"&&
                str!="-s"&&str!="-start"&&str!="-i"&&str!="-increase"&&str!="-r"&&str!="-reduce"&&
                str!="-C"&&str!="-CurrentItemInLoop"&&str!="-L"&&str!="-Loop"&&str!="-R"&&str!="-Random"&&
                str!="-m"&&str!="-move"&&str!="-c"&&str!="-close"&&str!="-sta"&&str!="-state"&&
                str!="-t"&&str!="-title"&&str!="-init"&&str!="-initialization")
        {
            qDebug()<<"参数不合规，请使用--h或者--help参数获取帮助";
            return -1;
        }
        if(str == "-m"||str == "-move")
        {
            if(str2!=""&&str3!="")
            {
                bool ok1;
                bool ok2;
                str2.toInt(&ok1);
                str3.toInt(&ok2);
                if(!ok1 || !ok2)
                {
                    qDebug()<<"参数不合规，请使用--h或者--help参数获取帮助";
                    return -1;
                }
            }
        }
    }
    Widget w(strList);
    w.show();
    w.creartFinish();
    return a.exec();
}
