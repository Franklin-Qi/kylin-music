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
    QString getSearchText()const;
public:
    void setSearchText(QString text);
protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
Q_SIGNALS:
    void hoverIndexChanged(QModelIndex index);
    void leaveFromItem();
private:
    TableViewDelegate *m_delegate;
    QString m_searchText;
private:
    void initStyle();
};

#endif // TABLEBASEVIEW_H
