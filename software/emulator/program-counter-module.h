#ifndef PROGRAMCOUNTERMODULE_H
#define PROGRAMCOUNTERMODULE_H


class ProgramCounterModule;

#include "register.h"
#include "bbcomputer.h"
#include "timer.h"

#include <QObject>


class ProgramCounterModule : public Register {
    Q_OBJECT

protected:
    bool isEnabled;

public slots:
    virtual void setEnabled(void);
    virtual void setDisabled(void);
    virtual void clockTick(void);
    virtual void reset(void) { clear(); }


public:
    ProgramCounterModule(BBComputer *bb);
    virtual ~ProgramCounterModule() {}
};


#endif // PROGRAMCOUNTERMODULE_H
