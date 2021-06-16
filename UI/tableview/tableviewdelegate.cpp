#include "tableviewdelegate.h"
#include "UI/base/widgetstyle.h"

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
