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

#include "labedit.h"
#include <QRegExpValidator>

LabEdit::LabEdit()
{
    init();
}

void LabEdit::init()
{
    QHBoxLayout* mainLayout = new QHBoxLayout();
    lab = new QLabel(this);
    lab->setText(QString("%1").arg(leng));
    mainLayout->addWidget(lab);
    mainLayout->setAlignment(Qt::AlignRight);
    this->setLayout(mainLayout);

    QRegExp rx = QRegExp("[^\\\\/:*?\"&<>|]*"); //限制以下特殊符号在lineEdit中的输入
    QRegExpValidator* validator = new QRegExpValidator(rx);
    this->setValidator(validator);
}

void LabEdit::setLabelNumber(int num)
{
    lab->setText(QString("%1").arg(num));

    if (num == 0) {
        lab->setStyleSheet("QLabel{color:#F44E50;}");
    } else {
        lab->setStyleSheet("QLabel{color:#000000;}");
    }
}

void LabEdit::slotLableSetFontSize(int size)
{
    //默认大小12px,换算成pt为9
    double lableBaseFontSize = PT_11;//魔鬼数字，自行处理
    double nowFontSize = lableBaseFontSize * double(size) / 11;//11为系统默认大小，魔鬼数字，自行处理
    QFont font;
    font.setPointSizeF(nowFontSize);
    lab->setFont(font);
}
