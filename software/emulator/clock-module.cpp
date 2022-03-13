#include "clock-module.h"
#include "bbcomputer.h"
#include "ui_bbcomputer.h"


ClockModule::ClockModule(Ui::BBComputer *ui) : QObject(), ui(ui)
{
    isRunMode = true;
    isHalted = false;

    reset();

    timer = new Timer();

    // -- connect the timer to its LED and frequency control
    connect(timer, &Timer::risingEdge, ui->timerLed, &KLed::on);
    connect(timer, &Timer::fallingEdge, ui->timerLed, &KLed::off);

    connect(ui->clockFreqSlider, &QSlider::valueChanged, timer, &Timer::updateFreq);


    // -- connect the run/manual switch to the slots
    connect(ui->runModeCheckbox, &QCheckBox::stateChanged, this, &ClockModule::setRun);

    // -- also connect the timer to the timer rising and falling edge slots
    connect(timer, &Timer::risingEdge, this, &ClockModule::timerRisingEdge);
    connect(timer, &Timer::fallingEdge, this, &ClockModule::timerFallingEdge);

    // -- wire up the manual push button to its LED and the internal control
    connect(ui->maunalTick, &QPushButton::pressed, ui->tickLed, &KLed::on);
    connect(ui->maunalTick, &QPushButton::released, ui->tickLed, &KLed::off);
    connect(ui->maunalTick, &QPushButton::pressed, this, &ClockModule::manualRisingEdge);
    connect(ui->maunalTick, &QPushButton::released, this, &ClockModule::manualFallingEdge);


    // -- connect the signals to the clock LED
    connect(this, &ClockModule::clockRisingEdge, ui->clockLed, &KLed::on);
    connect(this, &ClockModule::clockFallingEdge, ui->clockLed, &KLed::off);

    timer->start();
}


ClockModule::~ClockModule()
{
    timer->requestInterruption();
}


void ClockModule::reset(void)
{
    // -- turn off the halted LED
    ui->haltLed->off();
    isHalted = false;
}


void ClockModule::halt(void)
{
    isHalted = true;
    ui->haltLed->on();
}


void ClockModule::setRun(void)
{
    isRunMode = (ui->runModeCheckbox->checkState() == Qt::Checked);

    if (!isRunMode && ui->clockLed->state() == KLed::On) {
        emit clockFallingEdge();
    }
}


void ClockModule::manualRisingEdge(void)
{
    if (!isHalted && !isRunMode)
    {
        emit clockRisingEdge();
    }
}


void ClockModule::manualFallingEdge(void)
{
    if (!isHalted && !isRunMode)
    {
        emit clockFallingEdge();
    }
}

void ClockModule::timerRisingEdge(void)
{
    if (!isHalted && isRunMode)
    {
        emit clockRisingEdge();
    }
}


void ClockModule::timerFallingEdge(void)
{
    if (!isHalted && isRunMode)
    {
        emit clockFallingEdge();
    }
}
