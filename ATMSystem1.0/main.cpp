#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow & w=get_global<MainWindow>();
    //w.setWindowTitle("ATM系统");
    w.show();

//     ATM atm(42, "Gordon College", "First National Bank of Podunk");
//     atm.run();
    return a.exec();
}
