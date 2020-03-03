#ifndef HELP_WIDGET_FORM_H
#define HELP_WIDGET_FORM_H

#include <ui_help_widget.h>

class Help_Widget_Form : public QWidget, public Ui::HForm
{
    Q_OBJECT
public:
    explicit Help_Widget_Form() {
    setupUi(this);
    this->setFixedSize(this->size());
}

signals:
   void mw_show();

private slots:
    void on_Ok_button_clicked () { this->hide(); emit mw_show(); }
};

#endif // HELP_WIDGET_FORM_H
