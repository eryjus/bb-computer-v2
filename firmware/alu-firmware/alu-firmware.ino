/*****************************************************************************************************************//**
* @file         alu-firmware
* @brief        EEPROM Programmer for the ALU firmware
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
* This code will program the EEPROM specifically for the ALU Control Logic.  So, what control logic would be needed 
* for an ALU?  Well, the there are 4 control signals, 1 mode signal, and 1 carry bit which all need to be controlled 
* whittled down to 16 ALU-related microcode instructions.  Each of the 16 instructions would have to set the 6 
* control lines properly for the instruction and determine if the incoming and outgoing carry bit need to be inverted 
* before and after the operation.
* 
* Overall, I will be able to handle all this with 4 lines from the Control Module rather than 6 lines and lots of 
* additional logic gates.  I think this is a win.
* 
* The 16 microcode operations will be:
* * NOT
* * AND
* * NAND
* * OR
* * NOR
* * XOR
* * XNOR
* * FALSE
* * TRUE
* * ADD
* * SUB
* * SHL (1-bit)
* * INC
* * DEC
* * ADC
* * SBB
* 
* Each will have its own set of the 4 control lines for the ALU with its mode line set (32 possibilities) and several
* will have a need to invert the carry bit, which will be included as an input and output as mentioned above.  So,
* we can represent the 64 theoretical possibilities in the 16 supported instructions.
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
* 
* In this case we are programming the ALU firmware.  We will have 5 input lines (address lines) for the 4 control
* lines and 1 carry bit.  Output will be 4 control lines, 1 mode line, and 1 carry inversion control signal.
* 
* Now for orgainization:
* - The lowest 4 bits of the address will handle the 4 control lines coming from the Control Module.
* - The next highest bit (1<<4) will handle the carry-bit input, meaning that most of the lower 16 bytes and next 
*   16 bytes will remain the same.
* - The remaining 3 bits will remain unused and will be assumed to be 0, leaving that section of the EEPROM 
*   unprogrammed.
*///==================================================================================================================


//
// -- Some constants for interacting with the Control Module (lower 4 bits of the address):
//    -------------------------------------------------------------------------------------
#define   NOT   0b0000
#define   AND   0b0001
#define   NAND  0b0010
#define   OR    0b0011
#define   NOR   0b0100
#define   XOR   0b0101
#define   XNOR  0b0110
#define   FALSE 0b0111
#define   TRUE  0b1000
#define   ADD   0b1001
#define   SUB   0b1010
#define   SHL   0b1011
#define   INC   0b1100
#define   DEC   0b1101
#define   ADC   0b1110
#define   SBB   0b1111


//
// -- Now, define the input Carry Flag
//    --------------------------------
#define   CF    0b10000


//
// -- These are the output controls as part of the resulting byte of data
//    -------------------------------------------------------------------
#define   S3    0b01000000
#define   S2    0b00100000
#define   S1    0b00010000
#define   S0    0b00001000
#define   M     0b00000100
#define   CINV  0b00000010
#define   CFLG  0b00000001

//
// -- Here are the 16 basic control words needed for the ALU
//    ------------------------------------------------------
byte aluControl[] = {
  M|CINV,               // 0b00000 - NOT
  S3|S1|S0|M,           // 0b00001 - AND
  S2|M|CINV,            // 0b00010 - NAND
  S3|S2|S1|M,           // 0b00011 - OR
  S0|M|CINV,            // 0b00100 - NOR
  S2|S1|M|CINV,         // 0b00101 - XOR
  S3|S0|M,              // 0b00110 - XNOR
  S1|S0|M,              // 0b00111 - FALSE
  S3|S2|M,              // 0b01000 - TRUE
  S3|S0|CFLG|CINV,      // 0b01001 - ADD
  S2|S1,                // 0b01010 - SUB
  S3|S2|CFLG|CINV,      // 0b01011 - SHL
  0,                    // 0b01100 - INC
  S3|S2|S1|S0|CFLG,     // 0b01101 - DEC
  S3|S0|CFLG|CINV,      // 0b01110 - ADC
  S2|S1,                // 0b01111 - SBB
  M|CINV,               // 0b10000 - NOT with CF
  S3|S1|S0|M,           // 0b10001 - AND with CF
  S2|M|CINV,            // 0b10010 - NAND with CF
  S3|S2|S1|M,           // 0b10011 - OR with CF
  S0|M|CINV,            // 0b10100 - NOR with CF
  S2|S1|M|CINV,         // 0b10101 - XOR with CF
  S3|S0|M,              // 0b10110 - XNOR with CF
  S1|S0|M,              // 0b10111 - FALSE with CF
  S3|S2|M,              // 0b11000 - TRUE with CF
  S3|S0|CFLG|CINV,      // 0b11001 - ADD with CF
  S2|S1,                // 0b11010 - SUB with CF
  S3|S2|CFLG|CINV,      // 0b11011 - SHL with CF
  0,                    // 0b11100 - INC with CF
  S3|S2|S1|S0|CFLG,     // 0b11101 - DEC with CF
  S3|S0|CINV,           // 0b11110 - ADC with CF
  S2|S1|CFLG,           // 0b11111 - SBB with CF
};


//
// -- Complete the EEPROM programming
//    -------------------------------
void programEeprom() {
  for (int addr = 0; addr < 32; addr ++) {
    writeEeprom(addr, aluControl[addr]);
  }
}


//
// -- Standard Setup function
//    -----------------------
void setup() {
  Serial.begin(57600);
  Serial.println("Welcome to the ALU-Control-Logic EEPROM Programmer");
  Serial.println("  This software will program the firmware to control the ALU");

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
