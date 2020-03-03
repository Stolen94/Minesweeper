#ifndef FIELD_H
#define FIELD_H

#include <QSignalMapper>
#include <QTimer>
#include <QTime>
#include "cell.h"

class Field : public QWidget
{
    Q_OBJECT
public:
    explicit Field(QWidget *parent = 0, int w=9, int h=9, int m=10);
    ~Field();

    int * Get_Parameters();
//Группа функций, реализующих алгоритм "саморазгадывания". Возможно, имеет смысл прописать более умный "случайный клик")
    void Self_Solving();
    void Revealing();
    bool Disabling();
    void Random();
//======================================================================================================================

private:
    int cell_size;   //Размер ячеек.

    int f_width;       //Ширина поля (в ячейках)
    int f_height;      //Высота поля (в ячейках)
    int mines;       //Количество мин

    cell **field;    //Массив ячеек.

    bool first_click;//Первый клик (нужен для функций, исключающих взрыв при первом клике).
    int mines_counter; //Переменная, хранящая текущее значение счетчика мин.
    int mines_disarmed;  //Переменная, хранящая количество реально обезвреженных мин.
    int revealed;    //Количество открытых ячеек.

    QSignalMapper *signalmapper_for_L_Click;   //SignalMapper для сигналов левой кнопкой мыши
    QSignalMapper *signalmapper_for_R_Click;   //SignalMapper для сигналов правой кнопкой мыши
    QSignalMapper *signalmapper_for_D_Click;

    void Create_Field();
    void Scan_Field();

    cell * Cell_By_Id(int);
    void Get_Neighbours(cell * );

    void Click_Zero_Cell(int);
    void First_Click_Mined(int);

    void Count_Change(bool);

signals:
    void win();
    void lose();
    void start();
    void count_ch(int);

public slots:
    void click_on_cell(int);
    void click_disable(int);
    void click_reveal(int);
};

#endif // FIELD_H
