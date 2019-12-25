#ifndef MESSAGE_H
#define MESSAGE_H
#include <iostream>
#include <string>
using namespace std;

#include "card.h"
#include "money.h"

class Message
{
public:
    Message(int messageCode, Card card, int pin,
            int serialNumber, int fromAccount, int toAccount, Money amount);
    string toString();
    void setPIN(int pin)
    {
        _pin = pin;
    }
    int getMessageCode()
    {
        return _messageCode;
    }
    Card getCard()
    {
        return _card;
    }
    int getPIN()
    {
        return _pin;
    }
    int getSerialNumber()
    {
        return _serialNumber;
    }
    int getFromAccount()
    {
        return _fromAccount;
    }
    int getToAccount()
    {
        return _toAccount;
    }
    Money getAmount()
    {
        return _amount;
    }
public:
//    static const int VERIIFYPIN = 5;
    static const int WITHDRAWAL = 0;
    static const int INITIATE_DEPOSIT = 1;
    static const int COMPLETE_DEPOSIT = 2;
    static const int TRANSFER = 3;
    static const int INQUIRY = 4;
private:
    int _messageCode;
    Card _card;
    int _pin;
    int _serialNumber;
    int _fromAccount;
    int _toAccount;
    Money _amount;
};

#endif // MESSAGE_H
