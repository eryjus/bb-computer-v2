#ifndef REGISTER_H
#define REGISTER_H


class BBComputer;

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class BBComputer; }
QT_END_NAMESPACE



#include "kled.h"

#include <QObject>


class Register : public QObject {
    Q_OBJECT

protected:
    BBComputer *bb;
    Ui::BBComputer *ui;
    uint16_t value;
    bool readUpperEnabled;
    bool readLowerEnabled;
    bool assertUpperEnabled;
    bool assertLowerEnabled;

    KLed *upperAssertLed;
    KLed *lowerAssertLed;
    KLed *upperReadLed;
    KLed *lowerReadLed;
    KLed *bit15Led;
    KLed *bit14Led;
    KLed *bit13Led;
    KLed *bit12Led;
    KLed *bit11Led;
    KLed *bit10Led;
    KLed *bit9Led;
    KLed *bit8Led;
    KLed *bit7Led;
    KLed *bit6Led;
    KLed *bit5Led;
    KLed *bit4Led;
    KLed *bit3Led;
    KLed *bit2Led;
    KLed *bit1Led;
    KLed *bit0Led;


protected:
    void updateRegister(void);


public slots:
    virtual void updateUpper(void);
    virtual void updateLower(void);
    virtual void clockTick(void);
    virtual void assertUpper(void);
    virtual void resetAssertUpper(void);
    virtual void assertLower(void);
    virtual void resetAssertLower(void);
    virtual void readUpper(void);
    virtual void resetReadUpper(void);
    virtual void readLower(void);
    virtual void resetReadLower(void);
    virtual void clear(void);


public:
    Register(BBComputer *bb);
    virtual ~Register() {}
};


#include "bbcomputer.h"


#endif // REGISTER_H
