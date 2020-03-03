#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_mainwindow.h>
#include "field.h"
#include "new_game_form.h"
#include "help_widget_form.h"

namespace Ui {
class M_Form;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Field * A;

private:
     Ui::M_Form *ui;

     New_Game_Form *NGF;
     Help_Widget_Form * HWF;

                      //Игровое поле
     bool End;
     void Recreate();

     QTime * time;
     QTimer * timer;

public slots:
//Слоты, связанные с меню.
  void Show_New_Game_Widget();//Показывает окно начала новой игры

  void New_Game_Easy();       //Новая игра (9*9, 10 мин)
  void New_Game_Medium();     //Новая игра (9*16, 40 мин)
  void New_Game_Expert();     //Новая игра (16*30, 99 мин)
  void New_Game_Special();    //Новая игра с заданными параметрами

  void Show_Help_Widget();    //Показывает how-to-play руководство

  void Solving_Step();    //Показывает how-to-play руководство

  //Слоты, связанные с кнопкой-смайликом.
  void Restart();             //Пересоздание игры с текущими параметрами
  void Icon_Win();            //Установка победного смайлика
  void Icon_Lose();           //Установка мертвого смайлика
  void Icon_Pressed();        //Установка смайлика с открытым ртом (при удержании кнопки)
  void Icon_Released();       //Установка обычного смайлика (при отпускании кнопки)

  void time_count ();
  void Count_Change(int);
  void time_start();
};
#endif // MAINWINDOW_H
