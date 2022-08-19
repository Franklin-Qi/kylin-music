#ifndef CUSTOMTREEVIEW_H
#define CUSTOMTREEVIEW_H

#include <QGSettings>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItemModel>
#include <QListView>
#include <QTreeView>

#include "UIControl/tableview/musiclistmodel.h"
#include "UIControl/base/musicDataBase.h"
#include "customTreeViewDelegate.h"



class CustomTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit CustomTreeView(bool isHistory = false, QWidget *parent = nullptr);
    ~CustomTreeView();

    MusicListModel *m_model = nullptr; // 设置表头

    void initStyle();
    void setIsHistoryView(bool isHistory);
    void initGsettings();

    void setSearchText(QString text);
    void setSearchListName(QString listName);

    QString pluginBtnHoverColor(QString styleName, bool hoverFlag);
    QString getSearchText()const;
    QString getSearchListName()const;
    QString getNowPlayListName()const;

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void setNowPlayListName(QString listName);

Q_SIGNALS:
    void hoverIndexChanged(QModelIndex index);
    void hoverColorChanged(QString hoverColor);
    void doubleClickedSignal(QModelIndex index);
    void leaveFromItem();


private:
    bool isHistory = false;

    QGSettings *qtSettings = nullptr;
    QString hoverColor;
    QString clickColor;

    CustomTreeViewDelegate *m_delegate = nullptr; // 绘制歌曲列表样式
    QString m_searchText;
    QString m_searchListName;
    QString m_nowListName = "";

};

#endif // CUSTOMTREEVIEW_H
