#ifndef TABLEBASEVIEW_H
#define TABLEBASEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItemModel>
#include "UIControl/tableview/musiclistmodel.h"
#include "UIControl/base/musicDataBase.h"
#include "tableviewdelegate.h"
class TableBaseView : public QTableView
{
    Q_OBJECT
//    void mouseMoveEvent(QMouseEvent *event);
public:
    explicit TableBaseView(QTableView *parent = nullptr);
    ~TableBaseView();
    MusicListModel *m_model;
protected:
    void mouseMoveEvent(QMouseEvent *event)override;
    void leaveEvent(QEvent *event)override;
signals:
    void hoverIndexChanged(QModelIndex index);
    void leaveFromItem();
private:
    TableViewDelegate *m_delegate;
private:
    void initStyle();
};

#endif // TABLEBASEVIEW_H
