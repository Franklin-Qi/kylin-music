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

    initGsettings();
    initLayout();
    setCheckable(true);
    setProperty("isImportant", true);

    this->setIconSize(QSize(16,16));
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,&CustomToolButton::clicked,this,&CustomToolButton::selectChanged);
    connect(this,&CustomToolButton::customContextMenuRequested,this,&CustomToolButton::selectChanged);
    connect(this,&CustomToolButton::customContextMenuRequested,this,&CustomToolButton::requestCustomContextMenu);

}

CustomToolButton::~CustomToolButton()
{

}

/**
 * @brief CustomToolButton::initGsettings
 * 需要自行获取hover和click的状态，参看控制面板`shell/mainwindow.cpp`
 */
void CustomToolButton::initGsettings()
{
    if (QGSettings::isSchemaInstalled("org.ukui.style")) {
        QGSettings *qtSettings = new QGSettings("org.ukui.style", QByteArray(), this);
        if (qtSettings->keys().contains("styleName")) {
            hoverColor = pluginBtnHoverColor(qtSettings->get("style-name").toString(), true);
            clickColor = pluginBtnHoverColor(qtSettings->get("style-name").toString(), false);
            if (!this->isChecked())
                this->setStyleSheet(QString("QToolButton:hover{background-color:%1;border-radius: 6px;}"
                                            "QToolButton:pressed{background-color:%2;border-radius: 6px;}").arg(hoverColor).arg(clickColor));
        }

        connect(qtSettings, &QGSettings::changed, this, [=](const QString &key) {
            if (key == "styleName") {
                hoverColor = this->pluginBtnHoverColor(qtSettings->get("style-name").toString(), true);
                clickColor = pluginBtnHoverColor(qtSettings->get("style-name").toString(), false);
                if (!this->isChecked())
                    this->setStyleSheet(QString("QToolButton:hover{background-color:%1;border-radius: 6px;}"
                                                "QToolButton:pressed{background-color:%2;border-radius: 6px;}").arg(hoverColor).arg(clickColor));
            } else if (key == "themeColor" && this->isChecked()) {
                this->toggled(true);
            }

        });
    }



}

void CustomToolButton::initLayout()
{
    this->setFixedSize(180, 36);

    m_iconLable->setFixedSize(16, 16);

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
#if 0
    if(buttonListName == ALLMUSIC) {
        setLablePixmap(":/img/clicked/ukui-folder-music-symbolic.svg");
    } else if(buttonListName == FAV) {
        setLablePixmap(":/img/clicked/ukui-play-love-symbolic.svg");
    } else {
        setLablePixmap(":/img/clicked/audio-card-symbolic.svg");
    }

    m_textLabel->setStyleSheet("color: #FFFFFF;");
#endif

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor(0, 255, 0, 30));
    this->setPalette(pal);
    setAutoFillBackground(true);

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

/**
 * @brief CustomToolButton::defaultStyle
 * 由于主题或者SDK不能实现效果，因此需要自定义hover和click的样式
 */
void CustomToolButton::defaultStyle()
{
    if(this->statusTip() == IS_SELECT) {
        this->setStyleSheet("QToolButton:checked{background-color: palette(highlight); border-radius: 6px;}");
        m_textLabel->setStyleSheet("color: white;");

        if(buttonListName == ALLMUSIC) {
            setLablePixmap(":/img/clicked/ukui-folder-music-symbolic.svg");
        } else if(buttonListName == FAV) {
            setLablePixmap(":/img/clicked/ukui-play-love-symbolic.svg");
        } else {
            setLablePixmap(":/img/clicked/audio-card-symbolic.svg");
        }


        return;

    } else if(this->statusTip() != IS_SELECT) {
        this->setStyleSheet(QString("QToolButton:hover{background-color:%1;border-radius: 6px;}"
                                    "QToolButton:pressed{background-color:%2;border-radius: 6px;}").arg(hoverColor).arg(clickColor));

        if(WidgetStyle::themeColor == 0) {
            m_textLabel->setStyleSheet("color: palette(windowText);");
            if(buttonListName == ALLMUSIC) {
                setLablePixmap(":/img/default/ukui-folder-music-symbolic.svg");
            } else if(buttonListName == FAV) {
                setLablePixmap(":/img/default/ukui-play-love-symbolic.svg");
            } else {
                setLablePixmap(":/img/default/audio-card-symbolic.svg");
            }
        } else if(WidgetStyle::themeColor == 1) {
            m_textLabel->setStyleSheet("color: white;");
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

QString CustomToolButton::pluginBtnHoverColor(QString styleName, bool hoverFlag)
{
    QColor color1 = palette().color(QPalette::Active, QPalette::Button);
    QColor color2 = palette().color(QPalette::Active, QPalette::BrightText);
    QColor color;
    qreal r,g,b,a;
    QString hoverColor;
    if (((styleName.contains("dark") || styleName.contains("black")) && hoverFlag) ||
        ((!styleName.contains("dark") && !styleName.contains("black")) && !hoverFlag)) {
        r = color1.redF() * 0.8 + color2.redF() * 0.2;
        g = color1.greenF() * 0.8 + color2.greenF() * 0.2;
        b = color1.blueF() * 0.8 + color2.blueF() * 0.2;
        a = color1.alphaF() * 0.8 + color2.alphaF() * 0.2;
    } else {
        r = color1.redF() * 0.95 + color2.redF() * 0.05;
        g = color1.greenF() * 0.95 + color2.greenF() * 0.05;
        b = color1.blueF() * 0.95 + color2.blueF() * 0.05;
        a = color1.alphaF() * 0.95 + color2.alphaF() * 0.05;
    }
    color = QColor::fromRgbF(r, g, b, a);
    hoverColor = QString("rgba(%1, %2, %3, %4)").arg(color.red())
                                                .arg(color.green())
                                                .arg(color.blue())
                                                .arg(color.alpha());
    return hoverColor;

}
