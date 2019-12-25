#ifndef CARDREADER_H
#define CARDREADER_H
#include "card.h"
//#include "mainwindow.h"
class ATM;
class CardReader
{
    /*进行读卡模拟 */

public:

    CardReader(/*ATM atm*/);

    Card* readCard();
    //int readCardNumber(QString);
    //QString readCardPassword(QString);
    void ejectCard();
    void retainCard();

    //ATM* _atm;
};

#endif // CARDREADER_H
