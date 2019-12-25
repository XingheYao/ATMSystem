#include "message.h"
Message::Message(int messageCode, Card card, int pin, int serialNumber, int fromAccount, int toAccount, Money amount)
    :_messageCode(messageCode)
    , _card(card)
    ,_pin(pin)
    ,_serialNumber(serialNumber)
    ,_fromAccount(fromAccount)
    ,_toAccount(toAccount)
    ,_amount(amount)
{}


string Message::toString()
{
    string result=" ";
    Money zeroAccount(0);
    switch (_messageCode)
    {
        case WITHDRAWAL:

            result = result + "WITHDRAW";
            break;

        case INITIATE_DEPOSIT:

            result = result + "INIT_DEP";
            break;

        case COMPLETE_DEPOSIT:

            result = result + "COMP_DEP";
            break;

        case TRANSFER:

            result = result + "TRANSFER";
            break;

        case INQUIRY:

            result = result + "INQUIRY ";
            break;
    }

    result = result + " CARD# " + to_string(_card.getNumber());
    result = result + " TRANS# " + to_string(_serialNumber);
    if (_fromAccount >= 0)
        result = result + " FROM  " + to_string(_fromAccount);
    else
        result = result + " NO FROM";
    if (_toAccount >= 0)
        result = result + " TO  " + to_string(_toAccount) ;
    else
        result += " NO TO";
    if (! _amount.lessEqual(zeroAccount))
        result = result + " " + _amount.toString();
    else
        result = result + " NO AMOUNT";

    return result;
}

