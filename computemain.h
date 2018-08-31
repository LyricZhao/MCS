# ifndef COMPUTEMAIN_H
# define COMPUTEMAIN_H

# include <QRect>
# include <QDialog>
# include <QVector>
# include <QThread>

# include "timer.h"

// width default = 200 -> 5 (/40)

# define mR 40
# define mL (mR / 8)

# define dW 200

namespace Ui {
class ComputeMain;
}

class ComputeMain : public QDialog
{
    Q_OBJECT

public:
    int row, i1, i2, o1, o2, o3;
    bool sa; double targetValue;
    explicit ComputeMain(QWidget *parent = 0, int r = -1, int in1 = -1, int in2 = -1, int ou1 = -1, int ou2 = -1, int ou3 = -1, bool isa = false, double itv = 0);
    ~ComputeMain();

private slots:
    // void on_cancelButton_clicked();

signals:
    void m_startClock();
    void m_endClock();

private:
    bool changeOK;
    int opX, opY;
    double fi1, fi2, fo1, fo2, fo3;
    QPoint mousePos;
    QVector<int> wH, wV;
    QVector<QRect> qrecH, qrecV;
    QVector<bool> qSH, qSV;
    QVector<double> fH, fV;
    timer *timerThread;
    Ui::ComputeMain *ui;

    double cv(double v);
    int nodeID(int i, int j);
    int getMax(int v, int i);
    void changeWidth(int v, int i);
    void setR3(double v1, double v2, double v3);
    QColor getColor(double v);
    void compute();

    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
};

#endif // COMPUTEMAIN_H
