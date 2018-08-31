#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_AboutButton_clicked();
    void on_QuitButton_clicked();

    void on_QuitAction_triggered();
    void on_AboutAction_triggered();
    void on_ComputeAction_triggered();

    void on_SAAction_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
