#include "musicslider.h"

MusicSlider::MusicSlider(QWidget *parent):QSlider(parent)
{
    //现在进度条样式已基本符合要求，但是两端稍微有点瑕疵，暂搁置
    //逻辑也稍微有点问题，如果不选择歌曲，进度条应该禁止操作
    //所以将音乐播放进度条独立出来，方便协作和扩展

    this->installEventFilter(this);
    initStyle();//初始化样式
}

void MusicSlider::initStyle()
{
    this->setFixedHeight(22);
    this->setOrientation(Qt::Horizontal);
    this->setStyleSheet("QSlider::groove:horizontal{padding-left:0px;height: 2px;background:#3790FA;}"
                        "QSlider::sub-page:horizontal{left: 10px;right:10px;background:#3790FA;}"
                        "QSlider::add-page:horizontal{background:#ECEEF5;}"
                        );
}

bool MusicSlider::eventFilter(QObject * obj, QEvent * ev)
{
    if (obj == this && ev->type() == QEvent::Leave)
    {
        this->setStyleSheet(
                    "   QSlider::groove:horizontal{                 "
                    "           padding-left:0px;                   "
                    "           height: 2px;                        "
                    "           border:0px;                         "
                    "   }                                           "
                    "   QSlider::sub-page:horizontal{               "
                    "           background:#3790FA;      "
                    "   }                                           "
                    "   QSlider::add-page:horizontal{               "
                    "           background:#ECEEF5;      "
                    "   }                                           "
                    "   QSlider::handle:horizontal{          "
                    "           width:24px;                         "
                    "           height:24px;                        "
                    "           margin-top: -11px;                  "
                    "           margin-left: -12px;                 "
                    "           margin-bottom: -11px;               "
                    "           margin-right: -12px;                "
                    "   }                                           "
                    );
    }
    else if (obj == this && ev->type() == QEvent::Enter)
    {
        this->setStyleSheet(
                    "   QSlider::groove:horizontal{                 "
                    "           padding-left:0px;                   "
                    "           height: 2px;                        "
                    "           border:0px;                         "
                    "   }                                           "
                    "   QSlider::sub-page:horizontal{               "
                    "           background:#3790FA;                 "
                    "   }                                           "
                    "   QSlider::add-page:horizontal{               "
                    "           background:#ECEEF5;                 "
                    "   }                                           "
                    "   QSlider::handle:horizontal {                "
                    "           width:24px;                         "
                    "           height:24px;                        "
                    "           margin-top: -11px;                  "
                    "           margin-left: -12px;                 "
                    "           margin-bottom: -11px;               "
                    "           margin-right: -12px;                "
                    "           border-image:url(:/img/default/point.png); "
                    "   }                                           "
                    );
    }
    else if(obj == this)
    {
        if (ev->type()==QEvent::MouseButtonPress)           //判断类型
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(ev);
            if (mouseEvent->button() == Qt::LeftButton) //判断左键
            {
                int dur = this->maximum() - this->minimum();
                int pos = this->minimum() + dur * ((double)mouseEvent->x() / this->width());
                if(pos != this->sliderPosition())
                {
                    this->setValue(pos);
                }
            }
        }
    }
    return QWidget::eventFilter(obj, ev);
}

void MusicSlider::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
}
