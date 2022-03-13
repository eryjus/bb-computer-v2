#include "bbcomputer.h"
#include "ui_bbcomputer.h"

BBComputer::BBComputer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BBComputer)
{
    ui->setupUi(this);
    clock = new ClockModule(ui);
}

BBComputer::~BBComputer()
{
    delete ui;
}

