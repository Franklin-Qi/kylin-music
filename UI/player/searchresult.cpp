#include "searchresult.h"

SearchResult::SearchResult(QWidget *parent) : QWidget(parent)
{
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);
    this->setWindowFlag(Qt::Tool);

    //窗体禁止拖动
    this->setProperty("useStyleWindowManager", false);
//    setFixedWidth(200);
    vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(32, 0, 32, 0);
    vlayout->setSpacing(0);
    this->setLayout(vlayout);
    //歌曲名
    m_MusicLabel = new QLabel(tr("Music"),this);
    m_MusicLabel->setFixedHeight(36);
//    m_MusicLabel->setContentsMargins(32, 0, 0, 0);

    m_MusicView = new MusicSearchListview(this);
    m_MusicView->setSearchType(SearchType::TitleType);
    m_MusicView->setMinimumWidth(136);
//    m_MusicView->setContentsMargins(32, 0, 0, 0);

//    m_SingerFrame = new QFrame(this);
//    m_SingerFrame->setFrameShape(QFrame::HLine);
//    m_SingerFrame->setFrameShadow(QFrame::Raised);
//    m_SingerFrame->setLineWidth(2);

    //歌手
    m_SingerLabel = new QLabel(tr("Singer"),this);
    m_SingerLabel->setFixedHeight(36);
//    m_SingerLabel->setContentsMargins(32, 0, 0, 0);

    m_SingerView = new MusicSearchListview(this);
    m_SingerView->setSearchType(SearchType::SingerType);
    m_SingerView->setMinimumWidth(136);
//    m_SingerView->setContentsMargins(32, 0, 0, 0);

//    m_AlbumFrame = new QFrame(this);
//    m_AlbumFrame->setFrameShape(QFrame::HLine);
//    m_AlbumFrame->setFrameShadow(QFrame::Raised);
//    m_AlbumFrame->setLineWidth(2);

    //专辑
    m_AlbumLabel = new QLabel(tr("Album"),this);
    m_AlbumLabel->setFixedHeight(36);
//    m_AlbumLabel->setContentsMargins(32, 0, 0, 0);

    m_AlbumView = new MusicSearchListview(this);
    m_AlbumView->setSearchType(SearchType::AlbumType);
    m_AlbumView->setMinimumWidth(136);
//    m_AlbumView->setContentsMargins(32, 0, 0, 0);

    m_MusicLabel->setStyleSheet("color:#8F9399;");
    m_SingerLabel->setStyleSheet("color:#8F9399;");
    m_AlbumLabel->setStyleSheet("color:#8F9399;");

    vlayout->addWidget(m_MusicLabel, 1);
    vlayout->addWidget(m_MusicView, 0);
    vlayout->addWidget(m_SingerFrame);
    vlayout->addWidget(m_SingerLabel, 1);
    vlayout->addWidget(m_SingerView, 0);
    vlayout->addWidget(m_AlbumFrame);
    vlayout->addWidget(m_AlbumLabel, 1);
    vlayout->addWidget(m_AlbumView, 0);
    autoResize();
}

SearchResult::~SearchResult()
{

}

void SearchResult::autoResize()
{
    if (m_searchEdit != nullptr) {
        if (m_searchEdit->text() != "") {
            this->show();
        }
    }

    int lineHeight = 0;
    int lineShowCount = 0;
    if (m_MusicView->rowCount() == 0) {
        m_MusicLabel->hide();
        m_MusicView->hide();
    } else {
        m_MusicLabel->show();
        m_MusicView->show();
        lineShowCount++;
        m_MusicView->setFixedHeight(m_MusicView->rowCount() * 36);
    }

    if (m_SingerView->rowCount() == 0) {
        m_SingerLabel->hide();
        m_SingerView->hide();
    } else {
        m_SingerLabel->show();
        m_SingerView->show();
        lineShowCount++;
        m_SingerView->setFixedHeight(m_SingerView->rowCount() * 36);
    }

    if (m_AlbumView->rowCount() == 0) {
        m_AlbumLabel->hide();
        m_AlbumView->hide();
    } else {
        m_AlbumLabel->show();
        m_AlbumView->show();
        lineShowCount++;
        m_AlbumView->setFixedHeight(m_AlbumView->rowCount() * 36);
    }

    if (lineShowCount > 0) {
        lineHeight = 2 * (lineShowCount - 1);
    }

    int total = 0;
    total += m_MusicLabel->isVisible() ? m_MusicLabel->height() : 0;
    total += m_MusicView->isVisible() ? m_MusicView->height() : 0;
//    total += s_ArtistLine->isVisible() ? s_ArtistLine->height() : 0;
    total += m_SingerLabel->isVisible() ? m_SingerLabel->height() : 0;
    total += m_SingerView->isVisible() ? m_SingerView->height() : 0;
//    total += s_AblumLine->isVisible() ? s_AblumLine->height() : 0;
    total += m_AlbumLabel->isVisible() ? m_AlbumLabel->height() : 0;
    total += m_AlbumView->isVisible() ? m_AlbumView->height() : 0;
    total += lineHeight;

    if (m_MusicView->isHidden() && m_SingerView->isHidden() && m_AlbumLabel->isHidden()) {
        setFixedHeight(total);
    } else {
        setFixedHeight(total + 28);
    }
}

