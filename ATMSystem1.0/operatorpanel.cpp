#include "operatorpanel.h"
#include "mainwindow.h"

Money OperatorPanel::getInitialCash()
{
    MainWindow & w1=get_global<MainWindow>();
    Money InitialCash=w1.getInitialCash();
    std::cout<<"InitialCash = "<<InitialCash.toString()<<std::endl;
    return InitialCash;
}
