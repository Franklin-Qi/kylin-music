#include "menumodule.h"
#include "UI/mainwidget.h"
#include "UI/base/xatom-helper.h"
#include "UI/globalsignal.h"

#define PT_14 14

menuModule::menuModule(QWidget *parent = nullptr) : QWidget(parent)
{
    init();
}

void menuModule::init()
{
    initAction();
    setStyle();
}

void menuModule::initAction()
{
    menuButton = new QToolButton;
    menuButton->setToolTip(tr("Menu"));
    menuButton->setProperty("isWindowButton", 0x1);
    menuButton->setProperty("useIconHighlightEffect", 0x2);
    menuButton->setPopupMode(QToolButton::InstantPopup);
    menuButton->setFixedSize(30,30);
    menuButton->setIconSize(QSize(16, 16));
    menuButton->setAutoRaise(true);
    menuButton->setIcon(QIcon::fromTheme("application-menu"));

    m_menu = new QMenu();

    QList<QAction *> actions ;
    QAction *actionTheme = new QAction(m_menu);
    actionTheme->setText(tr("Theme"));
    QAction *actionHelp = new QAction(m_menu);
    actionHelp->setText(tr("Help"));
    QAction *actionAbout = new QAction(m_menu);
    actionAbout->setText(tr("About"));
    QAction *actionExit = new QAction(m_menu);
    actionExit->setText(tr("Exit"));

    actions << actionHelp << actionAbout << actionExit;
    m_menu->addActions(actions);

//    互斥按钮组
    QMenu *themeMenu = new QMenu;
    QActionGroup *themeMenuGroup = new QActionGroup(this);
    QAction *autoTheme = new QAction(tr("Auto"),this);
    autoTheme->setObjectName("Auto");
    themeMenuGroup->addAction(autoTheme);
    themeMenu->addAction(autoTheme);
    autoTheme->setCheckable(true);
    QAction *lightTheme = new QAction(tr("Light"),this);
    lightTheme->setObjectName("Light");
    lightTheme->setCheckable(true);
    QAction *darkTheme = new QAction(tr("Dark"),this);
    darkTheme->setObjectName("Dark");
    darkTheme->setCheckable(true);
    QList<QAction* > themeActions;
    themeActions<<autoTheme<<lightTheme<<darkTheme;
    actionTheme->setMenu(themeMenu);

    menuButton->setMenu(m_menu);

    initGsetting();
    setThemeFromLocalThemeSetting(themeActions);
    themeUpdate();

    connect(m_menu,&QMenu::triggered,this,&menuModule::triggerMenu);
    connect(themeMenu,&QMenu::triggered,this,&menuModule::triggerThemeMenu);
    //键盘F1响应唤出用户手册绑定
    connect(Widget::mutual,&Widget::signalShowGuide,this,&menuModule::helpAction);
}

void menuModule::setThemeFromLocalThemeSetting(QList<QAction* > themeActions)
{
#if DEBUG_MENUMODULE
    confPath = "org.kylin-music-data.settings";
#endif
    m_pGsettingThemeStatus = new QGSettings(confPath.toLocal8Bit());
    if(!m_pGsettingThemeStatus){
    	qDebug()<<"系统对应的gsetting字段未正确配置";
    }

    QString appConf = m_pGsettingThemeStatus->get("thememode").toString();
    if ("lightonly" == appConf){
        themeStatus = themeLightOnly;
        themeActions[1]->setChecked(true);   //程序gsetting中为浅色only的时候就给浅色按钮设置checked
    } else if ("darkonly" == appConf){
        themeStatus = themeBlackOnly;
        themeActions[2]->setChecked(true);
    } else {
        themeStatus = themeAuto;
        themeActions[0]->setChecked(true);
    }
}

void menuModule::themeUpdate()
{
      setStyleByThemeGsetting();   //应用颜色更新只跟随主题
}

void menuModule::setStyleByThemeGsetting()
{
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();

    if("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle) {
        setThemeDark();
    } else {
        setThemeLight();
    }
}

