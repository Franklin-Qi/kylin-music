#ifndef GLOBALSIGNAL_H
#define GLOBALSIGNAL_H

#include <QObject>


/** ******************************
 * 用户全局信号，主要用来发送主动信号
 *********************************/
class GlobalUserSignal : public QObject
{
    Q_OBJECT
public:
    static GlobalUserSignal* getInstance();

    void exitApp(); // 退出

signals:
    void sigExit();

private:
    explicit GlobalUserSignal(QObject *parent = nullptr);
    static GlobalUserSignal* instance;
};


#define g_user_signal GlobalUserSignal::getInstance()

#endif // GLOBALSIGNAL_H
