#include "control-module.h"


#include <QThread>

ControlModule::ControlModule(BBComputer *bb) : bb(bb), ui(bb->getUi())
{
    // -- wire-up the reset button
    connect(ui->resetButton, &QPushButton::pressed, this, &ControlModule::doReset);

    // -- wire up the halt control line
    connect(ui->haltButton, &QPushButton::pressed, this, &ControlModule::doHalt);

    // -- wire up the program counter control lines
    connect(ui->pcEnableCtl, &QCheckBox::stateChanged, this, &ControlModule::doCountEnableToggle);
    connect(ui->upcOutCtl, &QCheckBox::stateChanged, this, &ControlModule::doUpcOutToggle);
    connect(ui->lpcOutCtl, &QCheckBox::stateChanged, this, &ControlModule::doLpcOutToggle);
    connect(ui->upcInCtl, &QCheckBox::stateChanged, this, &ControlModule::doUpcInToggle);
    connect(ui->lpcInCtl, &QCheckBox::stateChanged, this, &ControlModule::doLpcInToggle);

    // -- wire up the temp reg 1 control lines
    connect(ui->uT1OutCtl, &QCheckBox::stateChanged, this, &ControlModule::doUt1OutToggle);
    connect(ui->lT1OutCtl, &QCheckBox::stateChanged, this, &ControlModule::doLt1OutToggle);
    connect(ui->uT1InCtl, &QCheckBox::stateChanged, this, &ControlModule::doUt1InToggle);
    connect(ui->lT1InCtl, &QCheckBox::stateChanged, this, &ControlModule::doLt1InToggle);

    // -- wire up the temp reg 2 control lines
    connect(ui->uT2OutCtl, &QCheckBox::stateChanged, this, &ControlModule::doUt2OutToggle);
    connect(ui->lT2OutCtl, &QCheckBox::stateChanged, this, &ControlModule::doLt2OutToggle);
    connect(ui->uT2InCtl, &QCheckBox::stateChanged, this, &ControlModule::doUt2InToggle);
    connect(ui->lT2InCtl, &QCheckBox::stateChanged, this, &ControlModule::doLt2InToggle);
}


void ControlModule::resetStart(void)
{
    ui->resettingLed->on();
    ui->resettingLed->repaint();
    ui->resetButton->setDisabled(true);
}

void ControlModule::resetEnd(void)
{
    ui->resetButton->setDisabled(false);
    ui->resettingLed->repaint();
    ui->resettingLed->off();
}


void ControlModule::doReset(void)
{
    resetStart();

    // -- set the halt condition during the reset
    bb->getClock()->halt();
    ui->haltLed->repaint();

    // -- sleep to make sure it's visible momentarily
    QThread::msleep(50);

    // -- reset the program counter and its controls
    bb->getProgramCounter()->reset();
    ui->lpcInCtl->setCheckState(Qt::Unchecked);
    ui->lpcInLed->off();
    ui->upcInCtl->setCheckState(Qt::Unchecked);
    ui->upcInLed->off();
    ui->lpcOutCtl->setCheckState(Qt::Unchecked);
    ui->lpcOutLed->off();
    ui->upcOutCtl->setCheckState(Qt::Unchecked);
    ui->upcOutLed->off();
    ui->pcEnableCtl->setCheckState(Qt::Unchecked);
    ui->clockEnabCtlLed->off();

    // -- reset the Temp1 Register
    bb->getTempReg1()->clear();
    ui->lT1InCtl->setCheckState(Qt::Unchecked);
    ui->lT1InLed->off();
    ui->uT1InCtl->setCheckState(Qt::Unchecked);
    ui->uT1InLed->off();
    ui->lT1OutCtl->setCheckState(Qt::Unchecked);
    ui->lT1OutLed->off();
    ui->uT1OutCtl->setCheckState(Qt::Unchecked);
    ui->uT1OutLed->off();

    // -- reset the Temp2 Register
    bb->getTempReg2()->clear();
    ui->lT2InCtl->setCheckState(Qt::Unchecked);
    ui->lT2InLed->off();
    ui->uT2InCtl->setCheckState(Qt::Unchecked);
    ui->uT2InLed->off();
    ui->lT2OutCtl->setCheckState(Qt::Unchecked);
    ui->lT2OutLed->off();
    ui->uT2OutCtl->setCheckState(Qt::Unchecked);
    ui->uT2OutLed->off();

    bb->getClock()->reset();

    resetEnd();
}


void ControlModule::doHalt(void)
{
    bb->getClock()->halt();
}


void ControlModule::doCountEnableToggle(void)
{
    if (ui->pcEnableCtl->checkState() == Qt::Checked) doCountEnableOn();
    else doCountEnableOff();
}


void ControlModule::doCountEnableOn(void)
{
    ui->clockEnabCtlLed->on();
    bb->getProgramCounter()->setEnabled();
}

void ControlModule::doCountEnableOff(void)
{
    ui->clockEnabCtlLed->off();
    bb->getProgramCounter()->setDisabled();
}



void ControlModule::doUpcOutToggle(void)
{
    if (ui->upcOutCtl->checkState() == Qt::Checked) doUpcOutEnableOn();
    else doUpcOutEnableOff();
}


void ControlModule::doUpcOutEnableOn(void)
{
    ui->upcOutLed->on();
    bb->getProgramCounter()->assertUpper();
}

void ControlModule::doUpcOutEnableOff(void)
{
    ui->upcOutLed->off();
    bb->getProgramCounter()->resetAssertUpper();
}



