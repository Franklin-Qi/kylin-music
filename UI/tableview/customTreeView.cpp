
#include "customTreeView.h"
#include "UI/base/widgetstyle.h"

CustomTreeView::CustomTreeView(bool isHistory, QWidget *parent)
{
    this->setMouseTracking(true);

    setIsHistoryView(isHistory);

    initGsettings();
    initStyle();

    header()->setStretchLastSection(true); // 拉伸最后一列
    header()->setSectionsMovable(false); // 禁止移动列

    setRootIsDecorated(false); // 取消第一列箭头,否则会出现第一列和标题不对齐的情况
    setIconSize(QSize(0,0)); // 取消第2列-第n列的图标，也是为了标题不对齐问题
    setAlternatingRowColors(false); // 奇偶行颜色变化

    connect(this,&CustomTreeView::hoverIndexChanged,m_delegate,&CustomTreeViewDelegate::onHoverIndexChanged);
    connect(this,&CustomTreeView::hoverColorChanged,m_delegate,&CustomTreeViewDelegate::onHoverColorChanged);
    connect(this,&CustomTreeView::leaveFromItem,m_delegate,&CustomTreeViewDelegate::onLeaveFromItemEvent);
}

void CustomTreeView::initStyle()
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    setSelectionMode(QAbstractItemView::ExtendedSelection);//设置按ctrl键选中多个
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setAlternatingRowColors(false);
    this->setAutoScroll(false);

}

void CustomTreeView::setIsHistoryView(bool isHistory)
{
    isHistory = isHistory;

    m_delegate = new CustomTreeViewDelegate();
    m_delegate->setIsHistory(isHistory);
    setItemDelegate(m_delegate);
}

void CustomTreeView::initGsettings()
{
    if (QGSettings::isSchemaInstalled("org.ukui.style")) {
        if (qtSettings == nullptr) {
            qtSettings = new QGSettings("org.ukui.style", QByteArray(), this);
        }
        if (qtSettings->keys().contains("styleName")) {
            hoverColor = pluginBtnHoverColor(qtSettings->get("style-name").toString(), true);
            clickColor = pluginBtnHoverColor(qtSettings->get("style-name").toString(), false);
        }

        connect(qtSettings, &QGSettings::changed, this, [=](const QString &key) {
            if (key == "styleName") {
                hoverColor = this->pluginBtnHoverColor(qtSettings->get("style-name").toString(), true);
                clickColor = pluginBtnHoverColor(qtSettings->get("style-name").toString(), false);
            } else if (key == "themeColor") {
                // todo : selected
            }

        });
    }


}

QString CustomTreeView::pluginBtnHoverColor(QString styleName, bool hoverFlag)
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



CustomTreeView::~CustomTreeView()
{
    if(m_delegate!=nullptr) {
        m_delegate->deleteLater();
    }
}

QString CustomTreeView::getSearchText()const
{
    return m_searchText;
}

void CustomTreeView::setSearchText(QString text)
{
    m_searchText = text;
}

QString CustomTreeView::getSearchListName()const
{
    return m_searchListName;
}

void CustomTreeView::setSearchListName(QString listName)
{
    m_searchListName = listName;
}

void CustomTreeView::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index = this->indexAt(event->pos());
    Q_EMIT hoverIndexChanged(index);

    Q_EMIT hoverColorChanged(hoverColor);

}

void CustomTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = this->indexAt(event->pos());
    Q_EMIT doubleClicked(index);

    QWidget::mouseDoubleClickEvent(event);

}
void CustomTreeView::leaveEvent(QEvent *event)
{
    Q_EMIT leaveFromItem();
    viewport()->update();
}

QString CustomTreeView::getNowPlayListName()const
{
    return m_nowListName;
}

void CustomTreeView::setNowPlayListName(QString listName)
{
    m_nowListName = listName;
}
