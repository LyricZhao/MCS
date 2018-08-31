#ifndef NEWCOMPUTEWIN_H
#define NEWCOMPUTEWIN_H

#include <QDialog>

namespace Ui {
class NewComputeWin;
}

class NewComputeWin : public QDialog
{
    Q_OBJECT

public:
    explicit NewComputeWin(QWidget *parent = 0, bool isSA = false, double tv = 0);
    ~NewComputeWin();

private slots:

    void on_OKButton_clicked();
    void on_CancelButton_clicked();

private:
    bool sa;
    double targetValue;

    int numRow;
    int numIn1, numIn2, numOu1, numOu2, numOu3;

    Ui::NewComputeWin *ui;
    bool checkInput();
};

#endif // NEWCOMPUTEWIN_H
