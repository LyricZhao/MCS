# include "about.h"
# include "mainwindow.h"
# include "newcomputewin.h"
# include "ui_mainwindow.h"

# include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui -> setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_AboutButton_clicked() {
    About *aboutDialog = new About(this);
    aboutDialog -> show();
}

void MainWindow::on_QuitButton_clicked() {
    this -> close();
}

void MainWindow::on_QuitAction_triggered() {
    this -> close();
}

void MainWindow::on_AboutAction_triggered() {
    About *aboutDialog = new About(this);
    aboutDialog -> show();
}

void MainWindow::on_ComputeAction_triggered() {
    NewComputeWin *ncw = new NewComputeWin(this, false);
    ncw -> show();
}

void MainWindow::on_SAAction_triggered() {
    bool ok = false;
    double ret = QInputDialog:: getDouble(this, tr("MCS"), tr("Please input the target flow."), 150, 100, 200, 0, &ok);
    if(!ok) return;

    NewComputeWin *ncw = new NewComputeWin(this, true, ret);
    ncw -> show();
}
