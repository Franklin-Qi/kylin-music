/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
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

#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

#include <QMessageBox>
#include <QRegExp>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDialog>
#include <QPainter>
#include <QTextEdit>
#include <QLineEdit>
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QDebug>
#include <QMessageBox>
#include "UIControl/base/musicDataBase.h"
#include "UI/base/customLabel.h"
#include "UI/base/newSonglistLineEdit.h"

#include "kdialog.h"
using namespace kdk;

class PopupDialog :public KDialog
{
    Q_OBJECT
public:
    explicit PopupDialog(QWidget *parent = nullptr);

    QPushButton *confirmBtn = nullptr;
    QPushButton *cancelBtn = nullptr;
    QToolButton *closeBtn = nullptr;
    QLabel      *titleLab = nullptr;
    QLabel      *tips = nullptr;
    NewSonglistLineEdit   *enterLineEdit = nullptr;
    QVBoxLayout *testLayout = nullptr;
    QHBoxLayout *titleLayout = nullptr;
    QHBoxLayout *btnLayout = nullptr;

    QStackedWidget *stackWid = nullptr;
    void dlgcolor();

    bool nameIsValid(QString textName);


public Q_SLOTS:
    void closeDialog();
    void slotTextChanged(QString text);
    void slotLableSetFontSize(int size);

private:
    void inpupdialog();

};


class  MusicInfoDialog : public QDialog
{
    Q_OBJECT
public:

    explicit MusicInfoDialog(musicDataStruct date);
    ~MusicInfoDialog();
    musicDataStruct musicDate;

    void initStyle();

public Q_SLOTS:
    //字体
    void slotLableSetFontSize(int size);

private:
    CustomLabel *songNameLabel = nullptr;
    CustomLabel *singerLabel = nullptr;
    CustomLabel *albumLabel = nullptr;
    CustomLabel *filePathLabel = nullptr;
    CustomLabel *fileTypeLabel = nullptr;
    CustomLabel *fileTimeLabel = nullptr;
    CustomLabel *fileSizeLable = nullptr;

    QLabel *musicInfoLabel = nullptr;

    QVBoxLayout *mainVLayout = nullptr;

    QHBoxLayout *buttonLayout = nullptr;
    QWidget *btnWidget = nullptr;
    QPushButton *cancelBtn = nullptr;
    QPushButton *confirmBtn = nullptr;

};

#endif // POPUPDIALOG_H
