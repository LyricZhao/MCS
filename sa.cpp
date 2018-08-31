# include "sa.h"
# include "solver.h"

# include <cmath>
# include <ctime>
# include <vector>
# include <cstdlib>
# include <QVector>
# include <sys/time.h>

unsigned long long get_wall_time() {
  timeval ti;
  gettimeofday(&ti, NULL);
  return ti.tv_sec * 1000000ull + ti.tv_usec;
}

void SA:: randomMap(QVector<bool> &qSV, QVector<bool> &qSH) {
    srand(19981011);

    for(int i = 0; i < qSV.size(); ++ i) {
        int k = rand() % 20;
        qSV[k] = (k == 19) ? false : true;
    }

    for(int i = 0; i < qSH.size(); ++ i) {
        int k = rand() % 20;
        qSH[k] = (k == 19) ? false : true;
    }

    minDelta = 1e9;
    counter = 0;
    updateOnce();
}

void SA:: updateOnce() {
    unsigned long long stime = get_wall_time();

    Solver solver;
    std:: vector<double> width;
    std:: vector<double> length;

    for(int i = 0; i < row; ++ i) {
        for(int j = 0; j < row - 1; ++ j) {
            int pos = j * row + i;
            width.push_back(200.0);
            length.push_back((double)(qSV[pos]));
        }
    }
    for(int i = 0; i < row - 1; ++ i) {
        for(int j = 0; j < row; ++ j) {
            int pos = j * (row - 1) + i;
            width.push_back(200.0);
            length.push_back((double)(qSH[pos]));
        }
    }
    for(int i = 0; i < 5; ++ i) length.push_back(1), width.push_back(200.0);
    std:: vector<double> result = solver.caluconspeed(row, width, length, i1, i2, o1, o2, o3);
    v1 = result[2 * row * (row - 1) + 2];
    v2 = result[2 * row * (row - 1) + 3];
    v3 = result[2 * row * (row - 1) + 4];
    if(isnan(v1) || isnan(v2) || isnan(v3)) minDelta = 1e8;
    else {
        minDelta = std:: min(fabs(v1 - target), fabs(v2 - target));
        minDelta = std:: min(minDelta, fabs(v3 - target));
    }

    ++ counter;
    lcd -> display(counter);

    unsigned long long etime = get_wall_time();
    int ms = etime - stime;

    timer -> display(ms);
}

void SA:: run() {
    randomMap(qSV, qSH);

    assert(qSV.size() == qSH.size());

    double T = 1e6;
    while(T > 0.0001) {
        double oDelta = minDelta;

        int c = rand() & 1, v = rand() % qSV.size();
        if(c == 0) qSV[v] ^= 1;
        if(c == 1) qSH[v] ^= 1;
        updateOnce();

        double de = oDelta - minDelta;
        if(de > 0 || exp(de / T) > ((rand() % 1000) / 1000.0)) {
            resLabel -> setText(tr("minDelta: ") + QString:: number(minDelta) + tr(" (") + QString:: number(target) + tr(")"));
            ol1 -> setText(QString:: number(v1));
            ol2 -> setText(QString:: number(v2));
            ol3 -> setText(QString:: number(v3));
            ff1 = v1, ff2 = v2, ff3 = v3;
            emit roundEnd();
        } else {
            if(c == 0) qSV[v] ^= 1;
            if(c == 1) qSH[v] ^= 1;
            minDelta = oDelta;
        }

        QMutexLocker locker(&mutex);
        if(stopSignal == true) break;
        locker.unlock();

        T *= 0.993;
    }
    emit AutoStop();
    this -> quit();
}
