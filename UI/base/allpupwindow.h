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

#ifndef ALLPUPWINDOW_H
#define ALLPUPWINDOW_H
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
#include "UI/base/mylabel.h"

class AllPupWindow :public QWidget
{
    Q_OBJECT
public:
    explicit AllPupWindow(QWidget *parent = nullptr);

    QDialog     *pupDialog;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
    QToolButton *closeBtn;
    QLabel      *titleLab;
    QLineEdit   *enterLineEdit;
    QVBoxLayout *testLayout;
    QHBoxLayout *titleLayout;
    QHBoxLayout *btnLayout;

    QStackedWidget *stackWid;
    void dlgcolor();



public slots:
    void closeDialog();

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
public slots:
    //字体
    void slotLableSetFontSize(int size);
private:
    MyLabel *songNameLabel;
    MyLabel *singerLabel;
    MyLabel *albumLabel;
    MyLabel *filePathLabel;
    MyLabel *fileTypeLabel;
    MyLabel *fileTimeLabel;
    MyLabel *fileSizeLable;

    QLabel *musicInfoLabel;

    QVBoxLayout *mainVLayout;

    QHBoxLayout *buttonLayout;
    QWidget *btnWidget;
    QPushButton *cancelBtn;
    QPushButton *confirmBtn;

    void initStyle();
};

#endif // ALLPUPWINDOW_H
