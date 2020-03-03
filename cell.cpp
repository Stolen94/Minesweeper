#include "cell.h"
#include <QDebug>
cell::cell(QWidget *parent, bool p_mined, bool p_disabled, int p_mines_around, bool p_is_revealed) :
    QPushButton(parent)
{
    mined=p_mined;
    disabled=p_disabled;
    mines_around=p_mines_around;
    is_revealed=p_is_revealed;
}

cell::~cell()
{
delete neighbours;
}
//=======================================================================================================
//=======================================================================================================
void cell::Set_Icon (QIcon A)
{
    this->setIcon(A);
    this->setIconSize(QSize(19,19));
}
//=======================================================================================================
//=======================================================================================================
void cell::mousePressEvent(QMouseEvent *a)
{
    if (a->button()==Qt::RightButton)  emit right_clicked();
    if (a->button()==Qt::LeftButton)   emit pressed();
}
//=======================================================================================================
//=======================================================================================================
void cell::mouseReleaseEvent(QMouseEvent *a)
{
     if (a->button()==Qt::LeftButton) emit clicked();
}
//=======================================================================================================
//=======================================================================================================
void cell::mouseDoubleClickEvent(QMouseEvent *a)
{
    if ((this->is_revealed==true)&&(a->button()==Qt::LeftButton)) { emit double_clicked(); }
}

