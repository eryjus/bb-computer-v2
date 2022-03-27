#include "program-counter-module.h"


ProgramCounterModule::ProgramCounterModule(BBComputer *bb) : Register(bb)
{
    isEnabled = false;
    clear();

    bit0Led = ui->lPrgRegBit0;
    bit1Led = ui->lPrgRegBit1;
    bit2Led = ui->lPrgRegBit2;
    bit3Led = ui->lPrgRegBit3;
    bit4Led = ui->lPrgRegBit4;
    bit5Led = ui->lPrgRegBit5;
    bit6Led = ui->lPrgRegBit6;
    bit7Led = ui->lPrgRegBit7;
    bit8Led = ui->uPrgRegBit0;
    bit9Led = ui->uPrgRegBit1;
    bit10Led = ui->uPrgRegBit2;
    bit11Led = ui->uPrgRegBit3;
    bit12Led = ui->uPrgRegBit4;
    bit13Led = ui->uPrgRegBit5;
    bit14Led = ui->uPrgRegBit6;
    bit15Led = ui->uPrgRegBit7;
    upperAssertLed = ui->uPrgRegAssertLed;
    lowerAssertLed = ui->lPrgRegAssertLed;
    upperReadLed = ui->uPrgRegReadLed;
    lowerReadLed = ui->lPrgRegReadLed;

    connect(bb->getClock(), &ClockModule::clockRisingEdge, this, &ProgramCounterModule::clockTick);
    connect(ui->pcClrButton, &QPushButton::pressed, this, &ProgramCounterModule::clear);
}


void ProgramCounterModule::setEnabled(void)
{
    isEnabled = true;
}

void ProgramCounterModule::setDisabled(void)
{
    isEnabled = false;
}

void ProgramCounterModule::clockTick(void) {
    if (isEnabled) {
        value ++;
        updateRegister();
    }

    // -- update the bus?
    if (assertUpperEnabled) bb->getBus()->setValue((value >> 8) & 0xff);
    if (assertLowerEnabled) bb->getBus()->setValue(value & 0xff);

    // -- latch the upper program counter on rising edge
    updateLower();

    // -- latch the upper program counter on rising edge
    updateUpper();
}


