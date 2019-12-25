#ifndef BALANCES_H
#define BALANCES_H
#include "money.h"

class Balances
{
public:
    Balances()
    {}

    void setBalances(Money total, Money available)
    {
        _total=total;
        _available=available;
    }

    Money getTotal(){   return _total;}
    Money getAvailable(){   return _available;}


private:
    Money _total;
    Money _available;
};


#endif // BALANCES_H
