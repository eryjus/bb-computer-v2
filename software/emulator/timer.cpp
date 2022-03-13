#include "timer.h"


void Timer::updateFreq(int f)
{
    freq = f;
}


void Timer::run(void)
{
    long delay;

    while (!isInterruptionRequested()) {
        emit risingEdge();
        delay = 500000 / (freq?freq:1);
        usleep(delay);

        emit fallingEdge();
        delay = 500000 / (freq?freq:1);
        usleep(delay);
    }
}
