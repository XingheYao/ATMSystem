#include "cashdispenser.h"
#include "mainwindow.h"
CashDispenser::CashDispenser(Log log)
    :_log(log)
{
    _cashOnHand =  Money(0);
}


void CashDispenser::setInitialCash(Money initialCash)
{
    _cashOnHand = initialCash;
    cout<< "_cashOnHand = " <<_cashOnHand.toString()<<endl;
}

bool CashDispenser::checkCashOnHand(Money amount)
{
    cout<< "_cashOnHand = " <<_cashOnHand.toString()<<endl;
    return amount.lessEqual( _cashOnHand);
}

void CashDispenser::dispenseCash(Money amount)
{
    _cashOnHand.subtract(amount);
    cout<< "_cashOnHand = " <<_cashOnHand.toString()<<endl;
    MainWindow & w1=get_global<MainWindow>();
    w1.dispenseCash(amount);

    // Log cash having been dispensed

    _log.logCashDispensed(amount);
}


