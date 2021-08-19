
#include "tablebaseview.h"
#include "UI/base/widgetstyle.h"

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
//    setAutoScroll(false);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    setAutoFillBackground(true);
    setAlternatingRowColors(false);
    this->setAutoScroll(false);
//    this->verticalScrollBarPolicy();
//    this->setAutoFillBackground(false);
    //限制应用内字体固定大小
//    QFont sizeFont;
//    sizeFont.setPixelSize(14);
//    this->setFont(sizeFont);

}

TableBaseView::~TableBaseView()
{
    if(m_delegate!=nullptr) {
        m_delegate->deleteLater();
    }
}

QString TableBaseView::getSearchText()const
{
    return m_searchText;
}

void TableBaseView::setSearchText(QString text)
{
    m_searchText = text;
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
