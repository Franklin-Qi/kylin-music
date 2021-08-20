#include "labedit.h"

LabEdit::LabEdit()
{
    init();
}

void LabEdit::init()
{
    QHBoxLayout* mainLayout = new QHBoxLayout();
    lab = new QLabel(this);
    lab->setText(QString("%1").arg(leng));
    mainLayout->addWidget(lab);
    mainLayout->setAlignment(Qt::AlignRight);
    this->setLayout(mainLayout);
}

void LabEdit::setLabelNumber(int num)
{
    lab->setText(QString("%1").arg(num));

    if (num == 0) {
        lab->setStyleSheet("QLabel{color:#F44E50;}");
    } else {
        lab->setStyleSheet("QLabel{color:#000000;}");
    }
}
