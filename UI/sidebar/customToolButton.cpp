#include "customToolButton.h"
#include "UI/base/widgetstyle.h"
#include "UIControl/base/musicDataBase.h"
#include "UI/base/widgetstyle.h"
#include <QDebug>
#include <QPixmap>
#include <QImage>

CustomToolButton::CustomToolButton(QWidget *parent): QToolButton(parent),
    m_iconLable(new QLabel()),
    m_textLabel(new QLabel()),
    m_hboxLayout(new QHBoxLayout())
{
    initLayout();

//    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); //文字在图标旁边
    this->setIconSize(QSize(16,16));
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,&CustomToolButton::clicked,this,&CustomToolButton::selectChanged);
    connect(this,&CustomToolButton::customContextMenuRequested,this,&CustomToolButton::selectChanged);
    connect(this,&CustomToolButton::customContextMenuRequested,this,&CustomToolButton::requestCustomContextMenu);

}

CustomToolButton::~CustomToolButton()
{

}

void CustomToolButton::initLayout()
{
    this->setFixedSize(180, 36);

    m_iconLable->setFixedSize(16, 16);
//    QPalette pe;
//    //设置黑底红字
//    pe.setColor(QPalette::Background,Qt::black);
//    m_iconLable->setPalette(pe);
//    m_iconLable->setStyleSheet("background-color: #2b5612;");

//    setLablePixmap(":/img/default/ukui-folder-music-symbolic.svg");

    m_hboxLayout->setSpacing(0);
    m_hboxLayout->addWidget(m_iconLable);
    m_hboxLayout->addSpacing(8);
    m_hboxLayout->addWidget(m_textLabel);
    m_hboxLayout->setContentsMargins(16, 8, 16, 8);

    this->setLayout(m_hboxLayout);
}

void CustomToolButton::setLablePixmap(QString img)
{
    QImage Image;
    Image.load(img);

    QPixmap pixmap = QPixmap::fromImage(Image);
    int with = m_iconLable->width();
    int height = m_iconLable->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); // 饱满填充
    m_iconLable->setPixmap(fitpixmap);

}

void CustomToolButton::setLableText(QString text)
{
    m_textLabel->setText(text);
}

QString CustomToolButton::getLableText()
{
    return m_textLabel->text();
}

void CustomToolButton::setSelectedAndHoverdStyle()
{
    if(buttonListName == ALLMUSIC) {
        setLablePixmap(":/img/clicked/ukui-folder-music-symbolic.svg");
    } else if(buttonListName == FAV) {
        setLablePixmap(":/img/clicked/ukui-play-love-symbolic.svg");
    } else {
        setLablePixmap(":/img/clicked/audio-card-symbolic.svg");
    }

    m_textLabel->setStyleSheet("color: #FFFFFF;");
    this->setChecked(true);


}

void CustomToolButton::selectChanged()
{
    if(this->statusTip() == IS_SELECT) {
        Q_EMIT selectButtonChanged(m_textLabel->text());
        return;
    }

    Q_EMIT selectButtonChanged(m_textLabel->text());

    QList<CustomToolButton *> list = this->parent()->parent()->parent()->findChildren<CustomToolButton *>();
    for(CustomToolButton *tmp : list) {
        if(tmp->statusTip() == IS_SELECT) {
            tmp->setStatusTip("");
            tmp->defaultStyle();
        }
    }

    this->setStatusTip(IS_SELECT);
    this->defaultStyle();
}

void CustomToolButton::defaultStyle()
{
    if(this->statusTip() == IS_SELECT) {
        if(buttonListName == ALLMUSIC) {
            setLablePixmap(":/img/clicked/ukui-folder-music-symbolic.svg");
        } else if(buttonListName == FAV) {
            setLablePixmap(":/img/clicked/ukui-play-love-symbolic.svg");
        } else {
            setLablePixmap(":/img/clicked/audio-card-symbolic.svg");
        }

        m_textLabel->setStyleSheet("color: #FFFFFF;");
        this->setChecked(true);

        return;

    } else if(this->statusTip() != IS_SELECT) {
        m_textLabel->setStyleSheet("color: #000000;");
        this->setChecked(false);
        if(WidgetStyle::themeColor == 0) {
            if(buttonListName == ALLMUSIC) {
                setLablePixmap(":/img/default/ukui-folder-music-symbolic.svg");
            } else if(buttonListName == FAV) {
                setLablePixmap(":/img/default/ukui-play-love-symbolic.svg");
            } else {
                setLablePixmap(":/img/default/audio-card-symbolic.svg");
            }
        } else if(WidgetStyle::themeColor == 1) {
            if(buttonListName == ALLMUSIC) {
                setLablePixmap(":/img/dark/ukui-folder-music-symbolic.svg");
            } else if(buttonListName == FAV) {
                setLablePixmap(":/img/dark/ukui-play-love-symbolic.svg");
            } else {
                setLablePixmap(":/img/dark/audio-card-symbolic.svg");
            }
        }
    }
}

void CustomToolButton::requestCustomContextMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu();

    QAction *playAct = new QAction(this);
    QAction *pauseAct = new QAction(this);
    renameAct = new QAction(this);
    deleteAct = new QAction(this);
    playAct->setText(tr("Play"));
    pauseAct->setText(tr("Pause"));
    renameAct->setText(tr("Rename"));
    deleteAct->setText(tr("Delete"));
    menu->addAction(playAct);

    QString text = m_textLabel->text();
    QString btnText = text;

    connect(menu,&QMenu::triggered,this,[ = ](QAction * action) {
        if(action->text() == tr("Play")) {
            Q_EMIT playall(text);
        } else if(action->text() == tr("Pause")) {
        } else if(action->text() == tr("Rename")) {
            Q_EMIT renamePlayList(text);
        } else if(action->text() == tr("Delete")) {
            Q_EMIT removePlayList(text);
        }
    });

    if(btnText != tr("Song List") && btnText != tr("I Love")) {
        menu->addAction(renameAct);
        menu->addAction(deleteAct);
    }

    menu->exec(mapToGlobal(pos));

    delete menu;
    delete playAct;
    delete renameAct;
    delete deleteAct;
}

void CustomToolButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    m_isEnter = true;

    setSelectedAndHoverdStyle();

}

void CustomToolButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    m_isEnter = false;
    defaultStyle();

}

