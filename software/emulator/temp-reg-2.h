#ifndef TEMPREG2_H
#define TEMPREG2_H

class TempRegister2;


#include "register.h"


class TempRegister2 : public Register {
    Q_OBJECT


public:
    TempRegister2(BBComputer *bb);
    virtual ~TempRegister2() {}
};

#include "bbcomputer.h"



#endif // TEMPREG2_H
