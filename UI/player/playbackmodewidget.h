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

#ifndef PLAYBACKMODEWIDGET_H
#define PLAYBACKMODEWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QIcon>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QEvent>
#include <QDebug>
#include <QX11Info>

class PlayBackModeWidget : public QDialog
{
    Q_OBJECT
public:
    explicit PlayBackModeWidget(QWidget *parent = nullptr);
public:
    void changePlayModePos(int posX, int posY, int width, int height);
    void playModecolor();
public:
    QToolButton *loopBtn = nullptr;
    QToolButton *randomBtn = nullptr;
    QToolButton *sequentialBtn = nullptr;
    QToolButton *currentItemInLoopBtn = nullptr;
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;
Q_SIGNALS:

private:
    void initWidget();
private:
    int playModePosX;
    int playModePosY;
    int playModePosWidth;
    int playModePosHeight;
};

#endif // PLAYBACKMODEWIDGET_H
