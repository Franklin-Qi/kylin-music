#include "UI/mainwidget.h"
#include "UI/base/xatom-helper.h"
#include <QApplication>
#include <QDebug>
#include <ukui-log4qt.h>

#include "kwidget.h"
using namespace kdk;

/**
 * @brief matchHighDpiScreen 适配4K屏
 */
void matchHighDpiScreen()
{
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    #endif

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    #endif
}


/**
 * @brief handleCmdList  处理命令行参数
 * @param cmdline 获取的命令行参数
 * @param argv 命令行参数
 */
int handleCmdList(QStringList cmdList, char **argv)
{
    QString str = "";
    QString str2 = "";
    QString str3 = "";

    if(cmdList.size() > 1) {
        str = argv[1];

        if(cmdList.size() == 4) {
            str2 = argv[2];
            str3 = argv[3];
        }
    }

    if (str != "") {
        if(str == "--help"||str == "--h") {
            printf("Usage: kylin-music [options...]\n"
                      " -b, -back  上一首\n"
                      " -n, -next  下一首\n"
                      " -p, -pause  暂停\n"
                      " -s, -start  播放\n"
                      " -i, -increase  调高音量\n"
                      " -r, -reduce    调低音量\n"
                      " -t, -title              当前播放的歌曲名\n"
                      " -S, -state            当前播放状态\n"
                      " -I, -initialization  初始化音乐\n"
                      " -C, -CurrentItemInLoop    单曲循环\n"
                      " -L, -Loop                 列表循环\n"
                      " -R, -Random               随机播放\n"
                      " -m, -move int int 窗口移动\n"
                      " -c, -close     关闭窗口\n"
                   );
            return 2;
        }

        //如果参数不是命令也不是文件路径，则退出
        if(!QFileInfo::exists(str)&&str!="-b"&&str!="-back"&&str!="-n"&&str!="-next"&&str!="-p"&&str!="-pause"&&
                str!="-s"&&str!="-start"&&str!="-i"&&str!="-increase"&&str!="-r"&&str!="-reduce"&&
                str!="-C"&&str!="-CurrentItemInLoop"&&str!="-L"&&str!="-Loop"&&str!="-R"&&str!="-Random"&&
                str!="-m"&&str!="-move"&&str!="-c"&&str!="-close"&&str!="-sta"&&str!="-state"&&
                str!="-t"&&str!="-title"&&str!="-init"&&str!="-initialization") {
            printf("参数不合规，请使用--h或者--help参数获取帮助");
            return -1;
        }

        if (str == "-m"||str == "-move") {
            if (str2!=""&&str3!="") {
                bool ok1;
                bool ok2;
                str2.toInt(&ok1);
                str3.toInt(&ok2);

                if (!ok1 || !ok2) {
                    printf("参数不合规，请使用--h或者--help参数获取帮助");
                    return -1;
                }
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    QStringList strList;
    QString str1;
    for(int i = 0; i < argc;i++) {
        str1 = argv[i];
        strList << str1;
    }

    initUkuiLog4qt("kylin-music");
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    matchHighDpiScreen();

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon::fromTheme("kylin-music"));


    //////////////////////////////////////////////
    ///////////////////////////////////// 多語言支持 start

    QString locale = QLocale::system().name();
    QString qtTranslationDirectory;
    QString appTranslationDirectory;

    QTranslator apptranslator; // 应用内语言翻译
    QTranslator qtTranslator; // qt 控件语言翻译
    QTranslator peonyTranslator; // peony 控件语言翻译
    QTranslator kdkTranslator; // kdk 控件语言翻译

    if(locale == "zh_CN") {
        if(kdkTranslator.load(":/translations/gui_zh_CN.qm")) {
            a.installTranslator(&kdkTranslator);
        }
    }

    if (QDir("/usr/share/kylin-music/translations").exists()) {
         appTranslationDirectory = "/usr/share/kylin-music/translations";
    } else {
         appTranslationDirectory = qApp->applicationDirPath() + "/translations";
    }

    if(! apptranslator.load("kylin-music_" + locale + ".qm",  appTranslationDirectory)) {
        qDebug() << "Load translation file："<< "kylin-music_" + locale + ".qm from" <<  appTranslationDirectory << "failed!";
    } else {
        a.installTranslator(& apptranslator);
    }

    qtTranslationDirectory = QLibraryInfo::location(QLibraryInfo::TranslationsPath);  // /usr/share/qt5/translations
    if(!qtTranslator.load("qt_" + locale + ".qm", qtTranslationDirectory)) {
        qDebug() << "Load translation file："<< "qt_" + locale + ".qm from" << qtTranslationDirectory << "failed!";
    } else {
        a.installTranslator(&qtTranslator);
    }

    if (!peonyTranslator.load("/usr/share/libpeony-qt/libpeony-qt_"+ locale)){
        qDebug() << "Load translation file："<< "/usr/share/libpeony-qt/libpeony-qt_" + locale + ".qm from" << qtTranslationDirectory << "failed!";
    } else {
        a.installTranslator(&peonyTranslator);
    }

    ///////////////////////////////////// 多語言支持 end


    int retCmd = handleCmdList(strList, argv);
    if (retCmd == -1) {
        return -1;
    } else if (retCmd == 2) {
        return 0;
    }

    Widget w(strList);
    w.show();
    w.creartFinish();
    return a.exec();
}
