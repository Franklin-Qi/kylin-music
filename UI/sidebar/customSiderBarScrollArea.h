#ifndef CUSTOMSIDERBARSCROLLAREA_H
#define CUSTOMSIDERBARSCROLLAREA_H

#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>

class CustomSiderBarScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit CustomSiderBarScrollArea(QWidget *parent = nullptr);
    ~CustomSiderBarScrollArea();


};

#endif // CUSTOMSIDERBARSCROLLAREA_H
