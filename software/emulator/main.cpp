#include "bbcomputer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BBComputer w;
    w.show();
    return a.exec();
}
