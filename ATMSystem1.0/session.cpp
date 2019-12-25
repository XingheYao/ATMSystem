#include "session.h"
#include "transaction.h"
#include <QThread>
Session::Session()
    :_atm(_atm->get_instance()),
    _state(READING_CARD_STATE)
{ }

void Session::performSession()//
{
    Card* card=nullptr ;
    Transaction* currentTransaction;

    while (_state != FINAL_STATE)
    {
        switch(_state)
        {
            case READING_CARD_STATE:

                card = _atm->getCardReader()->readCard();

                if (card != nullptr)
                {
                    //cout<<"Here is runned"<<endl;
                    _state = READING_PIN_STATE;
                }
                else
                {
                    _atm->getCustomerConsole()->clearDisplay();
                    _atm->getCustomerConsole()->display("Unable to read card");
                    QThread::sleep(2);
                    _state = EJECTING_CARD_STATE;
                }
                break;

            case READING_PIN_STATE:

                try
                {
                    _atm->getCustomerConsole()->clearDisplay();
                    _pin = _atm->getCustomerConsole()->readPIN(
                        "Please enter your PIN\nThen press ENTER");
                    cout <<"PIN = "<< _pin <<endl;

                    _state = CHOOSING_TRANSACTION_STATE ;
                    if(_pin==-9999)
                    {
                        _state = EJECTING_CARD_STATE;
                    }
                        //QThread::sleep(3);
//                    _message->setPIN(_pin);
//                    Status status = _atm->getNetworkToBank()->sendMessage(*_message, _balances);
//                    if (! status.isInvalidPIN())
//                    {
//                        _session.setPIN(_pin);
//                        return status;
//                    }
                }
                catch(Cancelled& e)
                {
                    cout<<"session READING_PIN_STATE error"<<endl;
                    _state = EJECTING_CARD_STATE;
                }
                break;

            case CHOOSING_TRANSACTION_STATE:

                try
                {
                    currentTransaction =
                        Transaction::makeTransaction(_atm, *this, *card, _pin);
                    _state = PERFORMING_TRANSACTION_STATE;
                }
                catch(Cancelled &e)
                {
                    _state = EJECTING_CARD_STATE;
                }
                break;

            case PERFORMING_TRANSACTION_STATE:

                try
                {
                    bool doAgain =
                        currentTransaction->performTransaction();
                    if (doAgain)
                        _state = CHOOSING_TRANSACTION_STATE;
                    else
                        _state = EJECTING_CARD_STATE;
                }
                catch(CardRetained &e)
                {
                    _state = FINAL_STATE;
                }
                break;

            case EJECTING_CARD_STATE:

                _atm->getCardReader()->ejectCard();
                _atm->retainCardSignal();
                _state = FINAL_STATE;
                break;
        }
    }
}

//

void Session::setPIN(int pin)
{
     _pin=pin;
}
