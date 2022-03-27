#include "bus.h"


Bus::Bus(BBComputer *bb) : QObject(), bb(bb), ui(bb->getUi())
{
    setDefault();
}


void Bus::setValue(uint8_t v)
{
    value = v;
    updateValue();
}

uint8_t Bus::readValue()
{
    return value;
}

void Bus::setDefault(void)
{
    setValue(0);
}

void Bus::updateValue(void)
{
    ui->busBit7->setState(value & 0x80 ? KLed::State::On : KLed::State::Off);
    ui->busBit6->setState(value & 0x40 ? KLed::State::On : KLed::State::Off);
    ui->busBit5->setState(value & 0x20 ? KLed::State::On : KLed::State::Off);
    ui->busBit4->setState(value & 0x10 ? KLed::State::On : KLed::State::Off);
    ui->busBit3->setState(value & 0x08 ? KLed::State::On : KLed::State::Off);
    ui->busBit2->setState(value & 0x04 ? KLed::State::On : KLed::State::Off);
    ui->busBit1->setState(value & 0x02 ? KLed::State::On : KLed::State::Off);
    ui->busBit0->setState(value & 0x01 ? KLed::State::On : KLed::State::Off);
}
