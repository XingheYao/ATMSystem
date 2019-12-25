#include "receipt.h"
#include "atm.h"
#include "transaction.h"
#include <time.h>
Receipt::Receipt()
{

}
Receipt::Receipt( ATM *atm,Card card, Transaction* transaction, Balances* balances)
{
    // Heading portion of the receipt

    _headingPortion.clear();
    _headingPortion.resize(4);
    time_t date = time(nullptr);
    _headingPortion.push_back(ctime(&date)) ;
    _headingPortion.push_back( atm->getBankName());
    _headingPortion.push_back( "ATM #" + to_string(atm->getID()) + " " + atm->getPlace());
    _headingPortion.push_back( "CARD " + to_string(card.getNumber()) +
                 " TRANS #" + to_string(transaction->getSerialNumber()));



    _balancesPortion.clear();
    _balancesPortion.resize(2);
    _balancesPortion.push_back( "TOTAL BAL: " + balances->getTotal().toString());
    _balancesPortion.push_back( "AVAILABLE: " + balances->getAvailable().toString());
}

bool Receipt::hasMoreElements()
{
    return _portion<2 || _index < _balancesPortion.size();
}

string Receipt::nextElement()
{
    string line ;
    switch(_portion)
    {
        case 0:
            line = Receipt::_headingPortion[_index ++];
            if (_index >= _headingPortion.size())
            {
                _portion ++;
                _index = 0;
            }
            break;
        case 1:
            line = Receipt::_detailsPortion[_index ++];
            if (_index >= _detailsPortion.size())
            {
                _portion ++;
                _index = 0;
            }
            break;
        case 2:
            line = Receipt::_balancesPortion[_index ++];
            break;

    }
    return line;
}


Receipt::~Receipt()
{
//    if(_detailsPortion.empty())
//        delete _detailsPortion;
//    if(_headingPortion.empty())
//        delete  _headingPortion;
//    if(_balancesPortion.empty())
//        delete  _balancesPortion;
}
