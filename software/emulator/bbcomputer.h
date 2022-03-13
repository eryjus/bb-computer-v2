#ifndef BBCOMPUTER_H
#define BBCOMPUTER_H

class ClockModule;

#include "clock-module.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class BBComputer; }
QT_END_NAMESPACE

class BBComputer : public QMainWindow
{
    Q_OBJECT


private:
    ClockModule *clock;

public:
    BBComputer(QWidget *parent = nullptr);
    ~BBComputer();

private:
    Ui::BBComputer *ui;
};
#endif // BBCOMPUTER_H
