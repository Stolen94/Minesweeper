#include "field.h"
#include <QDebug>


Field::Field(QWidget * parent, int p_height, int p_width,  int p_mines) :
    QWidget(parent)
{
    cell_size=20;

    f_width=p_width;
    f_height=p_height;
    mines=p_mines;
    mines_counter=p_mines;

    mines_disarmed=0;
    first_click=1;
    revealed=0;

    Create_Field ();

    connect(this, SIGNAL(win()), parent, SLOT(Icon_Win()));
    connect(this, SIGNAL(lose()), parent, SLOT(Icon_Lose()));
    connect(this, SIGNAL(count_ch(int)), parent, SLOT(Count_Change(int)));
    connect(this, SIGNAL(start()), parent, SLOT(time_start()));

    for (int i=0; i<f_width; i++){
    for (int j=0; j<f_height; j++) {
    connect(&field[i][j], SIGNAL(pressed()), parent, SLOT(Icon_Pressed()));
    connect(&field[i][j], SIGNAL(clicked()), parent, SLOT(Icon_Released()));
    }}
}
//================================================================================================================
//================================================================================================================
Field::~Field()
{

delete signalmapper_for_L_Click;
delete signalmapper_for_R_Click;
delete signalmapper_for_D_Click;

for (int i=0; i<f_width; i++) {  delete[] field[i];  }
delete field;
}
//================================================================================================================
//================================================================================================================
int * Field::Get_Parameters()
{
    int * par=new int [3];
    par[0]=this->f_width;
    par[1]=this->f_height;
    par[2]=this->mines;
    return par;
}
//================================================================================================================
//================================================================================================================
void Field::Create_Field()
{
//Выделение памяти для ячеек.
 field=new cell * [f_width];
 for (int i=0; i<f_width; i++) { field[i]=new cell[f_height]; }

//Задание id
 int k=0;
 for (int i=0; i<f_width; i++) {
 for (int j=0; j<f_height; j++) {
        k++;
        field[i][j].id=k;
        field[i][j].x=i;
        field[i][j].y=j;
//Определение позиции ячейки.
        if (field[i][j].x==0) {field[i][j].position=1;}//left
        else if (field[i][j].y==0) {field[i][j].position=2;}//top
        else if (field[i][j].x==(f_width-1)) {field[i][j].position=3;}//right
        else if (field[i][j].y==(f_height-1)) {field[i][j].position=4;}//bottom
        else field[i][j].position=0;
 }
 }
field[0][0].position=12;//left_upper_corner
field[0][f_height-1].position=14;//left_lower_corner
field[f_width-1][0].position=23;//right_upper_corner
field[f_width-1][f_height-1].position=34;//right_lower_corner

//Определение соседних ячеек для каждой ячейки
for (int i=0; i<f_width; i++) {
for (int j=0; j<f_height; j++) {
    this->Get_Neighbours(&field[i][j]);
}
}

/* Генерация и размещение мин. Seed получается из текущего времени.
 * В случае, если количество мин меньше, чем половина общего количества ячеек, используется следующий алгоритм:
 * случайным образом берутся координаты ячейки, и если она пуста, она получает мину.
 * Иначе, цикл продлевается на одну итерацию*/
 if (mines<=f_width*f_height/2) {
 for (int m=0; m<mines; m++) {
        int k=(qrand()%(f_width-1));
        int p=(qrand()%(f_height-1));
        if (field[k][p].mined==0) { field[k][p].mined=1; }
        else m--;}}
 /*Если же количество мин больше, чем половина общего количества ячеек, то сначала все ячейки получают мины,
  * и затем, аналогично предыдущему алгоритму, случайно выбранные ячейки освобождаются от них*/
 else {
     for (int i=0; i<f_width; i++) {
     for (int j=0; j<f_height; j++) { field[i][j].mined=1; }}

     for (int m=0; m<f_width*f_height-mines; m++) {
         int k=(qrand()%(f_width-1));
         int p=(qrand()%(f_height-1));
         if (field[k][p].mined==1) { field[k][p].mined=0; }
         else m--;}
}
//Задание размеров виджета на основе размера ячейки и координат
 this->resize(f_width*cell_size, f_height*cell_size);

//Задание параметров ячеек и подключение ячеек к слотам.
//signalMapper-ы нужны, чтобы понять, какая именно ячейка отправила сигнал.
 signalmapper_for_L_Click = new QSignalMapper(this);
 signalmapper_for_R_Click = new QSignalMapper(this);
 signalmapper_for_D_Click = new QSignalMapper(this);

 for (int i=0; i< f_width; i++){
    for (int j=0; j<f_height; j++) {
      field[i][j].setParent(this);
      field[i][j].setGeometry(i*cell_size, j*cell_size, cell_size, cell_size);
      field[i][j].setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      field[i][j].setStyleSheet("background-color: lightGrey; border-style: inset; border-width: 0.5px; border-color: grey;");
      field[i][j].show();

      connect(&field[i][j], SIGNAL(clicked()), signalmapper_for_L_Click, SLOT (map()));
      signalmapper_for_L_Click->setMapping(&field[i][j], field[i][j].id);

      connect(&field[i][j], SIGNAL(double_clicked()), signalmapper_for_D_Click, SLOT (map()));
      signalmapper_for_D_Click->setMapping(&field[i][j], field[i][j].id);

      connect(&field[i][j], SIGNAL(right_clicked()), signalmapper_for_R_Click, SLOT (map()));
      signalmapper_for_R_Click->setMapping(&field[i][j], field[i][j].id);
  }
}

 connect(signalmapper_for_L_Click, SIGNAL(mapped(int)), this, SLOT(click_on_cell(int)));
 connect(signalmapper_for_R_Click, SIGNAL(mapped(int)), this, SLOT(click_disable(int)));
 connect(signalmapper_for_D_Click, SIGNAL(mapped(int)), this, SLOT(click_reveal(int)));
//Сканирование поля-----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
 Scan_Field();
//Вспомогательный блок для отладки. Позволяет отображать требуемые параметры на ячейках---------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
/* QString A;
 QString B;

 for (int i=0; i<p_width; i++) {
 for (int j=0; j<p_height; j++) {
     A.setNum(field[i][j].x);
     B.setNum(field[i][j].y);
    // field[i][j].setText(A + B);
  }
 }*/

 }
