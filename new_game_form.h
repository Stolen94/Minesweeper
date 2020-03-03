#ifndef NEW_GAME_FORM_H
#define NEW_GAME_FORM_H

#include <ui_new_game.h>

class New_Game_Form : public QWidget, public Ui::MyForm
{
    Q_OBJECT
public:

New_Game_Form (QWidget * parent = 0) : QWidget(parent)
{
        setupUi(this);
        this->setFixedSize(this->size());
}

int * Read_Parameters ()
{
    int * par=new int[3];
    par[0]=input_width->value();
    par[1]=input_height->value();
    par[2]=input_mines->value();
    return par;
}

signals:
    void start_b();
    void start_m();
    void start_e();
    void start_s();
    void mw_show();

private slots:

    void on_Cancel_Button_clicked () { this->hide(); emit mw_show(); }

    void on_Start_button_clicked ()
{
    if (Beginner_button->isChecked()) { this->hide(); emit start_b();  }
    if (Medium_button->isChecked()) { this->hide(); emit start_m();  }
    if (Expert_button->isChecked()) { this->hide(); emit start_e();  }
    if (Special_button->isChecked()) { this->hide(); emit start_s(); }
}
};

#endif // NEW_GAME_FORM_H
