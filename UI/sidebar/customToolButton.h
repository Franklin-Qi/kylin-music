#ifndef CUSTOMTOOLBUTTON_H
#define CUSTOMTOOLBUTTON_H

#include <QToolButton>
#include <QPainter>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLabel>
#include <QtWidgets>
#include <QEvent>

const QString IS_SELECT = "IS_SELECT";
class CustomToolButton : public QToolButton
{
     Q_OBJECT

public:
     CustomToolButton(QWidget *parent = 0);
    ~CustomToolButton();

     void initLayout();
     void setLablePixmap(QString img);
     void setLableText(QString text);
     QString getLableText();
     void setSelectedAndHoverdStyle();
     void selectChanged();
     void defaultStyle();
     void requestCustomContextMenu(const QPoint &pos);

     void enterEvent(QEvent *event);
     void leaveEvent(QEvent *event);

public:
    QAction *deleteAct = nullptr;
    QAction *renameAct = nullptr;
    QString buttonListName = "";

Q_SIGNALS:
    void playall(QString text);
    void renamePlayList(QString text);
    void removePlayList(QString text);
    void selectButtonChanged(QString text);

private:
    bool m_isEnter = false;

    QLabel *m_iconLable = nullptr;
    QLabel *m_textLabel = nullptr;
    QHBoxLayout *m_hboxLayout = nullptr;

};

#endif
