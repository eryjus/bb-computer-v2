#include "temp-reg-1.h"



TempRegister1::TempRegister1(BBComputer *bb) : Register(bb)
{
    clear();

    bit0Led = ui->lTmpReg1Bit0;
    bit1Led = ui->lTmpReg1Bit1;
    bit2Led = ui->lTmpReg1Bit2;
    bit3Led = ui->lTmpReg1Bit3;
    bit4Led = ui->lTmpReg1Bit4;
    bit5Led = ui->lTmpReg1Bit5;
    bit6Led = ui->lTmpReg1Bit6;
    bit7Led = ui->lTmpReg1Bit7;
    bit8Led = ui->uTmpReg1Bit0;
    bit9Led = ui->uTmpReg1Bit1;
    bit10Led = ui->uTmpReg1Bit2;
    bit11Led = ui->uTmpReg1Bit3;
    bit12Led = ui->uTmpReg1Bit4;
    bit13Led = ui->uTmpReg1Bit5;
    bit14Led = ui->uTmpReg1Bit6;
    bit15Led = ui->uTmpReg1Bit7;
    upperAssertLed = ui->uTmpReg1AssertLed;
    lowerAssertLed = ui->lTmpReg1AssertLed;
    upperReadLed = ui->uTmpReg1ReadLed;
    lowerReadLed = ui->lTmpReg1ReadLed;

    connect(bb->getClock(), &ClockModule::clockRisingEdge, this, &TempRegister1::clockTick);
    connect(ui->t1ClrButton, &QPushButton::pressed, this, &TempRegister1::clear);
}


