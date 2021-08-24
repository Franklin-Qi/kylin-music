#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include <QLineEdit>
#include <QDebug>
#include "UI/player/searchresult.h"

class SearchResult;
class SearchEdit : public QLineEdit
{
    Q_OBJECT
public:
    SearchEdit(QWidget *parent = nullptr);

public:
    void setWidget(QWidget *mainWidget);
    void moveSearchResult();
signals:
    void sigFoucusIn();
    //发送展示搜索结果界面
    void signalReturnPressed(QString searchEdit);
    //搜索的关键字
    void signalReturnText(QString text);
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;


public slots:
    void slotTextChanged();
    void slotReturnPressed();

public:
    SearchResult *m_result = nullptr;

private:

    QWidget *m_mainWidget = nullptr;
};

#endif // SEARCHEDIT_H
