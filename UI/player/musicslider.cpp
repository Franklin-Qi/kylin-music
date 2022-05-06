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

#include "musicslider.h"
#include "UI/base/widgetstyle.h"
#include "UIControl/player/player.h"

MusicSlider::MusicSlider(QWidget *parent):QSlider(parent),m_isPlaying(false)
{
    //现在进度条样式已基本符合要求，但是两端稍微有点瑕疵，暂搁置
    //逻辑也稍微有点问题，如果不选择歌曲，进度条应该禁止操作
    //所以将音乐播放进度条独立出来，方便协作和扩展

//    this->installEventFilter(this);
    initStyle();//初始化样式
}

void MusicSlider::initStyle()
{
    this->setFixedHeight(22);
    this->setOrientation(Qt::Horizontal);
//    this->setMinimum(0);
//    this->setMaximum(1000);
    if(WidgetStyle::themeColor == 1) {
        this->setStyleSheet("QSlider::groove:horizontal{left:0px;right:0px;height: 2px;background: transparent;}"
                            "QSlider::sub-page:horizontal{background:#3790FA;}"
                            "QSlider::add-page:horizontal{background:#4D4D4D;}"
                            );
    }
    else if(WidgetStyle::themeColor == 0) {
        this->setStyleSheet("QSlider::groove:horizontal{left:0px;right:0px;height: 2px;background: transparent;}"
                            "QSlider::sub-page:horizontal{background:#3790FA;}"
                            "QSlider::add-page:horizontal{background:#ECEEF5;}"
                            );
    }

}

void MusicSlider::isPlaying(bool isPlaying)
{
    m_isPlaying = isPlaying;
}

void MusicSlider::mousePressEvent(QMouseEvent *event)
{
    if(m_isPlaying == false)
    {
        return QSlider::mousePressEvent(event);
    }
    this->blockSignals(true);
    if (event->button() == Qt::LeftButton) //判断左键
    {
        int value = this->minimum() + ((this->maximum() - this->minimum()) * ((double)event->x())) / (this->width());
        setValue(value);
    }

}

void MusicSlider::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPlaying == false)
    {
        return QSlider::mousePressEvent(event);
    }

    auto rang = this->width();

    if(rang == 0)
    {
        return;
    }

    auto value = this->minimum() + ((this->maximum() - this->minimum()) * ((double)event->x())) / (this->width());
    setValue(value);
}

void MusicSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_isPlaying == false)
    {
        return QSlider::mousePressEvent(event);
    }
    this->blockSignals(false);
    QSlider::mouseReleaseEvent(event);

    int range = this->maximum() - minimum();
    Q_ASSERT(range != 0);
    if(value() <= range)
    {
        int position = value() * playController::getInstance().getPlayer()->duration() / range;
        playController::getInstance().getPlayer()->setPosition(position);
    }

}

void MusicSlider::enterEvent(QEvent *event)
{
    if(WidgetStyle::themeColor == 1)
    {
        this->setStyleSheet(
                    "QSlider::groove:horizontal{left:0px;right:0px;position: absolute;height: 2px;background: transparent;}"
                    "QSlider::sub-page:horizontal{background:#3790FA;}"
                    "QSlider::add-page:horizontal{background:#4D4D4D;}"
                    "QSlider::handle:horizontal {\
                            width:12px;\
                            height:12px;\
                            margin-top: -5px;\
                            margin-left: 0px;\
                            margin-bottom: -5px;\
                            margin-right: 0px;\
                            border-image:url(:/img/default/point.png);\
                    }");
    } else if (WidgetStyle::themeColor == 0){
        this->setStyleSheet(
                    "QSlider::groove:horizontal{left:0px;right:0px;position: absolute;height: 2px;background: transparent;}"
                    "QSlider::sub-page:horizontal{background:#3790FA;}"
                    "QSlider::add-page:horizontal{background:#ECEEF5;}"
                    "QSlider::handle:horizontal {\
                            width:12px;\
                            height:12px;\
                            margin-top: -5px;\
                            margin-left: 0px;\
                            margin-bottom: -5px;\
                            margin-right: 0px;\
                            border-image:url(:/img/default/point.png);\
                    }"
                    );
    }
//    QSlider::enterEvent(event);
}

void MusicSlider::leaveEvent(QEvent *event)
{
    initStyle();
    QSlider::leaveEvent(event);
}

void MusicSlider::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
}
