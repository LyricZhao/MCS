#include "timer.h"

void timer:: run() {
    m_timer = new QTimer;
    m_timer -> setTimerType(Qt:: PreciseTimer);
    m_timer -> setInterval(timeI);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    exec();
}

void timer:: startClock() {
    curTime = 0;
    m_timer -> start();
}

void timer:: endClock() {
    if(m_timer -> isActive()) {
        m_timer -> stop();
    }
}

void timer:: updateTime() {
    curTime += timeI;
    lcd -> display(curTime);
}
