#include "tableviewdelegate.h"
#include "UI/base/widgetstyle.h"
#include "UIControl/player/player.h"
#include "UIControl/base/musicDataBase.h"
#include "UI/search/musicsearchlistview.h"
#include "UI/tableview/tablebaseview.h"

#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

TableViewDelegate::TableViewDelegate(QWidget *parent)
    : QStyledItemDelegate(parent),
      m_pOpenButton(new QPushButton()),
      m_pDeleteButton(new QPushButton()),
      m_nSpacing(5),
      m_nWidth(25),
      m_nHeight(20),
      m_hoverrow(-1)
{
}

TableViewDelegate::~TableViewDelegate()
{

}

// 绘制按钮
void TableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const TableBaseView *tableview = qobject_cast<const TableBaseView *>(option.widget);
    QString playListName = tableview->getSearchListName();
    if (playListName != tr("搜索结果")) {
        QStyleOptionViewItem opt = option;
        if (index.row() == m_hoverrow ) {
            opt.state |= QStyle::State_Selected;
        } else {
            opt.state &= ~QStyle::State_MouseOver;
        }

        opt.palette.setColor(QPalette::HighlightedText,index.data(Qt::ForegroundRole).value<QColor>());
        if(WidgetStyle::themeColor == 0)
        {
            opt.palette.setColor(QPalette::Highlight,QColor(240,240,240));
        }else{
            opt.palette.setColor(QPalette::Highlight,QColor(54,54,55));
        }
        QStyledItemDelegate::paint(painter,opt,index);
        return;
    }

    // 根据关键字高亮
    QString searchText = tableview->getSearchText();
    QColor textColor;
    QColor lightColor;

    if (WidgetStyle::themeColor == 1) {
        textColor = QColor("#FFFFFF");
    } else if (WidgetStyle::themeColor == 0) {
        textColor = QColor("#000000");
    }

    QPalette pa = option.palette;
    QBrush selectBrush = pa.brush(QPalette::Active, QPalette::Highlight);
    QColor selectColor = selectBrush.color();
    lightColor = selectColor;

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);

    QString metaPtr1 = index.data(Qt::DisplayRole).value<QString>();

    QString mtext;
    mtext = metaPtr1;

    QFontMetricsF fontWidth(tableview->font());
    mtext = fontWidth.elidedText(mtext, Qt::ElideRight, 136);

    QTextDocument document;
    // 设置文字边距，保证绘制文字居中
    document.setDocumentMargin(0);
    document.setPlainText(mtext);
    bool found = false;
    QTextCursor highlight_cursor(&document);
    QTextCursor cursor(&document);

    cursor.beginEditBlock();
    QTextCharFormat color_format(highlight_cursor.charFormat());
    color_format.setForeground(lightColor);

    // 搜索字体高亮
    QTextCursor testcursor(&document);
    testcursor.select(QTextCursor::LineUnderCursor);
    QTextCharFormat fmt;
    fmt.setForeground(textColor);
    testcursor.mergeCharFormat(fmt);
    testcursor.clearSelection();
    testcursor.movePosition(QTextCursor::EndOfLine);

    while (!highlight_cursor.isNull() && !highlight_cursor.atEnd()) {
        highlight_cursor = document.find(searchText, highlight_cursor);
        if (!highlight_cursor.isNull()) {
            if (!found) {
                found = true;
            }
            highlight_cursor.mergeCharFormat(color_format);
        }
    }
    cursor.endEditBlock();

    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    QStyle *pStyle = viewOption.widget ? viewOption.widget->style() : QApplication::style();
//    viewOption.text = "";
//    pStyle->drawControl(QStyle::CE_ItemViewItem, &viewOption, painter, viewOption.widget);

    QAbstractTextDocumentLayout::PaintContext paintContext;
    QRect textRect = pStyle->subElementRect(QStyle::SE_ItemViewItemText, &viewOption);
    painter->save();
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    document.documentLayout()->draw(painter, paintContext);
    painter->restore();
}


void TableViewDelegate::onHoverIndexChanged(const QModelIndex& index)
{
    m_hoverrow = index.row();
}
void TableViewDelegate::onLeaveFromItemEvent()
{
    m_hoverrow = -1;
}

QSize TableViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(690, 40);
}