//================================================================================================================
//Ячейки "получают" цифры, отражающие количество мин в соседних ячейках===========================================
void Field::Scan_Field()
{
    for (int i=0; i<f_width; i++){
       for (int j=0; j<f_height; j++) {
           if (field[i][j].mined==true) {
               for (int k=0; k<field[i][j].neighbours_num; k++) { field[i][j].neighbours[k]->mines_around+=1;}
      }
    }
  }
}
//================================================================================================================
//================================================================================================================
void Field::click_on_cell(int p_id)
{
//Увеличение счетчика открытых ячеек на 1.
    revealed+=1;
//Если первый клик, то запустить таймер.
    if (first_click==1) {emit start();}

    cell * A=Cell_By_Id(p_id);
    A->is_revealed=true;

//Случай попадания на мину (не первый клик). Устанавливаются иконки мин, все сигналы для ячеек отключаются. Испускается сигнал для остановки таймера и смены смайлика

       if ((A->mined==true)&&(first_click==0)) {
           A->setStyleSheet("background-color: red; border-style: inset; border-width: 0.5px; border-color: grey;");
           A->Set_Icon(QIcon(":/res/mine_triggered.ico"));
           qDebug()<<"Cell "<<A->id<<" has triggered the mine. BOOM"<<" Is revealed: "<<A->is_revealed<<", is disabled: "<<A->disabled;

           for (int i=0; i<f_width; i++) {
           for (int j=0; j<f_height; j++){
                 field[i][j].blockSignals(true);
                 if ((field[i][j].mined==true)&&(field[i][j].disabled==false)&&(field[i][j].id!=p_id)) { field[i][j].Set_Icon(QIcon(":/res/mine.ico")); }
                 if ((field[i][j].mined==false)&&(field[i][j].disabled==true)) { field[i][j].Set_Icon(QIcon(":/res/mine_false.ico")); }
                }}
               emit lose();

           }
//Случай попадания на мину (первый клик). Мина переносится в случайно выбранную свободную ячейку.
     else {if ((A->mined==true)&&(first_click==1)) { First_Click_Mined(p_id);}}
//Ячейка не содержит мину. Задается стиль в соответствии с количеством мин в соседних ячейках.
       if (A->mined==false) {
       if (A->mines_around==0) A->setStyleSheet("border-style: inset; border-width: 0.5px; border-color: grey;");
       if (A->mines_around==1) A->setStyleSheet("font: bold 10px; color: blue; border-style: inset; border-width: 0.5px; border-color: grey;");
       if (A->mines_around==2) A->setStyleSheet("font: bold 10px; color: green; border-style: inset; border-width: 0.5px; border-color: grey;");
       if (A->mines_around==3) A->setStyleSheet("font: bold 10px; color: red; border-style: inset; border-width: 0.5px; border-color: grey;");
       if (A->mines_around==4) A->setStyleSheet("font: bold 10px; color: darkYellow; border-style: inset; border-width: 0.5px; border-color: grey;");
       if (A->mines_around==5) A->setStyleSheet("font: bold 10px; color: darkRed; border-style: inset; border-width: 0.5px; border-color: grey;");
       if (A->mines_around==6) A->setStyleSheet("font: bold 10px; color: darkGrey; border-style: inset; border-width: 0.5 px; border-color: grey;");
       if (A->mines_around==7) A->setStyleSheet("font: bold 10px; color: darkBlue; border-style: inset; border-width: 0.5 px; border-color: grey;");
       if (A->mines_around==8) A->setStyleSheet("font: bold 10px; color: darkCyan; border-style: inset; border-width: 0.5px; border-color: grey;");
       if (A->mines_around==9) A->setStyleSheet("font: bold 10px; color: darkMagenta; border-style: inset; border-width: 0.5px; border-color: grey;");

//Отображается количество мин в соседних ячейках (и пустое место для нулевых ячеек).
       QString B;
       if (A->mines_around==0) B=QString(""); else B.setNum(A->mines_around);
       A->setText(B);
//Ячейка отключается от одиночных кликов левой и правой кнопок мыши
       disconnect(A, SIGNAL(clicked()), signalmapper_for_L_Click, SLOT (map()));
       disconnect(A, SIGNAL(right_clicked()), signalmapper_for_R_Click, SLOT (map()));
//Если рядом нет мин, то открываются соседние ячейки.
       if (A->mines_around==0) Click_Zero_Cell(p_id);
       }

//Первый клик устанавливается в 0.
if (first_click==1) first_click=0;

//Если открыты все ячейки, кроме заминированных, то засчитывается победа.
if (revealed==f_width*f_height-mines) {
       for (int i=0; i<f_width; i++) {
            for (int j=0; j<f_height; j++){
               if ((field[i][j].mined==true)&&(field[i][j].disabled==false)) {
                     field[i][j].right_clicked();
                     field[i][j].blockSignals(true);}

}}
       emit win();
}
}
//================================================================================================================
//================================================================================================================
void Field::click_disable(int p_id)
{

    cell * A=Cell_By_Id(p_id);

    if (A->disabled==false) {
        disconnect(A, SIGNAL(clicked()), signalmapper_for_L_Click, SLOT (map()));
        A->disabled=true;
        A->is_revealed=true;
        A->Set_Icon(QIcon(":/res/flag.ico"));
        if (A->mined==true) { mines_disarmed+=1;}
        Count_Change(true);
 }
    else {
        connect(A, SIGNAL(clicked()), signalmapper_for_L_Click, SLOT (map()));
        A->disabled=false;
        A->is_revealed=false;
        A->Set_Icon(QIcon(""));
        if (A->mined==true) mines_disarmed-=1;
        Count_Change(false);
 }


if ((mines_disarmed==mines)&&(mines_counter==0)) {
//Открывает поле и блокирует сигналы, если все мины реально обезврежены
   for (int i=0; i<f_width; i++) {
   for (int j=0; j<f_height; j++){
    if (field[i][j].mined==true) field[i][j].blockSignals(true);
       else { field[i][j].clicked(); field[i][j].blockSignals(true); }
}}
emit win();
}
}
//================================================================================================================
//================================================================================================================
void Field::Count_Change(bool A)
{
  if (A) { mines_counter-=1; emit count_ch(mines_counter); }
  else { mines_counter+=1; emit count_ch(mines_counter); }
}
//================================================================================================================
//================================================================================================================
void Field::Click_Zero_Cell(int p_id)
{
  cell * A=Cell_By_Id(p_id);
  for (int k=0; k<A->neighbours_num; k++) { A->neighbours[k]->clicked(); }
}
//================================================================================================================
//================================================================================================================
void  Field::First_Click_Mined(int p_id)
{
//Поиск пустой ячейки и ее минирование.
bool complete=0;

for (int i=0; i<f_width; i++) {
  if (complete==1) break;
for (int j=0; j<f_height; j++) {
  if (complete==1) break;
  if (field[i][j].mined==0) {

   field[i][j].mined=1;
   complete=1;

   for (int k=0; k<field[i][j].neighbours_num; k++) { field[i][j].neighbours[k]->mines_around+=1; }
      }
   }
}
//Разминирование исходной ячейки.
cell * A=Cell_By_Id(p_id);
A->mined=false;
for (int k=0; k<A->neighbours_num; k++) { A->neighbours[k]->mines_around-=1; }
}
//================================================================================================================
//================================================================================================================
void Field::click_reveal(int p_id) {

    unsigned int check=0;

    cell * A=Cell_By_Id(p_id);

    for (int k=0; k<A->neighbours_num; k++) {
    if ((A->neighbours[k]->mined==true)&&(A->neighbours[k]->disabled==true)) { check+=1;}
 }

    if (A->mines_around==check) {
        for (int k=0; k<A->neighbours_num; k++) {
        if (A->neighbours[k]->mined==false) A->neighbours[k]->clicked();
    }
  }

}
//================================================================================================================
//================================================================================================================
void Field::Get_Neighbours(cell *C) {

switch (C->position) {

 case 0: {
          C->neighbours_num=8;
          C->neighbours=new  cell * [8];

          C->neighbours[0]=&field[C->x-1][C->y-1];
          C->neighbours[1]=&field[C->x-1][C->y];
          C->neighbours[2]=&field[C->x-1][C->y+1];
          C->neighbours[3]=&field[C->x][C->y-1];
          C->neighbours[4]=&field[C->x][C->y+1];
          C->neighbours[5]=&field[C->x+1][C->y-1];
          C->neighbours[6]=&field[C->x+1][C->y];
          C->neighbours[7]=&field[C->x+1][C->y+1];
          break;
}

case 2: {//TOP_EDGE
          C->neighbours_num=5;
          C->neighbours=new cell * [5];

          C->neighbours[0]=&field[C->x-1][C->y];
          C->neighbours[1]=&field[C->x-1][C->y+1];
          C->neighbours[2]=&field[C->x][C->y+1];
          C->neighbours[3]=&field[C->x+1][C->y+1];
          C->neighbours[4]=&field[C->x+1][C->y];
          break;
}
 case 1:{ //LEFT_EDGE
          C->neighbours_num=5;
          C->neighbours=new cell * [5];
         //qDebug()<<"Position read as" <<1;

          C->neighbours[0]=&field[C->x][C->y-1];
          C->neighbours[1]=&field[C->x+1][C->y-1];
          C->neighbours[2]=&field[C->x+1][C->y];
          C->neighbours[3]=&field[C->x+1][C->y+1];
          C->neighbours[4]=&field[C->x][C->y+1];
          break;
}
 case 3: {//RIGHT_EDGE
          C->neighbours_num=5;
          C->neighbours=new cell * [5];
//           qDebug()<<"Position read as" <<3;

          C->neighbours[0]=&field[C->x][C->y-1];
          C->neighbours[1]=&field[C->x-1][C->y-1];
          C->neighbours[2]=&field[C->x-1][C->y];
          C->neighbours[3]=&field[C->x-1][C->y+1];
          C->neighbours[4]=&field[C->x][C->y+1];
          break;
 }
 case 4: {//BOTTOM_EDGE
          C->neighbours_num=5;
          C->neighbours=new cell * [5];

          C->neighbours[0]=&field[C->x-1][C->y];
          C->neighbours[1]=&field[C->x-1][C->y-1];
          C->neighbours[2]=&field[C->x][C->y-1];
          C->neighbours[3]=&field[C->x+1][C->y-1];
          C->neighbours[4]=&field[C->x+1][C->y];
          break;
 }

case 12:  {//LEFT_UPPER_CORNER
     C->neighbours_num=3;
     C->neighbours=new cell * [3];
//      qDebug()<<"Position read as" <<12;

     C->neighbours[0]=&field[C->x][C->y+1];
     C->neighbours[1]=&field[C->x+1][C->y+1];
     C->neighbours[2]=&field[C->x+1][C->y];
     break;}

 case 14:  {//LEFT_LOWER_CORNER

      C->neighbours_num=3;
      C->neighbours=new cell * [3];

      C->neighbours[0]=&field[C->x][C->y-1];
      C->neighbours[1]=&field[C->x+1][C->y-1];
      C->neighbours[2]=&field[C->x+1][C->y];
      break;}
 case 23: { //RIGHT_UPPER_CORNER
      C->neighbours_num=3;
      C->neighbours=new cell * [3];

      C->neighbours[0]=&field[C->x-1][C->y];
      C->neighbours[1]=&field[C->x-1][C->y+1];
      C->neighbours[2]=&field[C->x][C->y+1];
      break;}
 case 34: { //RIGHT_LOWER_CORNER
      C->neighbours_num=3;
      C->neighbours=new cell * [3];

      C->neighbours[0]=&field[C->x][C->y-1];
      C->neighbours[1]=&field[C->x-1][C->y-1];
      C->neighbours[2]=&field[C->x-1][C->y];
      break;}

}
}

