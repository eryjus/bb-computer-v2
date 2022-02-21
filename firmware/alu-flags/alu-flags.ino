/*****************************************************************************************************************//**
* @file         alu-flags
* @brief        EEPROM Programmer for the ALU flags determination firmware
* @author       Adam Clark
*
* This code is adapted from the videos provided by Ben Eater.  It is  not a direct line-for-line copy, but I cannot
* claim the algorithms as my own.  I watched the videos and understood the point and then "wrote" a version of what
* I just watched.
*
* If this code is used anywhere please give Ben honorable mention.
*
* -------------------------------------------------------------------------------------------------------------------
*
* This code will perform the selection of the `P` (Parity), `Z` (Zero), and `V` Overflow bits for both the upper
* and lower ALU results bytes.  The whole purpose of this firmware is to eliminate quite a few logic gates to do
* this determination.  Since the results are predictable based on the results and 2 other inputs, it makes sense
* to do this simplification.
*
* The inputs here are the following:
* 0-7: The ALU Results, with bit 0 being the least significant bit
* 8: The lower (0) and upper (1) ALU result indicator
* 9: A15 input line
* 10: B15 input line
*
* `Z` is set only when the input[0-7] are all 0.  The results of the upper and lower byte will be ANDed together
* off-EEPROM to get the aggregate `Z` Flag.
*
* `P` needs to be determined by taking each bit in input[0-7] and XORing them together such that:
* `((i[0] ^ i[1]) ^ (i[2] ^ i[3])) ^ ((i[4] ^ i[5]) ^ (i[6] ^ i[7]))`
*
* The results of the upper and lower byte will be XORed together off-EEPROM to get the aggregate `P` Flag.
*
* The `V` flag will be determined by only in the upper EEPROM and the results will be ignored in the lower EEPROM.
* The log used to determine when to set the `V` flag is if either condition is met:
* * If `(((i[9] == 0 && i[10] == 0) && i[7] == 1) && i[8] == 1)`
* * If `(((i[9] == 1 && i[10] == 1) && i[7] == 0) && i[8] == 1)`
*
* In all other cases, the `V` flag will be 0.
*
*///==================================================================================================================



/*****************************************************************************************************************//**
* The following section is the common code used to program the EEPROM.  This section will be repeated with each
* separate piece of firmware (at least unless/until I can figure out how to tell the Arduino IDE to import something
* from another folder).
*
* In the meantime, I expect to have to do multiple-maintenance on anything in this section.
*///==================================================================================================================



//
// -- Define some constants that will be used to interact with the Arduino
//    --------------------------------------------------------------------
#define PIN_DATA    2
#define PIN_CLOCK   3
#define PIN_LATCH   4

#define DATA0       5
#define DATA7       12
#define WRITE_EN    13


//
// -- Set an address to be read from/written to
//    -----------------------------------------
void setAddress(int address, bool outputEnable) {
    address &= 0x7ff;       // safety first -- make sure the address is only 11 bits wide
    digitalWrite(WRITE_EN, HIGH);
    delay(1);

    shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
    shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, address & 0xff);

    delay(1);

    digitalWrite(PIN_LATCH, LOW);
    digitalWrite(PIN_LATCH, HIGH);
    digitalWrite(PIN_LATCH, LOW);

    delay(1);
}


//
// -- Write a single byte of data to an address in the EEPROM
//    -------------------------------------------------------
void writeEeprom(int address, byte data) {
    setAddress(address, false);

    for (int pin = DATA0; pin <= DATA7; pin ++) {
        pinMode(pin, OUTPUT);
    }

    for (int pin = DATA0; pin <= DATA7; pin ++) {
        digitalWrite(pin, data & 1);
        data = data >> 1;
    }

    digitalWrite(WRITE_EN, LOW);
    delayMicroseconds(1);
    digitalWrite(WRITE_EN, HIGH);
    delay(30);
}



