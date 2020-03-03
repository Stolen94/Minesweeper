#ifndef CELL_H
#define CELL_H

#include <QPushButton>
#include <QMouseEvent>

class cell : public QPushButton
{
    Q_OBJECT

friend class Field;

public:

explicit cell (QWidget *parent = 0, bool mined=0, bool is_disabled=0, int mines_around=0, bool is_revealed=0);
~cell();

void Set_Icon (QIcon);//Установка иконки для кнопки

private:

//Массив указателей, содержащий адреса соседних ячеек, и его размер.
cell ** neighbours;
int neighbours_num;

int id;//Нужен для привязки сигналов.
int position;//Описывает положение ячейки на поле и количество соседних ячеек.

bool mined;//Наличие мины в ячейке
bool disabled;//Стоит ли флаг в ячейке
unsigned int mines_around;//Описывает количество мин в соседних ячейках

bool is_revealed;//Открыта ячейка или нет

//Координаты ячейки на поле. Нужны для определения позиции.
int x;
int y;

private slots:
    void mousePressEvent(QMouseEvent *a);               //Определяет слот для нажатия на кнопку.
    void mouseReleaseEvent(QMouseEvent *a);             //Определяет слот для отпускания кнопки.
    void mouseDoubleClickEvent(QMouseEvent *a);         //Определяет слот для двойного нажатия на кнопку.

signals:
    void right_clicked();
    void double_clicked();
};

#endif // CELL_H
