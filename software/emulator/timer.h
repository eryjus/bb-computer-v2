#ifndef TIMER_H
#define TIMER_H

#include <QThread>


class Timer : public QThread {
    Q_OBJECT

private:
    volatile int freq;

public:
    Timer() : QThread(), freq(1) {}

signals:
    void risingEdge(void);
    void fallingEdge(void);

public slots:
    void updateFreq(int f);

public:
    virtual void run(void) override;
};


#endif // TIMER_H
