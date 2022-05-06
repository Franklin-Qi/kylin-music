/*
 * Copyright (C) 2021, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "searchedit.h"
#include "UI/base/xatom-helper.h"

SearchEdit::SearchEdit(QWidget *parent) : QLineEdit(parent)
{
    setFocusPolicy(Qt::ClickFocus);
    setContextMenuPolicy(Qt::DefaultContextMenu);

    setFixedSize(200, 32);
    connect(this, &SearchEdit::textChanged,this,&SearchEdit::slotTextChanged);
    connect(this, &SearchEdit::returnPressed,this,&SearchEdit::slotReturnPressed);
//    connect(this, &SearchEdit::editingFinished,this,&SearchEdit::slotEditingFinished);

    m_result = new SearchResult(m_mainWidget);
    m_result->setSearchEdit(this);
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(m_result->winId(), hints);

    moveSearchResult();
}

void SearchEdit::keyPressEvent(QKeyEvent *event)
{
    if(m_result == nullptr)
    {
        return;
    }
    if(event->key() == Qt::Key_Up)
    {
        m_result->selectUp();
    }
    if(event->key() == Qt::Key_Down)
    {
        m_result->selectDown();
    }
    QLineEdit::keyPressEvent(event);
}

void SearchEdit::slotTextChanged()
{
    if (m_result == nullptr) {
        return;
    }

    QString alltext = this->text();
    if (alltext.isEmpty()) {
        m_result->hide();
        return;
    }/* else {
        m_result->show();
        m_result->raise();
    }*/

    this->setCursorPosition(this->cursorPosition());

    auto text = this->text().trimmed();

    if (this->text().size() == 0) {
        m_result->hide();
        return;
    }
    if (text.length() >= 1) {
        m_result->setListviewSearchString(text);

        moveSearchResult();
        m_result->show();
//        m_result->activateWindow();
//        m_result->raise();
        m_result->autoResize();
//        m_result->setFocusPolicy(Qt::StrongFocus);
//        m_result->raise();
    } else {
        m_result->hide();
    }
}

void SearchEdit::slotReturnPressed()
{
    if (m_result == nullptr) {
        return;
    }

    if (m_result->getCurrentIndex() >= 0) {
        m_result->onReturnPressed();
        m_result->hide();
        return;
    }

    auto text = QString(this->text().trimmed());
    if(text.length() == 0)
        return;
    m_result->hide();
    Q_EMIT signalReturnPressed(tr("Search Result"));
    Q_EMIT signalReturnText(text);
}

void SearchEdit::setWidget(QWidget *mainWidget)
{
    m_mainWidget = mainWidget;
}

void SearchEdit::moveSearchResult()
{
    m_result->setFixedWidth(200);
    QPoint resultPos = this->mapToGlobal(this->rect().bottomLeft());
    resultPos.setX(resultPos.x());
    resultPos.setY(resultPos.y() + 12);
    QSize editSize = this->size();
    int newPosX = resultPos.x();
    int newPosY = resultPos.y() - editSize.height() - 12;
    m_result->changeSrearchResultPos(newPosX, newPosY, editSize.width(), editSize.height());
    m_result->move(resultPos);
}

