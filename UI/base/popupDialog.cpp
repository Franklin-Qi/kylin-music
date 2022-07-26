﻿/*
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

#include "popupDialog.h"
#include "widgetstyle.h"
#include "UI/mainwidget.h"
#include "xatom-helper.h"
#include <ukui-log4qt.h>

#define PT_12 12
#define PT_11 11

QRegExp gInvalidName("[\\\\/:\\*\\?\\\"&<>\\|]");/* 文件名或文件夹名中不能出现以下字符：\、/、:、*、?、"、&、<、>、|  */

PopupDialog::PopupDialog(QWidget *parent) : QDialog(parent)
{
    inpupdialog();
    dlgcolor();
}

void PopupDialog::closeDialog()
{
    this->close();
    enterLineEdit->clear();
}

void PopupDialog::slotTextChanged(QString text)
{
    KyInfo() << "text = " << text;
    if (!nameIsValid(text) && text != "") {
        QMessageBox::warning(Widget::mutual,tr("Prompt information"),tr("Could not contain characters: \ / : * ? \" & < > |"), QMessageBox::Ok);

        enterLineEdit->cursorBackward(true, 1);
        enterLineEdit->del();
    }

    enterLineEdit->setLabelNumber(15 - enterLineEdit->text().length());
    if (text.length() == 15) {
        tips->setText(tr("Reached upper character limit"));
        tips->setStyleSheet("QLabel{color:#F44E50;}");
    } else {
        tips->setText("");
    }
}

void PopupDialog::inpupdialog()
{
    this->setWindowTitle(tr("Music Player"));
    this->setFixedSize(376, 222);
//    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

//    MotifWmHints hints;
//    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
//    hints.functions = MWM_FUNC_ALL;
//    hints.decorations = MWM_DECOR_BORDER;
//    XAtomHelper::getInstance()->setWindowMotifHint(this->winId(), hints);

    this->setWindowModality(Qt::ApplicationModal); //弹出自定义对话框时主界面不可操作
//    pupDialog->setModal(true);
    testLayout = new QVBoxLayout();
    this->setLayout(testLayout);
    titleLayout = new QHBoxLayout();
    titleLab = new QLabel(this);
    closeBtn = new QToolButton(this);
    closeBtn->setFixedSize(30,30);
    closeBtn->setIcon(QIcon::fromTheme("window-close-symbolic"));
    closeBtn->setProperty("isWindowButton", 0x2);
    closeBtn->setProperty("useIconHighlightEffect", 0x8);
    closeBtn->setAutoRaise(true);
    closeBtn->hide();

//    titleLab->setText("提示信息");
    titleLab->setText(tr("Prompt information"));
    titleLab->setFixedHeight(28);

    titleLayout->addWidget(titleLab,Qt::AlignLeft);
    titleLayout->addWidget(closeBtn,0,Qt::AlignRight);

    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);

    enterLineEdit = new NewSonglistLineEdit;
    enterLineEdit->setFixedSize(312, 40);
    enterLineEdit->setMaxLength(15);

    tips = new QLabel(this);
    tips->setFixedHeight(28);


    confirmBtn = new QPushButton(this);

    cancelBtn = new QPushButton(this);

    btnLayout = new QHBoxLayout();

    stackWid = new QStackedWidget();
    stackWid->addWidget(enterLineEdit);

    stackWid->setFixedSize(312,40);
//    confirmBtn->setText("确认");
    confirmBtn->setText(tr("Confirm"));
    confirmBtn->setFixedSize(105,36);
    confirmBtn->setProperty("isImportant", true);
//    cancelBtn->setText("取消");
    cancelBtn->setText(tr("Cancel"));
    cancelBtn->setFixedSize(105,36);

    btnLayout->addWidget(cancelBtn, 0, Qt::AlignRight);
    btnLayout->addWidget(confirmBtn, Qt::AlignRight);
    btnLayout->setSpacing(20);
    btnLayout->setMargin(0);

    enterLineEdit->setPlaceholderText("请输入歌单标题：");
    enterLineEdit->setPlaceholderText(tr("Please input playlist name:"));

    testLayout->setMargin(0);
    testLayout->setSpacing(0);
    testLayout->addLayout(titleLayout);
    testLayout->addSpacing(16);
    testLayout->addWidget(stackWid);
    testLayout->addSpacing(2);
    testLayout->addWidget(tips);
    testLayout->addSpacing(14);
    testLayout->addLayout(btnLayout);
    testLayout->setContentsMargins(32, 16, 32, 24);

    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);

//    connect(addSongListBtn, SIGNAL(clicked(bool)), this, SLOT(addSongList()));
    connect(enterLineEdit,SIGNAL(textChanged(QString)),this,SLOT(slotTextChanged(QString)));
    connect(closeBtn,SIGNAL(clicked(bool)),this,SLOT(closeDialog()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(closeDialog()));

    //限制应用内字体固定大小
//    QFont sizeFont;
//    sizeFont.setPixelSize(14);
//    confirmBtn->setFont(sizeFont);
//    cancelBtn->setFont(sizeFont);
//    titleLab->setFont(sizeFont);
//    enterLineEdit->setFont(sizeFont);

}

