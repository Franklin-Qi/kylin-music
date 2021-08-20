#ifndef LABEDIT_H
#define LABEDIT_H

#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

class LabEdit : public QLineEdit
{
    Q_OBJECT
public:
    LabEdit();
    void setLabelNumber(int num);
private:
    void init();
private:
    QLabel *lab = nullptr;
    int leng = 15;
};

#endif // LABEDIT_H
