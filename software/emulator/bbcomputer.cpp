#include "bbcomputer.h"
#include "ui_bbcomputer.h"

BBComputer::BBComputer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BBComputer)
{
    ui->setupUi(this);
    bus = new Bus(this);
    clock = new ClockModule(this);
    control = new ControlModule(this);
    programCounter = new ProgramCounterModule(this);
    tempReg1 = new TempRegister1(this);
    tempReg2 = new TempRegister2(this);
}

BBComputer::~BBComputer()
{
    delete bus;
    delete clock;
    delete control;
    delete programCounter;
    delete tempReg1;
    delete tempReg2;

    delete ui;
}

