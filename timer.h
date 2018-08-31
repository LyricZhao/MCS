# ifndef TIMER_H
# define TIMER_H

# include <QTimer>
# include <QObject>
# include <QThread>
# include <QLCDNumber>

# define timeI 1

class timer:  public QThread{
    Q_OBJECT

public:
    timer(QLCDNumber *lcd0): lcd(lcd0) { moveToThread(this); }
    ~timer() {}

protected:
    void run();

public slots:
    void updateTime();
    void startClock();
    void endClock();

private:
    int curTime, inv;
    QLCDNumber *lcd;
    QTimer *m_timer;
};

#endif // TIMER_H
