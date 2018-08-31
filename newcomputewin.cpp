# include "computemain.h"
# include "newcomputewin.h"
# include "ui_newcomputewin.h"

# include <iostream>

# include <QMessageBox>

NewComputeWin::NewComputeWin(QWidget *parent, bool isSA, double tv) :
    QDialog(parent),
    ui(new Ui::NewComputeWin) {
    sa = isSA;
    targetValue = tv;
    ui->setupUi(this);
}

NewComputeWin::~NewComputeWin() {
    delete ui;
}

# define inRange(a, b, c) ((a) <= (b) && (b) <= (c))

bool NewComputeWin:: checkInput() {
    QString rows = ui -> rowLine -> text();
    QString inp1 = ui -> inp1 -> text();
    QString inp2 = ui -> inp2 -> text();
    QString out1 = ui -> out1 -> text();
    QString out2 = ui -> out2 -> text();
    QString out3 = ui -> out3 -> text();

    bool ok = false;

    numRow = rows.toInt(&ok); if(!ok) return false;
    numIn1 = inp1.toInt(&ok); if(!ok) return false;
    numIn2 = inp2.toInt(&ok); if(!ok) return false;
    numOu1 = out1.toInt(&ok); if(!ok) return false;
    numOu2 = out2.toInt(&ok); if(!ok) return false;
    numOu3 = out3.toInt(&ok); if(!ok) return false;

    if(!inRange(5, numRow, 8) || !inRange(5, numRow, 8)) return false;
    if(!inRange(0, numIn1, numRow - 1) || !inRange(0, numIn2, numRow - 1)) return false;
    if(!inRange(0, numOu1, numRow - 1) || !inRange(0, numOu2, numRow - 1) || !inRange(1, numOu3, numRow - 1)) return false;

    if(numIn1 == numIn2) return false;
    if(numOu1 == numOu2 || numOu1 == numOu3 || numOu2 == numOu3) return false;

    return true;
}

void NewComputeWin::on_OKButton_clicked() {
    if(!checkInput()) {
        QMessageBox:: information(NULL, tr("ERROR"), "Something must be wrong. Please check your input.");
    }
    else {
        ComputeMain *cm = new ComputeMain(this, numRow, numIn1, numIn2, numOu1, numOu2, numOu3, sa, targetValue);
        cm -> show();
        this -> close();
    }
}

void NewComputeWin::on_CancelButton_clicked() {
    this -> close();
}