void ControlModule::doLpcOutToggle(void)
{
    if (ui->lpcOutCtl->checkState() == Qt::Checked) doLpcOutEnableOn();
    else doLpcOutEnableOff();
}


void ControlModule::doLpcOutEnableOn(void)
{
    ui->lpcOutLed->on();
    bb->getProgramCounter()->assertLower();
}

void ControlModule::doLpcOutEnableOff(void)
{
    ui->lpcOutLed->off();
    bb->getProgramCounter()->resetAssertLower();
}



void ControlModule::doUpcInToggle(void)
{
    if (ui->upcInCtl->checkState() == Qt::Checked) doUpcInEnableOn();
    else doUpcInEnableOff();
}


void ControlModule::doUpcInEnableOn(void)
{
    ui->upcInLed->on();
    bb->getProgramCounter()->readUpper();
}

void ControlModule::doUpcInEnableOff(void)
{
    ui->upcInLed->off();
    bb->getProgramCounter()->resetReadUpper();
}



void ControlModule::doLpcInToggle(void)
{
    if (ui->lpcInCtl->checkState() == Qt::Checked) doLpcInEnableOn();
    else doLpcInEnableOff();
}


void ControlModule::doLpcInEnableOn(void)
{
    ui->lpcInLed->on();
    bb->getProgramCounter()->readLower();
}

void ControlModule::doLpcInEnableOff(void)
{
    ui->lpcInLed->off();
    bb->getProgramCounter()->resetReadLower();
}




// -- Temp Register 1
void ControlModule::doUt1OutToggle(void)
{
    if (ui->uT1OutCtl->checkState() == Qt::Checked) doUt1OutEnableOn();
    else doUt1OutEnableOff();
}


void ControlModule::doUt1OutEnableOn(void)
{
    ui->uT1OutLed->on();
    bb->getTempReg1()->assertUpper();
}

void ControlModule::doUt1OutEnableOff(void)
{
    ui->uT1OutLed->off();
    bb->getTempReg1()->resetAssertUpper();
}



void ControlModule::doLt1OutToggle(void)
{
    if (ui->lT1OutCtl->checkState() == Qt::Checked) doLt1OutEnableOn();
    else doLt1OutEnableOff();
}


void ControlModule::doLt1OutEnableOn(void)
{
    ui->lT1OutLed->on();
    bb->getTempReg1()->assertLower();
}

void ControlModule::doLt1OutEnableOff(void)
{
    ui->lT1OutLed->off();
    bb->getTempReg1()->resetAssertLower();
}



void ControlModule::doUt1InToggle(void)
{
    if (ui->uT1InCtl->checkState() == Qt::Checked) doUt1InEnableOn();
    else doUt1InEnableOff();
}


void ControlModule::doUt1InEnableOn(void)
{
    ui->uT1InLed->on();
    bb->getTempReg1()->readUpper();
}

void ControlModule::doUt1InEnableOff(void)
{
    ui->uT1InLed->off();
    bb->getTempReg1()->resetReadUpper();
}



void ControlModule::doLt1InToggle(void)
{
    if (ui->lT1InCtl->checkState() == Qt::Checked) doLt1InEnableOn();
    else doLt1InEnableOff();
}


void ControlModule::doLt1InEnableOn(void)
{
    ui->lT1InLed->on();
    bb->getTempReg1()->readLower();
}

void ControlModule::doLt1InEnableOff(void)
{
    ui->lT1InLed->off();
    bb->getTempReg1()->resetReadLower();
}



// -- Temp Register 2
void ControlModule::doUt2OutToggle(void)
{
    if (ui->uT2OutCtl->checkState() == Qt::Checked) doUt2OutEnableOn();
    else doUt2OutEnableOff();
}


void ControlModule::doUt2OutEnableOn(void)
{
    ui->uT2OutLed->on();
    bb->getTempReg2()->assertUpper();
}

void ControlModule::doUt2OutEnableOff(void)
{
    ui->uT2OutLed->off();
    bb->getTempReg2()->resetAssertUpper();
}



void ControlModule::doLt2OutToggle(void)
{
    if (ui->lT2OutCtl->checkState() == Qt::Checked) doLt2OutEnableOn();
    else doLt2OutEnableOff();
}


void ControlModule::doLt2OutEnableOn(void)
{
    ui->lT2OutLed->on();
    bb->getTempReg2()->assertLower();
}

void ControlModule::doLt2OutEnableOff(void)
{
    ui->lT2OutLed->off();
    bb->getTempReg2()->resetAssertLower();
}



void ControlModule::doUt2InToggle(void)
{
    if (ui->uT2InCtl->checkState() == Qt::Checked) doUt2InEnableOn();
    else doUt2InEnableOff();
}


void ControlModule::doUt2InEnableOn(void)
{
    ui->uT2InLed->on();
    bb->getTempReg2()->readUpper();
}

void ControlModule::doUt2InEnableOff(void)
{
    ui->uT2InLed->off();
    bb->getTempReg2()->resetReadUpper();
}



void ControlModule::doLt2InToggle(void)
{
    if (ui->lT2InCtl->checkState() == Qt::Checked) doLt2InEnableOn();
    else doLt2InEnableOff();
}


void ControlModule::doLt2InEnableOn(void)
{
    ui->lT2InLed->on();
    bb->getTempReg2()->readLower();
}

void ControlModule::doLt2InEnableOff(void)
{
    ui->lT2InLed->off();
    bb->getTempReg2()->resetReadLower();
}