cell * Field::Cell_By_Id(int p_id) {
    for (int i=0; i<f_width; i++) {
    for (int j=0; j<f_height; j++){
    if (field[i][j].id==p_id) { return &field[i][j]; }
    }
  }
}

//================================================================================================
//================================================================================================
void Field::Self_Solving()
{
if (Disabling()) { Revealing(); Revealing(); Revealing(); } else Random();
}

void Field::Revealing() {
    for (int i=0; i<f_width; i++) {
        for (int j=0; j<f_height; j++) {
    if ((field[i][j].mined==0)&&(field[i][j].is_revealed)) {click_reveal(field[i][j].id);}
        }
    }
}

bool Field::Disabling() {
    int result=false;

    for (int i=0; i<f_width; i++) {
         for (int j=0; j<f_height; j++) {
             int Neighbour_rev=0;
          if ( (field[i][j].is_revealed)&&(!field[i][j].disabled)) {

             //Определение числа "открытых" соседей для ячейки
           for (int k=0; k<field[i][j].neighbours_num; k++) {
                 if (field[i][j].neighbours[k]->is_revealed) Neighbour_rev+=1;
                 if (field[i][j].neighbours[k]->disabled) Neighbour_rev-=1;
             }


             if ( field[i][j].mines_around == (field[i][j].neighbours_num - Neighbour_rev) ) {
                 for (int k=0; k<field[i][j].neighbours_num; k++) {
                     if ( (field[i][j].neighbours[k]->mined)&&(field[i][j].neighbours[k]->disabled==0)) {
                         click_disable(field[i][j].neighbours[k]->id);
                         result=true;

                     if ((!field[i][j].neighbours[k]->mined)&&(field[i][j].neighbours[k]->is_revealed==1)) {
                         click_reveal(field[i][j].neighbours[k]->id);
                    }

                     }
                 }
             }


        }
    }
    }

    return result;
}

void Field::Random() {
    int p_id=(qrand()%(f_width*f_height-1))+1;

    cell * A=Cell_By_Id(p_id);
    if ((A->is_revealed==0)) A->clicked(); else (Random());
}