//
// -- Read a single byte from an address on the EEPROM
//    ------------------------------------------------
byte readEeprom(int address) {
  setAddress(address, true);

    for (int pin = DATA7; pin >= DATA0; pin --) {
        pinMode(pin, INPUT);
    }

    byte data = 0;

    for (int pin = DATA7; pin >= DATA0; pin --) {
        data = (data << 1) | digitalRead(pin);
    }

    return data;
}


//
// -- Dump the contents of the entire EEPROM to the Serial Port
//    ---------------------------------------------------------
void dumpEeprom(void) {
  char buf [80];

  Serial.println();
  Serial.println("Dumping the contents of the Eeprom\n");
  Serial.println();
  Serial.println("       0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07   0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f");
  Serial.println("       ---- ---- ---- ---- ---- ---- ---- ----   ---- ---- ---- ---- ---- ---- ---- ----");

  for (int base = 0; base < 2048; base += 16) {
        sprintf(buf, "0x%03x: ", base);
        Serial.print(buf);

        for (int offset = 0; offset < 16; offset ++) {
            int addr = base + offset;
            sprintf(buf, "0x%02x ", readEeprom(addr));
            Serial.print(buf);

            if (offset == 7) Serial.print("  ");
        }

        Serial.println();
    }
}


//
// -- Erase the EEPROM back to factory (burns one of the write cycles)
//    ----------------------------------------------------------------
void eraseEeprom(void) {
    Serial.print("Erasing EEPROM");

    for (int i = 0; i < 2048; i ++) {
        writeEeprom(i, 0xff);

        if (i % 32 == 0) Serial.print(".");
    }

    Serial.println();
}



//
// -- Perform the EEPROM programmer initialization
//    --------------------------------------------
void performInitialization() {
    digitalWrite(PIN_LATCH, LOW);
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_DATA, LOW);
    digitalWrite(WRITE_EN, HIGH);

    pinMode(PIN_DATA, OUTPUT);
    pinMode(PIN_CLOCK, OUTPUT);
    pinMode(PIN_LATCH, OUTPUT);
    pinMode(WRITE_EN, OUTPUT);
}



/*****************************************************************************************************************//**
* The following section is directly the task to complete.
*///==================================================================================================================

#define ZF 0b10000000
#define PF 0b01000000
#define VF 0b00100000


//
// -- Complete the EEPROM programming
//    -------------------------------
void programEeprom() {
    for (int addr = 0; addr < 2048; addr ++) {
        byte res = addr & 0xff;
        byte bit0 = (res & 0b00000001) >> 0;
        byte bit1 = (res & 0b00000010) >> 1;
        byte bit2 = (res & 0b00000100) >> 2;
        byte bit3 = (res & 0b00001000) >> 3;
        byte bit4 = (res & 0b00010000) >> 4;
        byte bit5 = (res & 0b00100000) >> 5;
        byte bit6 = (res & 0b01000000) >> 6;
        byte bit7 = (res & 0b10000000) >> 7;
        bool upper = (addr >> 8) & 1 ? true : false;
        bool a15 = (addr >> 9) & 1 ? true : false;
        bool b15 = (addr >> 10) & 1 ? true : false;

        // -- assume no flags will be set
        byte out = 0x00;

        // -- determine the ZF
        if (res == 0) out |= ZF;

        // -- determine the PF
        if (bit0 ^ bit1 ^ bit2 ^ bit3 ^ bit4 ^ bit5 ^ bit6 ^ bit7) out |= PF;

        // -- determine the VF
        if (upper) {
            if (a15 && b15 && (bit7 == 0)) out |= VF;
            if (!a15 && !b15 && (bit7 == 1)) out |= VF;
        }

        writeEeprom(addr, out);

        if (addr % 32 == 0) Serial.print(".");
    }
}


//
// -- Standard Setup function
//    -----------------------
void setup() {
    Serial.begin(57600);
    Serial.println("Welcome to the ALU-Flags Determination EEPROM Programmer");
    Serial.println("  This software will program the firmware to determine the flag settings after an ALU operation");

    performInitialization();
    programEeprom();
    dumpEeprom();
}


//
// -- Standard post-initialization loop function
//    ------------------------------------------
void loop() {
    // -- nothing to do here; what we want to do should not be in a loop
}
