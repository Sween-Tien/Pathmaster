#include "Pathmaster.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Pathmaster w;
    w.show();
    return a.exec();
}
