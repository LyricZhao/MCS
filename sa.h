# ifndef SA_H
# define SA_H

# include <QMutex>
# include <QLabel>
# include <QThread>
# include <QObject>
# include <QVector>
# include <QLineEdit>
# include <QLCDNumber>

class SA: public QThread {
    Q_OBJECT

public:
    SA(int mrow, double mTarget, int _i1, int _i2, int _o1, int _o2, int _o3, double &fo1, double &fo2, double &fo3, QLineEdit *_ol1, QLineEdit *_ol2, QLineEdit *_ol3, QVector<bool> &_qSV, QVector<bool> &_qSH, QVector<double> &_fV, QVector<double> &_fH, QMutex &_mutex, bool &_stopSignal, QLabel *qlab, QLCDNumber *qlcd, QLCDNumber *qtime):
        row(mrow), target(mTarget), i1(_i1), i2(_i2), o1(_o1), o2(_o2), o3(_o3), ff1(fo1), ff2(fo2), ff3(fo3), ol1(_ol1), ol2(_ol2), ol3(_ol3), qSV(_qSV), qSH(_qSH), fV(_fV), fH(_fH), mutex(_mutex), stopSignal(_stopSignal), resLabel(qlab), lcd(qlcd), timer(qtime)
    { moveToThread(this); }
    ~SA() {}

signals:
    void AutoStop();
    void roundEnd();

protected:
    void run();

private:
    QLabel *resLabel;
    QLCDNumber *lcd, *timer;
    QLineEdit *ol1, *ol2, *ol3;

    int row, counter;
    int i1, i2, o1, o2, o3;
    QVector<bool> &qSV, &qSH;
    QVector<double> &fV, &fH;
    QMutex &mutex;
    bool &stopSignal;
    double minDelta, target;
    double v1, v2, v3;
    double &ff1, &ff2, &ff3;
    void updateOnce();
    void randomMap(QVector<bool> &qSV, QVector<bool> &qSH);
};

#endif // SA_H
