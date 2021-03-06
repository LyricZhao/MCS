# include "sa.h"
# include "solver.h"
# include "csolver.h"
# include "computemain.h"
# include "ui_computemain.h"

# include <cmath>
# include <tuple>
# include <unistd.h>
# include <iostream>
# include <unistd.h>

# include <QPoint>
# include <QTimer>
# include <QPainter>
# include <QMouseEvent>
# include <QMutexLocker>
# include <QInputDialog>

ComputeMain:: ComputeMain(QWidget *parent, int r, int in1, int in2, int ou1, int ou2, int ou3, bool isa, double itv) :
   QDialog(parent),
    row(r), i1(in1), i2(in2), o1(ou1), o2(ou2), o3(ou3), sa(isa), targetValue(itv),
    ui(new Ui::ComputeMain) {
    changeOK = false;
    saThread = NULL;
    // draw chip
    for(int i = 0; i < row - 1; ++ i) {
        for(int j = 0; j < row; ++ j) {
            QRect tempRec((mR + mL) * j, mR * i + mL * (i + 1), mL, mR);
            qrecV.push_back(tempRec);
            qSV.push_back(true);
            wV.push_back(dW);
            fV.push_back(200);
        }
    }
    for(int i = 0; i < row; ++ i) {
        for(int j = 0; j < row - 1; ++ j) {
            QRect tempRec(mR * j + mL * (j + 1), (mR + mL) * i, mR, mL);
            qrecH.push_back(tempRec);
            qSH.push_back(true);
            wH.push_back(dW);
            fH.push_back(200);
        }
    }

    this -> setMouseTracking(true);
    ui -> setupUi(this);

    runningsa = false;

    timerThread = new timer(ui -> lcd);
    connect(this, SIGNAL(m_startClock()), timerThread, SLOT(startClock()));
    connect(this, SIGNAL(m_endClock()), timerThread, SLOT(endClock()));
    timerThread -> start();

    if(sa == false) {
        ui -> admLabel -> hide();
        ui -> saLayout -> setEnabled(false);
        ui -> resultLabel -> hide();
        ui -> saStart -> hide();
        ui -> saStop -> hide();
        ui -> roundLabel -> hide();
        ui -> roundLCD -> hide();
        changeOK = true;
        compute();
    } else {
        ui -> saStop -> setEnabled(false);
        ui -> cmLabel -> hide();
    }
}

QColor ComputeMain:: getColor(double v) {
    if(std:: isnan(v) || v <= 0.00001) return Qt:: black;
    double ratio = v / 400;
    QColor s = Qt:: red, e = Qt:: blue;
    int red = (int)(ratio * s.red() + (1 - ratio) * e.red());
    int green = (int)(ratio * s.green() + (1 - ratio) * e.green());
    int blue = (int)(ratio * s.blue() + (1 - ratio) * e.blue());
    return QColor(red, green, blue);
}

void ComputeMain:: paintEvent(QPaintEvent *ev) {
    // std:: cout << fo1 << " " << fo2 << " " << fo3 << std:: endl;


    int mcX = 200, mcY = 225;
    int maxX = (row - 1) * mR + row * mL;
    int maxY = (row - 1) * mR + row * mL;
    opX = mcX - maxX / 2;
    opY = mcY - maxY / 2;

    QPainter painter(this);
    painter.setPen(Qt:: NoPen);
    painter.setBrush(Qt:: white);
    painter.drawRect(0, 0, 500, 450);

    // Linear Gradient
    QLinearGradient linearGradient(430, 50, 430, 400);
    linearGradient.setColorAt(0, Qt:: red);
    linearGradient.setColorAt(1.0, Qt:: blue);
    painter.setBrush(linearGradient);
    painter.drawRect(415, 50, 30, 350);

    painter.translate(opX, opY);
    painter.setBrush(Qt:: gray);

    // draw basic line
    for(int i = 0; i < row; ++ i) {
        painter.drawRect((mR + mL) * i, 0, mL, maxY);
    }
    for(int i = 0; i < row; ++ i) {
        painter.drawRect(0, (mR + mL) * i, maxX, mL);
    }

    // draw edge
    painter.setBrush(Qt:: red);
    painter.setBrush(getColor(200)); painter.drawRect(i1 * (mL + mR), -mR, mL, mR);
    painter.setBrush(getColor(200)); painter.drawRect(i2 * (mL + mR), -mR, mL, mR);
    painter.setBrush(getColor(fo1)); painter.drawRect(o1 * (mL + mR), maxY, mL, mR);
    painter.setBrush(getColor(fo2)); painter.drawRect(o2 * (mL + mR), maxY, mL, mR);
    painter.setBrush(getColor(fo3)); painter.drawRect(o3 * (mL + mR), maxY, mL, mR);

    // draw chip
    int siz;
    siz = qrecV.size();
    for(int i = 0; i < siz; ++ i) {
        painter.setBrush(qSV[i] ? (getColor(fV[i])) : (Qt:: gray));
        painter.drawRect(qrecV[i]);
    }

    siz = qrecH.size();
    for(int i = 0; i < siz; ++ i) {
        painter.setBrush(qSH[i] ? (getColor(fH[i])) : (Qt:: gray));
        painter.drawRect(qrecH[i]);
    }

    // draw point
    painter.setPen(QPen(Qt:: black, 1, Qt::SolidLine));
    painter.setBrush(Qt:: white);
    for(int i = 0; i < row; ++ i) {
        for(int j = 0; j < row; ++ j) {
            int count = 0;
            if(i == 0) count += (j == i1 || j == i2);
            if(i == row - 1) count += (j == o1 || j == o2 || j == o3);
            if(i != 0) count += qSV[(i - 1) * row + j];
            if(i != row - 1) count += qSV[i * row + j];
            if(j != 0) count += qSH[i * (row - 1) + j - 1];
            if(j != row - 1) count += qSH[i * (row - 1) + j];
            if(count >= 2)
                painter.drawEllipse(QPoint(j * (mL + mR) + mL / 2, i * (mL + mR) + mL / 2), 6, 6);
        }
    }
    painter.setPen(Qt:: NoPen);
}

