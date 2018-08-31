#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

void About::on_OKButton_clicked() {
    this -> close();
}

void About::on_CancelButton_clicked() {
    this -> close();
}
