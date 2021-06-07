#include "musicslider.h"
#include "UI/base/widgetstyle.h"

MusicSlider::MusicSlider(QWidget *parent):QSlider(parent),m_isPlaying(false)
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
//    this->setMinimum(0);
//    this->setMaximum(1000);
    if(WidgetStyle::themeColor == 1) {
        this->setStyleSheet("QSlider::groove:horizontal{left:-1px;right:-1px;height: 2px;background: transparent;}"
                            "QSlider::sub-page:horizontal{background:#3790FA;}"
                            "QSlider::add-page:horizontal{background:#4D4D4D;}"
                            );
    }
    else if(WidgetStyle::themeColor == 0) {
        this->setStyleSheet("QSlider::groove:horizontal{left:-1px;right:-1px;height: 2px;background: transparent;}"
                            "QSlider::sub-page:horizontal{background:#3790FA;}"
                            "QSlider::add-page:horizontal{background:#ECEEF5;}"
                            );
    }

}

void MusicSlider::isPlaying(bool isPlaying)
{
    m_isPlaying = isPlaying;
}

bool MusicSlider::eventFilter(QObject * obj, QEvent * ev)
{
    if (obj == this && ev->type() == QEvent::Leave)
    {
        initStyle();
//        this->setStyleSheet(
//                    "QSlider::groove:horizontal{left:-1px;right:-1px;height: 2px;background: transparent;}"
//                    "QSlider::sub-page:horizontal{background:#3790FA;}"
//                    "QSlider::add-page:horizontal{background:#ECEEF5;}"
//                    );
    }
    else if (obj == this && ev->type() == QEvent::Enter)
    {
        if(WidgetStyle::themeColor == 1)
        {
            this->setStyleSheet(
                        "QSlider::groove:horizontal{left:-1px;right:-1px;height: 2px;background: transparent;}"
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
                        "QSlider::groove:horizontal{left:-1px;right:-1px;height: 2px;background: transparent;}"
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

//        this->setStyleSheet( "   QSlider::groove:horizontal{                 "
//                             "           padding-left:0px;                   "
//                             "           height: 2px;                        "
//                             "           border:0px;                         "
//                             "   }                                           "
//                            "QSlider::sub-page:horizontal {background: #3790FA;}"

//                            "QSlider::add-page:horizontal {background: #ECEEF5;}"

//                            "QSlider::handle:horizontal {\
//                                background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,\
//                                stop:0 #3790FA, stop:0.1 #3790FA);\
//                                width: 10px;\
//                                height:10px;\
//                                margin-top: -5px;\
//                                margin-left: 0px;\
//                                margin-bottom: -5px;\
//                                margin-right: 0px;\
//                                border-radius: 5px;\
//                            }"
//                            );
    }
    else if(obj == this)
    {
        if(m_isPlaying == false)
        {
            return QWidget::eventFilter(obj, ev);
        }
        if (ev->type()==QEvent::MouseButtonPress)           //判断类型
        {
            qDebug() << "m_isPlaying" << m_isPlaying;
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