void PopupDialog::slotLableSetFontSize(int size)
{
    //默认大小12px,换算成pt为9
    double lableBaseFontSize = PT_11;//魔鬼数字，自行处理
    double nowFontSize = lableBaseFontSize * double(size) / 11;//11为系统默认大小，魔鬼数字，自行处理
    QFont font;
    font.setPointSizeF(nowFontSize);
    tips->setFont(font);
}

void PopupDialog::dlgcolor()
{
    if(WidgetStyle::themeColor == 1 ) {

    } else if(WidgetStyle::themeColor == 0) {

    }
}

bool PopupDialog::nameIsValid(QString textName)
{
    if (nullptr == textName) {
        return false;
    }

    return !textName.contains(gInvalidName);
}


MusicInfoDialog::MusicInfoDialog(musicDataStruct date)
{
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);
    this->setWindowTitle(tr("Music Player"));
    musicDate = date;
    initStyle();
}

MusicInfoDialog::~MusicInfoDialog()
{

}


void MusicInfoDialog::initStyle()
{
    mainVLayout = new QVBoxLayout(this);
    this->setLayout(mainVLayout);
    songNameLabel = new CustomLabel(this);
    singerLabel = new CustomLabel(this);
    albumLabel = new CustomLabel(this);
    filePathLabel = new CustomLabel(this);
    fileTypeLabel = new CustomLabel(this);
    fileTimeLabel = new CustomLabel(this);
    fileSizeLable = new CustomLabel(this);
    musicInfoLabel = new QLabel(this);
    musicInfoLabel->setText(tr("Music Information"));
    musicInfoLabel->setFixedHeight(24);

    songNameLabel->setFixedHeight(28);
    singerLabel->setFixedHeight(28);
    albumLabel->setFixedHeight(28);
    fileTypeLabel->setFixedHeight(28);
    fileSizeLable->setFixedHeight(28);
    fileTimeLabel->setFixedHeight(28);
    filePathLabel->setFixedHeight(28);

    buttonLayout = new QHBoxLayout();
    cancelBtn = new QPushButton(this);
    confirmBtn = new QPushButton(this);
    btnWidget = new QWidget(this);

    connect(cancelBtn,&QPushButton::clicked,this,&MusicInfoDialog::close);
    connect(confirmBtn,&QPushButton::clicked,this,&MusicInfoDialog::close);

    btnWidget->setLayout(buttonLayout);
    buttonLayout->setAlignment(Qt::AlignRight);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(confirmBtn);
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(16);


    confirmBtn->setText(tr("Confirm"));
    confirmBtn->setFixedSize(105,36);
//    cancelBtn->setText("取消");s
    cancelBtn->setText(tr("Cancel"));
    cancelBtn->setFixedSize(105,36);
//    confirmBtn->setStyleSheet("background: #3790FA;color:#FFFFFF");
    confirmBtn->setProperty("isImportant", true);

    mainVLayout->setAlignment(Qt::AlignTop);
    mainVLayout->addWidget(musicInfoLabel);
    mainVLayout->addSpacing(10);
    mainVLayout->addWidget(songNameLabel);
    mainVLayout->addWidget(singerLabel);
    mainVLayout->addWidget(albumLabel);
    mainVLayout->addWidget(fileTypeLabel);
    mainVLayout->addWidget(fileSizeLable);
    mainVLayout->addWidget(fileTimeLabel);
    mainVLayout->addWidget(filePathLabel);
    mainVLayout->addSpacing(20);
    mainVLayout->addWidget(btnWidget);

    mainVLayout->setSpacing(6);
    mainVLayout->setContentsMargins(32,20,32,0);

    if(musicDate.title != "")
    {
        songNameLabel->setText(tr("Song Name : ") + musicDate.title);
        singerLabel  ->setText(tr("Singer : ") + musicDate.singer);
        albumLabel   ->setText(tr("Album : ") + musicDate.album);
        fileTypeLabel->setText(tr("File Type : ") + musicDate.filetype);
        fileSizeLable->setText(tr("File Size : ") + musicDate.size);
        fileTimeLabel->setText(tr("File Time : ") + musicDate.time);
        filePathLabel->setText(tr("File Path : ") + musicDate.filepath);

    }
    this->setFixedSize(376,370);

    //限制应用内字体固定大小
//    QFont sizeFont;
//    sizeFont.setPixelSize(14);
//    QFont titleFont;
//    titleFont.setPixelSize(16);
//    titleFont.setBold(true);
//    songNameLabel->setFont(sizeFont);
//    singerLabel->setFont(sizeFont);
//    albumLabel->setFont(sizeFont);
//    fileTypeLabel->setFont(sizeFont);
//    fileSizeLable->setFont(sizeFont);
//    fileTimeLabel->setFont(sizeFont);
//    filePathLabel->setFont(sizeFont);
//    musicInfoLabel->setFont(titleFont);

}

void MusicInfoDialog::slotLableSetFontSize(int size)
{
    //默认大小12px,换算成pt为9
    double lableBaseFontSize = PT_12;//魔鬼数字，自行处理
    double nowFontSize = lableBaseFontSize * double(size) / 11;//11为系统默认大小，魔鬼数字，自行处理
    QFont font;
    font.setPointSizeF(nowFontSize);
    musicInfoLabel->setFont(font);
}
