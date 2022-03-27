#ifndef BUS_H
#define BUS_H

class Bus;

#include "bbcomputer.h"
#include "ui_bbcomputer.h"

#include <QObject>


class Bus : public QObject {
    Q_OBJECT

protected:
    BBComputer *bb;
    Ui::BBComputer *ui;
    uint8_t value;

public:
    Bus(BBComputer *bb);
    virtual ~Bus() {}


protected:
    void updateValue(void);


public slots:
    void setValue(uint8_t v);
    void setDefault(void);
    uint8_t readValue(void);
};



#endif // BUS_H
