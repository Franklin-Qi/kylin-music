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

#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H

#include <QToolButton>
#include <QPainter>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QtWidgets>

const QString IS_SELECT = "IS_SELECT";
class MyToolButton : public QToolButton
{
     Q_OBJECT
public:
     MyToolButton();
    ~MyToolButton();
     void selectChanged();
     void defaultStyle();
     void requestCustomContextMenu(const QPoint &pos);
public:
    QAction *deleteAct = nullptr;
    QAction *renameAct = nullptr;
    QString buttonListName = "";
Q_SIGNALS:
    void playall(QString text);
    void renamePlayList(QString text);
    void removePlayList(QString text);

    void selectButtonChanged(QString text);
public Q_SLOTS:

private:

};


#endif
