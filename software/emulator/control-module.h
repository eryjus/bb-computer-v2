#ifndef CONTROLMODULE_H
#define CONTROLMODULE_H

class ControlModule;

#include <QWidget>
#include "bbcomputer.h"
#include "ui_bbcomputer.h"

class ControlModule : public QWidget
{
    Q_OBJECT

protected:
    BBComputer *bb;
    Ui::BBComputer *ui;

public:
    ControlModule(BBComputer *bb);
    virtual ~ControlModule() {}

public slots:
    void doReset(void);
    void doHalt(void);

    // -- Program Counter Controls
    void doCountEnableToggle(void);
    void doCountEnableOn(void);
    void doCountEnableOff(void);

    void doUpcOutToggle(void);
    void doUpcOutEnableOn(void);
    void doUpcOutEnableOff(void);

    void doLpcOutToggle(void);
    void doLpcOutEnableOn(void);
    void doLpcOutEnableOff(void);

    void doUpcInToggle(void);
    void doUpcInEnableOn(void);
    void doUpcInEnableOff(void);

    void doLpcInToggle(void);
    void doLpcInEnableOn(void);
    void doLpcInEnableOff(void);

    // -- Temp Register 1 Controls
    void doUt1OutToggle(void);
    void doUt1OutEnableOn(void);
    void doUt1OutEnableOff(void);

    void doLt1OutToggle(void);
    void doLt1OutEnableOn(void);
    void doLt1OutEnableOff(void);

    void doUt1InToggle(void);
    void doUt1InEnableOn(void);
    void doUt1InEnableOff(void);

    void doLt1InToggle(void);
    void doLt1InEnableOn(void);
    void doLt1InEnableOff(void);

    // -- Temp Register 2 Controls
    void doUt2OutToggle(void);
    void doUt2OutEnableOn(void);
    void doUt2OutEnableOff(void);

    void doLt2OutToggle(void);
    void doLt2OutEnableOn(void);
    void doLt2OutEnableOff(void);

    void doUt2InToggle(void);
    void doUt2InEnableOn(void);
    void doUt2InEnableOff(void);

    void doLt2InToggle(void);
    void doLt2InEnableOn(void);
    void doLt2InEnableOff(void);



private slots:
    void resetStart(void);
    void resetEnd(void);
};

#endif // CONTROLMODULE_H
