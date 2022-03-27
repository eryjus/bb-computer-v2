#ifndef BBCOMPUTER_H
#define BBCOMPUTER_H

class Bus;
class ClockModule;
class ControlModule;
class BBComputer;
class ProgramCounterModule;
class TempRegister1;
class TempRegister2;

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class BBComputer; }
QT_END_NAMESPACE


class BBComputer : public QMainWindow
{
    Q_OBJECT


private:
    Bus *bus;
    ClockModule *clock;
    ControlModule *control;
    ProgramCounterModule *programCounter;
    TempRegister1 *tempReg1;
    TempRegister2 *tempReg2;


public:
    BBComputer(QWidget *parent = nullptr);
    virtual ~BBComputer();

public:
    Ui::BBComputer *getUi(void) const { return ui; }
    Bus *getBus(void) const { return bus; }
    ClockModule *getClock(void) const { return clock; }
    ControlModule *getControl(void) const { return control; }
    ProgramCounterModule *getProgramCounter(void) const { return programCounter; }
    TempRegister1 *getTempReg1(void) const { return tempReg1; }
    TempRegister2 *getTempReg2(void) const { return tempReg2; }

private:
    Ui::BBComputer *ui;
};

#include "clock-module.h"
#include "control-module.h"
#include "program-counter-module.h"
#include "bus.h"
#include "temp-reg-1.h"
#include "temp-reg-2.h"


#endif // BBCOMPUTER_H