void SearchResult::setListviewSearchString(const QString &str)
{
    //搜索歌曲候选:<=5个
    m_MusicView->setSearchText(str);
    m_SingerView->setSearchText(str);
    m_AlbumView->setSearchText(str);
    autoResize();
    m_Count = m_MusicView->rowCount()
              + m_AlbumView->rowCount()
              + m_SingerView->rowCount();
    m_CurrentIndex = -1;
}

void SearchResult::setLineEditSearchString(const QString &str)
{
//    if (m_searchEdit) {
//        m_searchEdit->blockSignals(true);
//        m_searchEdit->setText(str);
//        m_searchEdit->blockSignals(false);
//    }
}

void SearchResult::selectUp()
{
    if (m_Count == 0) {
        return;
    }
    if (m_CurrentIndex < 0) {
        return;
    }
    m_CurrentIndex --;
    if (m_CurrentIndex < m_MusicView->rowCount()) {
        m_MusicView->setCurrentIndexInt(m_CurrentIndex);
        m_SingerView->setCurrentIndexInt(-1);
        m_AlbumView->setCurrentIndexInt(-1);
    } else if (m_CurrentIndex >= m_MusicView->rowCount() - 1
               && m_CurrentIndex < (m_MusicView->rowCount() + m_SingerView->rowCount())) {
        m_SingerView->setCurrentIndexInt(m_CurrentIndex
                                         - m_MusicView->rowCount());
        m_MusicView->setCurrentIndexInt(-1);
        m_AlbumView->setCurrentIndexInt(-1);
    } else {
        m_AlbumView->setCurrentIndexInt(m_CurrentIndex
                                        - m_MusicView->rowCount()
                                        - m_SingerView->rowCount());
        m_MusicView->setCurrentIndexInt(-1);
        m_SingerView->setCurrentIndexInt(-1);
    }

    m_MusicView->update();
    m_SingerView->update();
    m_AlbumView->update();
}

void SearchResult::selectDown()
{
    if (m_Count == 0) {
        return;
    }
    if (m_CurrentIndex >= m_Count - 1) {
        return;
    }

    m_CurrentIndex ++;

    if (m_CurrentIndex < m_MusicView->rowCount()) {
        m_MusicView->setCurrentIndexInt(m_CurrentIndex);

        m_SingerView->setCurrentIndexInt(-1);
        m_AlbumView->setCurrentIndexInt(-1);

    } else if (m_CurrentIndex >= m_MusicView->rowCount() - 1
               && m_CurrentIndex < (m_MusicView->rowCount() + m_SingerView->rowCount())) {

        m_SingerView->setCurrentIndexInt(m_CurrentIndex
                                         - m_MusicView->rowCount());

        m_MusicView->setCurrentIndexInt(-1);
        m_AlbumView->setCurrentIndexInt(-1);

    } else {
        m_AlbumView->setCurrentIndexInt(m_CurrentIndex
                                        - m_MusicView->rowCount()
                                        - m_SingerView->rowCount());

        m_MusicView->setCurrentIndexInt(-1);
        m_SingerView->setCurrentIndexInt(-1);

        m_MusicView->clearSelection();
        m_SingerView->clearSelection();
    }

    m_MusicView->update();
    m_SingerView->update();
    m_AlbumView->update();
}

int SearchResult::getCurrentIndex()
{
    return m_CurrentIndex;
}

void SearchResult::setSearchEdit(SearchEdit *edit)
{
    m_searchEdit = edit;
}

//bool SearchResult::nativeEvent(const QByteArray &eventType, void *message, long *result)
//{
//    Q_UNUSED(result);
//    if(eventType != "xcb_generic_event_t")
//    {
//        return false;
//    }

//    xcb_generic_event_t *event = (xcb_generic_event_t*)message;
//    switch (event->response_type & ~0x80)
//    {
//        case XCB_FOCUS_OUT:
//            QRect rect(m_resultPosX, m_resultPosY, m_resultPosWidth, m_resultPosHeight);
//            if(rect.contains(QCursor::pos(), false))
//            {
//                return 0;
//            }
//            else
//            {
//                this->hide();
//                break;
//            }
//    }
//    return false;
//}

void SearchResult::changeSrearchResultPos(int posX, int posY, int width, int height)
{
    m_resultPosX = posX;
    m_resultPosY = posY;
    m_resultPosWidth = width;
    m_resultPosHeight = height;
}

void SearchResult::onReturnPressed()
{
    if (m_MusicView->getIndexInt() >= 0) {
        m_MusicView->onReturnPressed();
    } else if (m_SingerView->getIndexInt() >= 0) {
        m_SingerView->onReturnPressed();
    } else if (m_AlbumView->getIndexInt() >= 0) {
        m_AlbumView->onReturnPressed();
    }
}
