#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::M_Form)
{
    ui->setupUi(this);

 //   End=false;

//Создание формы начала новой игры, установка соединений с соотв. слотами главной формы.
    NGF=new New_Game_Form();
    HWF=new Help_Widget_Form();
    connect(NGF, SIGNAL(start_b()), this, SLOT(New_Game_Easy()));
    connect(NGF, SIGNAL(start_m()), this, SLOT(New_Game_Medium()));
    connect(NGF, SIGNAL(start_e()), this, SLOT(New_Game_Expert()));
    connect(NGF, SIGNAL(start_s()), this, SLOT(New_Game_Special()));
    connect(NGF, SIGNAL(mw_show()), this, SLOT(show()));
    connect(HWF, SIGNAL(mw_show()), this, SLOT(show()));

//Настройка внешнего вида счетчиков.
    ui->Time_C_Wid->autoFillBackground();
    ui->Time_C_Wid->setStyleSheet("background-color: black; color: red; border-color: black");
    ui->Mine_C_Wid->autoFillBackground();
    ui->Mine_C_Wid->setStyleSheet("background-color: black; color: red; border-color: black");

//Создание панели инструментов (меню) и установка соединений с соотв. слотами
    ui->toolBar->addAction("New Game", this, SLOT(Show_New_Game_Widget()));
    ui->toolBar->addAction("How to play", this, SLOT(Show_Help_Widget()));
    ui->toolBar->addAction("AutoSolve", this, SLOT(Solving_Step()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_count()));
    time=new QTime(0,0);

//Настройка кнопки смайлика.
    connect(ui->restart_smile, SIGNAL(clicked()), this, SLOT(Restart()));

//Cоздание поля новой игры (по умолчанию - beginner level), и изменение размеров главного окна и его элементов
//в соотв. с размерами поля.

    A=new Field(this,9,9,10);

    this->Recreate();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete NGF;
    delete HWF;

    delete A;
    delete time;
    delete timer;
}

void MainWindow::Recreate() //Изменение размеров главного окна в соответствии с размерами поля. Смена смайлика.
{
    End=false;

    int * par=A->Get_Parameters();
    ui->restart_smile->setIcon(QIcon(":/res/smile.ico"));
    ui->restart_smile->setIconSize(QSize(39,39));

    ui->Mine_C_Wid->display(par[2]);
    timer->stop();
    ui->Time_C_Wid->display(0);

    A->setGeometry(0,ui->horizontalLayoutWidget->height()+30,A->width(),A->height());
    this->setFixedSize(QSize(A->width(), A->height()+ ui->horizontalLayoutWidget->height() + 30));
    ui->horizontalLayoutWidget->resize(QSize(A->width(),ui->horizontalLayoutWidget->height()));
    A->show();
}
//Слоты, связанные с новой игрой===========================================================================
//=========================================================================================================
 void MainWindow::New_Game_Easy() //Слот начала новой игры (легкая)
{
     this->show();

     delete A;
     A=new Field(this,9,9,10);
     this->Recreate();
}
//=========================================================================================================
 void MainWindow::New_Game_Medium() //Слот начала новой игры (средняя)
{
     this->show();

     delete A;
     A=new Field(this,16,16,40);
     this->Recreate();
}
//=========================================================================================================
 void MainWindow::New_Game_Expert() //Слот начала новой игры (сложная)
{
     this->show();

     delete A;
     A=new Field(this,16,30,90);
     this->Recreate();
}
//=========================================================================================================
 void MainWindow::New_Game_Special() //Слот начала новой игры (спец. параметры)
{
     this->show();

/*Чтение параметров из формы новой игры.
Здесь par[0] - длина, par[1] - высота, par[2] - количество мин
Если число мин превышает количество ячеек n, число мин устанавливается в n-1*/

     int * par=new int [3];
     par=NGF->Read_Parameters();
     if (par[2]>=par[0]*par[1]) par[2]=par[0]*par[1]-1;

     delete A;
     A=new Field(this, par[1], par[0], par[2]);
     this->Recreate();

}
//=========================================================================================================
void MainWindow::Restart() //Слот начала новой игры (с текущими параметрами)
{
     int *par=A->Get_Parameters();
     delete A;
     A=new Field(this, par[1], par[0], par[2]);
     this->Recreate();
}
//Слоты, связанные со смайликом============================================================================
//=========================================================================================================
void MainWindow::Icon_Win()//Установка победного смайлика
{
     End=true;
     timer->stop();
     ui->restart_smile->setIcon(QIcon(":/res/smile_cool.ico"));
     ui->restart_smile->setIconSize(QSize(39,39));
}
//=========================================================================================================
void MainWindow::Icon_Lose()//Установка мертвого смайлика
{
     End=true;
     timer->stop();
     ui->restart_smile->setIcon(QIcon(":/res/smile_dead.ico"));
     ui->restart_smile->setIconSize(QSize(39,39));

     ui->toolBar->blockSignals(true);
}
//=========================================================================================================
void MainWindow::Icon_Pressed()//Установка о-смайлика (нажатие кнопки)
{
    ui->restart_smile->setIcon(QIcon(":/res/smile_o.ico"));
    ui->restart_smile->setIconSize(QSize(39,39));
}
//=========================================================================================================
void MainWindow::Icon_Released()//Установка обычного смайлика (в случае, если нажатие не завершило игру.
{                               //Т.е. в случае победы или смерти слот не запустится)
    if (End==false) {
    ui->restart_smile->setIcon(QIcon(":/res/smile.ico"));
    ui->restart_smile->setIconSize(QSize(39,39));}
}
//Слоты, связанные с кнопками Toolbar======================================================================
//=========================================================================================================
void MainWindow::Show_New_Game_Widget() { NGF->show(); this->hide(); }
//=========================================================================================================
void MainWindow::Show_Help_Widget() { this->hide(); HWF->show(); }
//Слоты, связанные со счетчиками===========================================================================
//=========================================================================================================
void MainWindow::time_count()
{
       ui->Time_C_Wid->display(time->elapsed()/1000);
       if ((time->elapsed()/1000 + 1) == 1000) timer->stop();
}
//=========================================================================================================
void MainWindow::Count_Change(int M)
{
ui->Mine_C_Wid->display(M);
}

void MainWindow::time_start() {
    timer->start(1);
    time->start();
}

void MainWindow::Solving_Step(){
A->Self_Solving();
}
