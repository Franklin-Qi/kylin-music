
#include "tablebaseview.h"

TableBaseView::TableBaseView(QTableView *parent)
{
    m_delegate = new TableViewDelegate();
    connect(this,&TableBaseView::hoverIndexChanged,m_delegate,&TableViewDelegate::onHoverIndexChanged);
    connect(this,&TableBaseView::leaveFromItem,m_delegate,&TableViewDelegate::onLeaveFromItemEvent);
    setItemDelegate(m_delegate);
    this->setMouseTracking(true);
    initStyle();
}

void TableBaseView::initStyle()
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    setSelectionMode(QAbstractItemView::ExtendedSelection);//设置按ctrl键选中多个
    setAutoFillBackground(true);
    setAlternatingRowColors(false);

    this->setStyleSheet("QTableView{border:none;}");
    this->horizontalHeader()->setVisible(false);// 水平不可见
    this->verticalHeader()->setVisible(false);// 垂直不可见
    this->setAutoScroll(true);
    this->verticalScrollBarPolicy();
    this->setAutoFillBackground(true);

}
TableBaseView::~TableBaseView()
{
    if(m_delegate!=nullptr) {
        m_delegate->deleteLater();
    }
}
void TableBaseView::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index = this->indexAt(event->pos());
    emit hoverIndexChanged(index);
}
void TableBaseView::leaveEvent(QEvent *event)
{
    emit leaveFromItem();
    viewport()->update();
}
