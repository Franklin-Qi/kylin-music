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

#ifndef LABEDIT_H
#define LABEDIT_H

#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

#define PT_11 11

class LabEdit : public QLineEdit
{
    Q_OBJECT
public:
    LabEdit();
    void setLabelNumber(int num);
public Q_SLOTS:
    void slotLableSetFontSize(int size);
private:
    void init();
private:
    QLabel *lab = nullptr;
    int leng = 15;
};

#endif // LABEDIT_H
