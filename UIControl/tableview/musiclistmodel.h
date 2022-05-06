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

#ifndef SCOREINFOMODEL_H
#define SCOREINFOMODEL_H

#include <QObject>
#include <QStandardItem>
#include <QTableView>
#include <QList>
#include <QDebug>
#include "UIControl/base/musicDataBase.h"

class MusicListModel : public QObject
{
    Q_OBJECT
public:
    explicit MusicListModel(QObject *parent = 0);
    bool add(musicDataStruct info);
    bool add(QList<musicDataStruct> list);
    bool remove(int i);
    int count();
    void clear();
    void setView(QTableView &view);
    musicDataStruct getItem(int i);
    QStringList getPathList(QString listName);
    void getMusicDateList(QList<musicDataStruct> list);
    QStandardItemModel m_model;
    QStringList titleList;
    QStandardItem* item[4];
private:
    QList<musicDataStruct> resList;

};

#endif // SCOREINFOMODEL_H