int ComputeMain:: getMax(int v, int i) {
    int distance, ret = dW * 8 * 2;
    if(v == 0) {
        int h = i % row;
        if(h != 0) {
            distance = dW * 14 - wV[i - 1];
            if(distance < ret) ret = distance;
        }
        if(h != row - 1) {
            distance = dW * 14 - wV[i + 1];
            if(distance < ret) ret = distance;
        }
    } else {
        int h = i / (row - 1);
        if(h != 0) {
            distance = dW * 14 - wH[i - (row - 1)];
            if(distance < ret) ret = distance;
        }
        if(h != row - 1) {
            distance = dW * 14 - wH[i + (row - 1)];
            if(distance < ret) ret = distance;
        }
    }
    return ret;
}

void ComputeMain:: changeWidth(int vec, int i) {
    bool ok = false;
    // std:: cout << "max = " << getMax(vec, i) << std:: endl;
    int ret = QInputDialog:: getInt(this, tr("MCS"), tr("Please input the width of this pipe."), vec == 0 ? wV[i] : wH[i], 20, getMax(vec, i), 1, &ok);
    if(ok) {
        int length = ret / 40;
        if(vec == 0) {
            int v = i / row;
            int h = i % row;
            wV[i] = ret;
            qrecV[i] = QRect(h * (mL + mR) + mL / 2 - length / 2, (v + 1) * mL + v * mR, length, mR);
        } else {
            int v = i / (row - 1);
            int h = i % (row - 1);
            wH[i] = ret;
            qrecH[i] = QRect((h + 1) * mL + h * mR, v * (mL + mR) + mL / 2 - length / 2, mR, length);
        }
        compute();
        update();
    }
}

void ComputeMain:: keyPressEvent(QKeyEvent *ev) {
    if(!changeOK) return;
    if(ev -> key() != Qt::Key_Space) return;

    QPoint qpos = mousePos - QPoint(opX, opY);

    int siz;
    siz = qrecV.size();
    for(int i = 0; i < siz; ++ i) {
        if(qrecV[i].contains(qpos)) {
            changeWidth(0, i);
            return;
        }
    }

    siz = qrecH.size();
    for(int i = 0; i < siz; ++ i) {
        if(qrecH[i].contains(qpos)) {
            changeWidth(1, i);
            return;
        }
    }
}

void ComputeMain:: mouseMoveEvent(QMouseEvent *ev) {
    mousePos = ev -> pos();
}

void ComputeMain:: mousePressEvent(QMouseEvent *ev) {
    if(!changeOK) return;

    QPoint qpos = ev -> pos();
    qpos -= QPoint(opX, opY);

    int siz;
    siz = qrecV.size();
    for(int i = 0; i < siz; ++ i) {
        if(qrecV[i].contains(qpos)) {
            qSV[i] ^= 1;
            compute();
            update();
            return;
        }
    }

    siz = qrecH.size();
    for(int i = 0; i < siz; ++ i) {
        if(qrecH[i].contains(qpos)) {
            qSH[i] ^= 1;
            compute();
            update();
            return;
        }
    }
}

ComputeMain::~ComputeMain() {
    delete ui;
}

double ComputeMain:: cv(double v) {
    if(std:: isnan(v) || v < 0.00001) return 0;
    return v;
}

int ComputeMain:: nodeID(int i, int j) {
    return i * row + j + 2;
}

void ComputeMain:: setR3(double v1, double v2, double v3) {
    fo1 = v1, fo2 = v2, fo3 = v3;
}

