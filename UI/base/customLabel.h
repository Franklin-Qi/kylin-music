#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <QLabel>

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CustomLabel(QWidget *parent=0, Qt::WindowFlags f=0);
    explicit CustomLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);

    void setText(const QString &text);
    void setFullText(const QString &text);
    void setTextLimitShrink(const QString &text, int width);
    void setTextLimitExpand(const QString &text);
    QString fullText() const;
//    QString dealMessage(QString msg);
    int fontSize = 24;
    int minSize = 120;
protected:
    void paintEvent(QPaintEvent *);

private:
    void elideText();
private:
    QString m_fullText;
};
#endif // CUSTOMLABEL_H
