#include "tableviewdelegate.h"
#include "UI/base/widgetstyle.h"
//#include "UIControl/player/player.h"
//#include "UIControl/base/musicDataBase.h"
//#include "UI/search/musicsearchlistview.h"
//#include "UI/tableview/tablebaseview.h"

//#include <QTextDocument>

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
    QStyleOptionViewItem opt = option;
    if(index.row() == m_hoverrow ){
        opt.state |= QStyle::State_Selected;
    } else {
        opt.state &= ~QStyle::State_MouseOver;
    }


        // 根据关键字高亮 （搜索结果高亮功能，暂未实现）
//    const TableBaseView *tableview = qobject_cast<const TableBaseView *>(option.widget);
//    QString searchText = tableview->getSearchText();
//    QString metaPtr = index.data(Qt::DisplayRole).value<QString>();
//    QString mtext;
//    mtext = metaPtr;

//    QColor textColor;
//    QColor lightColor;

//    if (WidgetStyle::themeColor == 1) {
//        textColor = QColor("#FFFFFF");
//    } else if (WidgetStyle::themeColor == 0) {
//        textColor = QColor("#000000");
//    }

//    QPalette pa = option.palette;
//    QBrush selectBrush = pa.brush(QPalette::Active, QPalette::Highlight);
//    QColor selectColor = selectBrush.color();
//    lightColor = selectColor;

//    QString playListName = playController::getInstance().getPlayListName();
//    if (playListName == SEARCH) {
//        QTextDocument document;
//        // 设置文字边距，保证绘制文字居中
//        document.setDocumentMargin(0);
//        document.setPlainText(mtext);
//        bool found = false;
//        QTextCursor highlight_cursor(&document);
//        QTextCursor cursor(&document);

//        cursor.beginEditBlock();
//        QTextCharFormat color_format(highlight_cursor.charFormat());
//        color_format.setForeground(lightColor);

//        // 搜索字体高亮
//        QTextCursor testcursor(&document);
//        testcursor.select(QTextCursor::LineUnderCursor);
//        QTextCharFormat fmt;
//        fmt.setForeground(textColor);
//        testcursor.mergeCharFormat(fmt);
//        testcursor.clearSelection();
//        testcursor.movePosition(QTextCursor::EndOfLine);

//        while (!highlight_cursor.isNull() && !highlight_cursor.atEnd()) {
//            highlight_cursor = document.find(searchText, highlight_cursor);
//            if (!highlight_cursor.isNull()) {
//                if (!found) {
//                    found = true;
//                }
//                highlight_cursor.mergeCharFormat(color_format);
//            }
//        }
//        cursor.endEditBlock();
//    }

    opt.palette.setColor(QPalette::HighlightedText,index.data(Qt::ForegroundRole).value<QColor>());
    if(WidgetStyle::themeColor == 0)
    {
        opt.palette.setColor(QPalette::Highlight,QColor(240,240,240));
    }else{
        opt.palette.setColor(QPalette::Highlight,QColor(54,54,55));
    }
    QStyledItemDelegate::paint(painter,opt,index);
}


void TableViewDelegate::onHoverIndexChanged(const QModelIndex& index)
{
    m_hoverrow = index.row();
}
void TableViewDelegate::onLeaveFromItemEvent()
{
    m_hoverrow = -1;
}
