#ifndef CLOCKMODULE_H
#define CLOCKMODULE_H


#include "ui_bbcomputer.h"
#include "timer.h"

#include <QObject>


class ClockModule : public QObject {
    Q_OBJECT

protected:
    Ui::BBComputer *ui;
    bool isHalted;
    bool isRunMode;


private:
    Timer *timer;


signals:
    void clockRisingEdge(void);
    void clockFallingEdge(void);


public slots:
    void reset(void);
    void halt(void);
    void setRun(void);
    void manualRisingEdge(void);
    void manualFallingEdge(void);
    void timerRisingEdge(void);
    void timerFallingEdge(void);


public:
    ClockModule(Ui::BBComputer *ui);
    virtual ~ClockModule();

};


#endif // CLOCKMODULE_H
