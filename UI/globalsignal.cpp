#include "globalsignal.h"

GlobalUserSignal* GlobalUserSignal::instance = new GlobalUserSignal;

GlobalUserSignal *GlobalUserSignal::getInstance()
{
    return instance;
}

void GlobalUserSignal::exitApp()
{
    emit sigExit();
}

GlobalUserSignal::GlobalUserSignal(QObject *parent) : QObject(parent)
{

}
