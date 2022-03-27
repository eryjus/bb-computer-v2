#include "temp-reg-2.h"



TempRegister2::TempRegister2(BBComputer *bb) : Register(bb)
{
    clear();

    bit0Led = ui->lTmpReg2Bit0;
    bit1Led = ui->lTmpReg2Bit1;
    bit2Led = ui->lTmpReg2Bit2;
    bit3Led = ui->lTmpReg2Bit3;
    bit4Led = ui->lTmpReg2Bit4;
    bit5Led = ui->lTmpReg2Bit5;
    bit6Led = ui->lTmpReg2Bit6;
    bit7Led = ui->lTmpReg2Bit7;
    bit8Led = ui->uTmpReg2Bit0;
    bit9Led = ui->uTmpReg2Bit1;
    bit10Led = ui->uTmpReg2Bit2;
    bit11Led = ui->uTmpReg2Bit3;
    bit12Led = ui->uTmpReg2Bit4;
    bit13Led = ui->uTmpReg2Bit5;
    bit14Led = ui->uTmpReg2Bit6;
    bit15Led = ui->uTmpReg2Bit7;
    upperAssertLed = ui->uTmpReg2AssertLed;
    lowerAssertLed = ui->lTmpReg2AssertLed;
    upperReadLed = ui->uTmpReg2ReadLed;
    lowerReadLed = ui->lTmpReg2ReadLed;

    connect(bb->getClock(), &ClockModule::clockRisingEdge, this, &TempRegister2::clockTick);
    connect(ui->t2ClrButton, &QPushButton::pressed, this, &TempRegister2::clear);
}


