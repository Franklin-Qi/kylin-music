/*
 * Copyright (C) 2013 ~ 2015 National University of Defense Technology(NUDT) & Kylin Ltd.
 *
 * Authors:
 *  Kobe Lee    xiangli@ubuntukylin.com/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kylintoolbutton.h"
#include <QDebug>
#include <QtSvg/QSvgRenderer>

KylinToolButton::KylinToolButton(const QString &pic_name, const QString &text, QWidget *parent)
    :QToolButton(parent)
{
    setDown(false);
    setFocusPolicy(Qt::NoFocus);

    txt=text;
    this->setText(text);
    this->setStyleSheet("color:white");


    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setPopupMode(QToolButton::InstantPopup);

    pixmap.load(pic_name);

	setAutoRaise(true);
    this->setObjectName("transparentToolButton");
	mouse_over = false;
	m_mousePressed = false;
    pressed = false;
}

KylinToolButton::~KylinToolButton()
{

}

void KylinToolButton::enterEvent(QEvent *event)
{
    if(isEnabled())
    {
        mouse_over = true;
    }
}

void KylinToolButton::leaveEvent(QEvent *event)
{
    if(isEnabled())
    {
        mouse_over = false;
    }
}

void KylinToolButton::mousePressEvent(QMouseEvent *event)
{
    if(isEnabled() && event->button() == Qt::LeftButton)
    {
    }
    QToolButton::mousePressEvent(event);
}

/**
  * 在此函数里面绘制按钮的三态样式
  *
  * @param   {QPaintEvent *}event
  * @return  null
**/
void KylinToolButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing); //反锯齿

    //设置渐变色的样式和范围
    QLinearGradient linear(this->height()/2,0,this->height()/2,this->width());
    linear.setColorAt(0, Qt::transparent);
    linear.setColorAt(1, QColor(255,255,255,80));


    //绘制按钮的 hover 状态的样式
    if(mouse_over)
    {
        painter.setPen(Qt::transparent);
        painter.setBrush(linear);//设置渐变色给 brush
        painter.drawRect(0,0,this->height(),this->width());//用渐变色填充这个 Rect 区域
        painter.drawPixmap(QRect(22,8,48,48),QPixmap(pixmap)); //绘制 pixmap 图片
//        qDebug() << Q_FUNC_INFO << txt;
        painter.setPen(Qt::white);
        painter.drawText(QRect(0,60,this->width(),25),Qt::AlignCenter,txt); //绘制文字
    }
    else //绘制按钮从 hover 转换到非 hover 状态的样式
    {
        painter.setPen(Qt::transparent);
        painter.setBrush(Qt::transparent);
        painter.drawRect(0,0,this->height(),this->width());
        painter.drawPixmap(QRect(22,8,48,48),QPixmap(pixmap));
//        qDebug() << Q_FUNC_INFO << txt;
        painter.setPen(Qt::white);
        painter.drawText(QRect(0,60,this->width(),25),Qt::AlignCenter,txt);
    }

    //绘制按钮的 pressed 状态的样式
    if(pressed)
    {
        painter.setPen(Qt::transparent);
        painter.setBrush(linear);   //设置渐变色给 brush
        painter.drawRect(0,0,this->height(),this->width()); //将渐变色填充带这个 Rect 区域
        painter.drawPixmap(QRect(22,8,48,48),QPixmap(pixmap));  //绘制 pixmap 图片
//        qDebug() << Q_FUNC_INFO << txt;
        painter.setPen(Qt::white);
        painter.drawText(QRect(0,60,this->width(),25),Qt::AlignCenter,txt); //绘制文字
    }
    else //绘制按钮从 pressed 转换到非 pressed 状态的样式
    {
        painter.setPen(Qt::transparent);
        painter.setBrush(Qt::transparent);
        painter.drawRect(0,0,this->height(),this->width());
        painter.drawPixmap(QRect(22,8,48,48),QPixmap(pixmap));
//        qDebug() << Q_FUNC_INFO << txt;
        painter.setPen(Qt::white);
        painter.drawText(QRect(0,60,this->width(),25),Qt::AlignCenter,txt);
    }

}


void KylinToolButton::setMousePress(bool is_press)
{
    this->m_mousePressed = is_press;

    if(this->m_mousePressed)
    {
        this->update();
        pressed = true;
    }
    else
    {
        this->update();
        pressed = false;
    }

}
