#include "register.h"


Register::Register(BBComputer *bb) : QObject(), bb(bb), ui(bb->getUi())
{
    readUpperEnabled = false;
    readLowerEnabled = false;
    assertUpperEnabled = false;
    assertLowerEnabled = false;

    upperAssertLed = nullptr;
    lowerAssertLed = nullptr;
    upperReadLed = nullptr;
    lowerReadLed = nullptr;
    bit15Led = nullptr;
    bit14Led = nullptr;
    bit13Led = nullptr;
    bit12Led = nullptr;
    bit11Led = nullptr;
    bit10Led = nullptr;
    bit9Led = nullptr;
    bit8Led = nullptr;
    bit7Led = nullptr;
    bit6Led = nullptr;
    bit5Led = nullptr;
    bit4Led = nullptr;
    bit3Led = nullptr;
    bit2Led = nullptr;
    bit1Led = nullptr;
    bit0Led = nullptr;
}


void Register::clockTick(void) {
    // -- latch the upper program value on rising edge
    updateUpper();

    // -- latch the upper program value on rising edge
    updateLower();
}

void Register::updateUpper(void)
{
    if (readUpperEnabled) {
        value = (value & 0x00ff) | (bb->getBus()->readValue() << 8);
        updateRegister();
    }
}


void Register::updateLower(void)
{
    if (readLowerEnabled) {
        value = (value & 0xff00) | bb->getBus()->readValue();
        updateRegister();
    }
}


void Register::assertUpper(void)
{
    if (upperAssertLed != nullptr) {
        upperAssertLed->setState(KLed::On);
        upperAssertLed->repaint();
    }

    bb->getBus()->setValue((value >> 8) & 0xff);
    assertUpperEnabled = true;
}

void Register::resetAssertUpper(void)
{
    if (upperAssertLed != nullptr) {
        upperAssertLed->setState(KLed::Off);
        upperAssertLed->repaint();
    }

    bb->getBus()->setDefault();
    assertUpperEnabled = false;
}

void Register::assertLower(void)
{
    if (lowerAssertLed != nullptr) {
        lowerAssertLed->setState(KLed::On);
        lowerAssertLed->repaint();
    }

    bb->getBus()->setValue(value & 0xff);
    assertLowerEnabled = true;
}

void Register::resetAssertLower(void)
{
    if (lowerAssertLed != nullptr) {
        lowerAssertLed->setState(KLed::Off);
        lowerAssertLed->repaint();
    }

    bb->getBus()->setDefault();
    assertLowerEnabled = false;
}

void Register::readUpper(void)
{
    if (upperReadLed != nullptr) {
        upperReadLed->setState(KLed::On);
        upperReadLed->repaint();
    }

    readUpperEnabled = true;
}

void Register::resetReadUpper(void)
{
    if (upperReadLed != nullptr) {
        upperReadLed->setState(KLed::Off);
        upperReadLed->repaint();
    }

    bb->getBus()->setDefault();
    readUpperEnabled = false;
}

void Register::readLower(void)
{
    if (lowerReadLed != nullptr) {
        lowerReadLed->setState(KLed::On);
        lowerReadLed->repaint();
    }

    readLowerEnabled = true;
}

void Register::resetReadLower(void)
{
    if (lowerReadLed != nullptr) {
        lowerReadLed->setState(KLed::Off);
        lowerReadLed->repaint();
    }

    readLowerEnabled = false;
}


void Register::clear(void)
{
    value = 0;
    updateRegister();
}


void Register::updateRegister(void)
{
    if (bit15Led != nullptr) {
        bit15Led->setState((value & 0x8000) != 0 ? KLed::On : KLed::Off);
        bit15Led->repaint();
    }

    if (bit14Led != nullptr) {
        bit14Led->setState((value & 0x4000) != 0 ? KLed::On : KLed::Off);
        bit14Led->repaint();
    }

    if (bit13Led != nullptr) {
        bit13Led->setState((value & 0x2000) != 0 ? KLed::On : KLed::Off);
        bit13Led->repaint();
    }

    if (bit12Led != nullptr) {
        bit12Led->setState((value & 0x1000) != 0 ? KLed::On : KLed::Off);
        bit12Led->repaint();
    }

    if (bit11Led != nullptr) {
        bit11Led->setState((value & 0x0800) != 0 ? KLed::On : KLed::Off);
        bit11Led->repaint();
    }

    if (bit10Led != nullptr) {
        bit10Led->setState((value & 0x0400) != 0 ? KLed::On : KLed::Off);
        bit10Led->repaint();
    }

    if (bit9Led != nullptr) {
        bit9Led->setState((value & 0x0200) != 0 ? KLed::On : KLed::Off);
        bit9Led->repaint();
    }

    if (bit8Led != nullptr) {
        bit8Led->setState((value & 0x0100) != 0 ? KLed::On : KLed::Off);
        bit8Led->repaint();
    }

    if (bit7Led != nullptr) {
        bit7Led->setState((value & 0x0080) != 0 ? KLed::On : KLed::Off);
        bit7Led->repaint();
    }

    if (bit6Led != nullptr) {
        bit6Led->setState((value & 0x0040) != 0 ? KLed::On : KLed::Off);
        bit6Led->repaint();
    }

    if (bit5Led != nullptr) {
        bit5Led->setState((value & 0x0020) != 0 ? KLed::On : KLed::Off);
        bit5Led->repaint();
    }

    if (bit4Led != nullptr) {
        bit4Led->setState((value & 0x0010) != 0 ? KLed::On : KLed::Off);
        bit4Led->repaint();
    }

    if (bit3Led != nullptr) {
        bit3Led->setState((value & 0x0008) != 0 ? KLed::On : KLed::Off);
        bit3Led->repaint();
    }

    if (bit2Led != nullptr) {
        bit2Led->setState((value & 0x0004) != 0 ? KLed::On : KLed::Off);
        bit2Led->repaint();
    }

    if (bit1Led != nullptr) {
        bit1Led->setState((value & 0x0002) != 0 ? KLed::On : KLed::Off);
        bit1Led->repaint();
    }

    if (bit0Led != nullptr) {
        bit0Led->setState((value & 0x0001) != 0 ? KLed::On : KLed::Off);
        bit0Led->repaint();
    }

}