void menuModule::triggerMenu(QAction *act)
{
    QString str = act->text();

    if (tr("Exit") == str){
        emit g_user_signal->exitApp();
    } else if (tr("About") == str){
        aboutAction();
    } else if (tr("Help") == str){
        helpAction();
    }
}

void menuModule::triggerThemeMenu(QAction *act)
{
    if(!m_pGsettingThemeStatus)
    {
        m_pGsettingThemeStatus = new QGSettings(confPath.toLocal8Bit());  //m_pGsettingThemeStatus指针重复使用避免占用栈空间
    }
    QString str = act->objectName();
    if("Light" == str){
        themeStatus = themeLightOnly;
        disconnect(m_pGsettingThemeData,&QGSettings::changed,this,&menuModule::dealSystemGsettingChange);
        m_pGsettingThemeStatus->set("thememode","lightonly");
//        disconnect()
        setThemeLight();
    }else if("Dark" == str){
        themeStatus = themeBlackOnly;
        disconnect(m_pGsettingThemeData,&QGSettings::changed,this,&menuModule::dealSystemGsettingChange);
        m_pGsettingThemeStatus->set("thememode","darkonly");
        setThemeDark();
    }else{
        themeStatus = themeAuto;
        m_pGsettingThemeStatus->set("thememode","auto");
        initGsetting();
//        updateTheme();
        themeUpdate();
    }
}

/**
 * @brief menuModule::aboutAction
 * 关于点击事件处理
 */
void menuModule::aboutAction()
{
    initAbout();
}

/**
 * @brief menuModule::helpAction
 * 帮助点击事件处理
 */
void menuModule::helpAction()
{
#if DEBUG_MENUMODULE
    appName = "tools/kylin-music";
#endif
    if(!ipcDbus)
    {
        ipcDbus = new DaemonIpcDbus();
    }
    if(!ipcDbus->daemonIsNotRunning())
    {
        ipcDbus->showGuide(appName);
    }
}

void menuModule::initAbout()
{
    m_aboutWindow = new kdk::KAboutDialog(this,QIcon::fromTheme("kylin-music"),tr(appShowingName.toLocal8Bit()),tr("Version: ") + APP_VERSION);
    m_aboutWindow->setBodyText(tr("Music player is a multimedia playback software.Cover Various music formats Playback tools for,fast and simple."));
    m_aboutWindow->setBodyTextVisiable(false);
    m_aboutWindow->setWindowModality(Qt::WindowModal);
    m_aboutWindow->setWindowModality(Qt::ApplicationModal);

    m_aboutWindow->show();
    m_aboutWindow->exec();
}


void menuModule::slotLableSetFontSize(int size)
{
    //默认大小12px,换算成pt为9
    double lableBaseFontSize = PT_14;//魔鬼数字，自行处理
    double nowFontSize = lableBaseFontSize * double(size) / 11;//11为系统默认大小，魔鬼数字，自行处理
    QFont font;
    font.setPointSizeF(nowFontSize);
}


void menuModule::setStyle()
{
//    menuButton->setStyleSheet("QPushButton::menu-indicator{image:None;}");
}

void menuModule::initGsetting()
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOWS)){
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOWS);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,&menuModule::dealSystemGsettingChange);
    }
}

void menuModule::dealSystemGsettingChange(const QString key)
{
    if(key == "styleName"){
        refreshThemeBySystemConf();
    }
}

void menuModule::refreshThemeBySystemConf()
{
    QString themeNow = m_pGsettingThemeData->get("styleName").toString();
    if ("ukui-dark" == themeNow || "ukui-black" == themeNow){
        setThemeDark();
    } else {
        setThemeLight();
    }
}

void menuModule::setThemeDark()
{
    Q_EMIT menuModuleSetThemeStyle("dark-theme");
}

void menuModule::setThemeLight()
{
    Q_EMIT menuModuleSetThemeStyle("light-theme");

}
