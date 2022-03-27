#ifndef TEMPREG1_H
#define TEMPREG1_H

class TempRegister1;


#include "register.h"


class TempRegister1 : public Register {
    Q_OBJECT


public:
    TempRegister1(BBComputer *bb);
    virtual ~TempRegister1() {}
};

#include "bbcomputer.h"



#endif // TEMPREG1_H
