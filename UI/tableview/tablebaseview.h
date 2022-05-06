/*
 * Copyright (C) 2021, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
    MusicListModel *m_model = nullptr;
    QString getSearchText()const;
    QString getSearchListName()const;
    QString getNowPlayListName()const;
public:
    void setSearchText(QString text);
    void setSearchListName(QString listName);
public Q_SLOTS:
    void setNowPlayListName(QString listName);
protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
Q_SIGNALS:
    void hoverIndexChanged(QModelIndex index);
    void leaveFromItem();
private:
    TableViewDelegate *m_delegate = nullptr;
    QString m_searchText;
    QString m_searchListName;
    QString m_nowListName = "";
private:
    void initStyle();
};

#endif // TABLEBASEVIEW_H
