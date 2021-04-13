#include "tableviewdelegate.h"

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
        opt.state |= QStyle::State_MouseOver;
    } else {
        opt.state &= ~QStyle::State_MouseOver;
    }
    QStyledItemDelegate::paint(painter,opt,index);
}

// 响应按钮事件 - 划过、按下
bool TableViewDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    qDebug()<< "aaaaaaaaaaaaa";
}


void TableViewDelegate::onHoverIndexChanged(const QModelIndex& index)
{
    m_hoverrow = index.row();
}
void TableViewDelegate::onLeaveFromItemEvent()
{
    m_hoverrow = -1;
}
