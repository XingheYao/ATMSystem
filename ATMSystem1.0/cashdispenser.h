#ifndef CASHDISPENSER_H
#define CASHDISPENSER_H
#include "money.h"
#include "log.h"

class CashDispenser
{
public:
    CashDispenser(Log log);
    void setInitialCash(Money initialCash);
    bool checkCashOnHand(Money amount);
    void dispenseCash(Money amount);

private:
    Log _log;
    Money _cashOnHand;
};

#endif // CASHDISPENSER_H