void ComputeMain:: compute() {
    changeOK = false;
    std:: vector<double> width;
    std:: vector<double> length;
    for(int i = 0; i < row; ++ i) {
        for(int j = 0; j < row - 1; ++ j) {
            int pos = j * row + i;
            width.push_back((double)(wV[pos]));
            length.push_back((double)(qSV[pos]));
        }
    }
    for(int i = 0; i < row - 1; ++ i) {
        for(int j = 0; j < row; ++ j) {
            int pos = j * (row - 1) + i;
            width.push_back((double)(wH[pos]));
            length.push_back((double)(qSH[pos]));
        }
    }
    for(int i = 0; i < 5; ++ i) length.push_back(1), width.push_back(dW);

    emit m_startClock();
    // timerThread -> startClock();

    Solver solver;
    std:: vector<double> result = solver.caluconspeed(row, width, length, i1, i2, o1, o2, o3);
    setR3(result[2 * row * (row - 1) + 2], result[2 * row * (row - 1) + 3], result[2 * row * (row - 1) + 4]);

    int tot = 0;
    std:: vector<std:: tuple<int, int, double> > edges;
    for(int i = 0; i < row; ++ i) {
        for(int j = 0; j < row - 1; ++ j, ++ tot) {
            int pos = j * row + i;
            fV[pos] = fabs(result[tot]);

            if(result[tot] > 0) edges.push_back(std:: make_tuple(nodeID(j, i), nodeID(j + 1, i),  result[tot]));
            else                edges.push_back(std:: make_tuple(nodeID(j + 1, i), nodeID(j, i), -result[tot]));
        }
    }
    for(int i = 0; i < row - 1; ++ i) {
        for(int j = 0; j < row; ++ j, ++ tot) {
            int pos = j * (row - 1) + i;
            fH[pos] = fabs(result[tot]);

            if(result[tot] > 0) edges.push_back(std:: make_tuple(nodeID(j, i), nodeID(j, i + 1),  result[tot]));
            else                edges.push_back(std:: make_tuple(nodeID(j, i + 1), nodeID(j, i), -result[tot]));
        }
    }

    fi1 = fi2 = 200;

    // input & output
    edges.push_back(std:: make_tuple(0, i1 + 2, 200));
    edges.push_back(std:: make_tuple(1, i2 + 2, 200));
    edges.push_back(std:: make_tuple(nodeID(row - 1, o1), row * row + 2, fo1));
    edges.push_back(std:: make_tuple(nodeID(row - 1, o2), row * row + 3, fo2));
    edges.push_back(std:: make_tuple(nodeID(row - 1, o3), row * row + 4, fo3));

    CSolver csolver(row * row + 5, edges);
    std:: tuple<double, double, double> cret = csolver.calc();

    double c1 = std:: get<0>(cret);
    double c2 = std:: get<1>(cret);
    double c3 = std:: get<2>(cret);

    ui -> ol1 -> setText(QString:: number(cv(fo1)) + "/" + QString:: number(cv(c1)));
    ui -> ol2 -> setText(QString:: number(cv(fo2)) + "/" + QString:: number(cv(c2)));
    ui -> ol3 -> setText(QString:: number(cv(fo3)) + "/" + QString:: number(cv(c3)));

    changeOK = true;
    emit m_endClock();
}

void ComputeMain::on_saStart_clicked() {
    if(runningsa) return;
    ui -> saStart -> setEnabled(false);
    ui -> saStop -> setEnabled(true);
    stopSignal = false;
    runningsa = true;
    if(saThread != NULL) delete saThread;
    saThread = new SA(row, targetValue, i1, i2, o1, o2, o3, fo1, fo2, fo3, ui -> ol1, ui -> ol2, ui -> ol3, qSV, qSH, fV, fH, mutex, stopSignal, ui -> resultLabel, ui -> roundLCD, ui -> lcd);
    connect(saThread, SIGNAL(roundEnd()), this, SLOT(update()));
    connect(saThread, SIGNAL(AutoStop()), this, SLOT(SAStop()));
    saThread -> start();
}

void ComputeMain:: SAStop() {
    ui -> saStart -> setEnabled(true);
    ui -> saStop -> setEnabled(false);
    disconnect(saThread, SIGNAL(roundEnd()), this, SLOT(update()));
    disconnect(saThread, SIGNAL(AutoStop()), this, SLOT(SAStop()));
    saThread -> wait();
    delete saThread;

    runningsa = false;
    saThread = NULL;
}

void ComputeMain::on_saStop_clicked() {
    if(runningsa == false) return;
    ui -> saStart -> setEnabled(true);
    ui -> saStop -> setEnabled(false);
    QMutexLocker locker(&mutex);
    stopSignal = true;
    disconnect(saThread, SIGNAL(roundEnd()), this, SLOT(update()));
    disconnect(saThread, SIGNAL(AutoStop()), this, SLOT(SAStop()));
    locker.unlock();
    saThread -> wait();
    delete saThread;

    runningsa = false;
    saThread = NULL;
}
