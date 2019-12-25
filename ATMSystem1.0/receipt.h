#ifndef RECEIPT_H
#define RECEIPT_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;
//#include "transaction.h"
#include "card.h"
#include "balances.h"
//#include "atm.h"
class ATM;
class Transaction;
class Receipt
{

public:
    friend class Transaction;
    friend class Transfer;
    friend class Inquiry;
    friend class Deposit;
    friend class Withdrawal;
    bool hasMoreElements();

    string nextElement();
    Receipt();
    Receipt(ATM* atm,Card card, Transaction* transaction, Balances* balances);
    ~Receipt();

protected:

    vector<string> _detailsPortion;
private:

    vector<string> _headingPortion;
    vector<string> _balancesPortion;
    int _portion = 0;
    int _index = 0;

};


#endif // RECEIPT_H
