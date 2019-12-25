#include "transaction.h"
#include <synchapi.h>

const vector<string>  AccountInformation::ACCOUNT_NAMES ={ "Checking", "Savings", "Money Market" };
const vector<string>  AccountInformation::ACCOUNT_ABBREVIATIONS ={ "CHKG", "SVGS", "MMKT" };

//
int Transaction::_nextSerialNumber = 1;
const vector<string>  Transaction::TRANSACTION_TYPES_MENU =
            { "Withdrawal", "Deposit", "Transfer", "Balance Inquiry" };
const int Transaction::GETTING_SPECIFICS_STATE = 1;
const int Transaction::SENDING_TO_BANK_STATE = 2;
const int Transaction::INVALID_PIN_STATE = 3;
const int Transaction::COMPLETING_TRANSACTION_STATE = 4;
const int Transaction::PRINTING_RECEIPT_STATE = 5;
const int Transaction::ASKING_DO_ANOTHER_STATE = 6;



Transaction::Transaction( Session session, Card card, int pin)
    : _atm(_atm->get_instance())
    , _session(session)
    , _card(card)
    , _pin(pin)
{
//    _nextSerialNumber=1;
    _serialNumber=_nextSerialNumber++;
    _balances = new  Balances();
    _state = GETTING_SPECIFICS_STATE;
    _message=nullptr;
}
Transaction::~Transaction()
{
    if(_message)
    {
        delete _message;
    }
}

Message* Transaction::getSpecificsFromCustomer() throw (Cancelled)
// 创建交易的会话；用于ATM与客户；与它通信的银行来执行工作
{
    std::cout<<"Transaction::getSpecificsFromCustomer"<<std::endl;
    return nullptr;
}

Receipt Transaction::completeTransaction() throw (Cancelled)
{
    std::cout<<"Transaction::completeTransaction"<<std::endl;
    return Receipt();
}


Transaction* Transaction::makeTransaction(ATM* atm, Session session,Card card, int pin) throw(Cancelled)
{
    Message message(Message::INQUIRY,
                    card, pin, -1, -1, -1, Money(0));
    Status *status = atm->getNetworkToBank()->sendMessage(&message);
    Transaction* transaction=new Transaction(session, card, pin);
    if (!status->isSuccess())
    {
        try
        {
            if(status->isInvalidPIN())
            {
                status = transaction->performInvalidPINExtension(pin);
            }
            else
            {
                atm->getCustomerConsole()->clearDisplay();
                atm->getCustomerConsole()->display("Invalid Card");
                QThread::sleep(2);
                throw Cancelled();
            }
            delete status;

        }
        catch(CardRetained e)
        {
            cout<<"CardRetained e: "<<e.what()<<endl;
            throw Cancelled();
            delete transaction;
            return nullptr;
        }
        // If customer repeatedly enters invalid PIN's, a
        // CardRetained exception is thrown, and this method
        // terminates

    }
    int choice = atm->getCustomerConsole()->readMenuChoice(
                    "Please choose transaction type", TRANSACTION_TYPES_MENU);
    switch(choice)
    {
        case 0:

            return new Withdrawal( session, card, pin);

        case 1:

            return new Deposit( session, card, pin);

        case 2:

            return new Transfer( session, card, pin);

        case 3:

            return new Inquiry( session, card, pin);

        default:

            return nullptr;    // To keep compiler happy - should not happen!
    }
}
bool Transaction::performTransaction() throw (CardRetained)
{
    string doAnotherMessage =" ";
    Status *status ;
    Receipt receipt;

    while (true)    // Terminates by return in ASKING_DO_ANOTHER_STATE or exception
    {
        switch(_state)
        {
            case GETTING_SPECIFICS_STATE:

                try
                {
                    _message = getSpecificsFromCustomer();
                   // _atm->getCustomerConsole()->display("GETTING_SPECIFICS_STATE");
                    _state = SENDING_TO_BANK_STATE;
                }
                catch(Cancelled e)
                {
                    doAnotherMessage = "Last transaction was cancelled";
                    _state = ASKING_DO_ANOTHER_STATE;
                }

                break;

            case SENDING_TO_BANK_STATE:

                status = _atm->getNetworkToBank()->sendMessage(_message, _balances);
                cout<< "_atm->getNetworkToBank()->sendMessage" <<endl;
                if (status->isInvalidPIN())
                    _state = INVALID_PIN_STATE;
                else if (status->isSuccess())
                    _state = COMPLETING_TRANSACTION_STATE;
                else
                {
                    doAnotherMessage = status->getMessage();
                    _state = ASKING_DO_ANOTHER_STATE;
                }
                delete status;
                break;

            case INVALID_PIN_STATE:

                try
                {
                    status = performInvalidPINExtension(_pin);

                    // If customer repeatedly enters invalid PIN's, a
                    // CardRetained exception is thrown, and this method
                    // terminates

                    if (status->isSuccess())
                        _state = COMPLETING_TRANSACTION_STATE;
                    else
                    {
                        doAnotherMessage = status->getMessage();
                        _state = ASKING_DO_ANOTHER_STATE;
                    }
                }
                catch(Cancelled e)
                {
                    doAnotherMessage = "Last transaction was cancelled";
                    _state = ASKING_DO_ANOTHER_STATE;
                }
                delete status;
                break;

            case COMPLETING_TRANSACTION_STATE:

                try
                {
                     receipt = completeTransaction();
                    _state = PRINTING_RECEIPT_STATE;
                }
                catch(Cancelled e)
                {
                    doAnotherMessage = "Last transaction was cancelled";
                    _state = ASKING_DO_ANOTHER_STATE;
                }

                break;

            case PRINTING_RECEIPT_STATE:

                _atm->getReceiptPrinter()->print(receipt);
                _state = ASKING_DO_ANOTHER_STATE;

                break;

            case ASKING_DO_ANOTHER_STATE:

                if (doAnotherMessage.length() > 0)
                    doAnotherMessage += "\n";

                try
                {
                    vector<string> yesNoMenu  = { "Yes", "No" };

                    bool doAgain = _atm->getCustomerConsole()->readMenuChoice(
                        doAnotherMessage +
                        "Would you like to do another transaction?",
                        yesNoMenu) == 0;
                    return doAgain;
                }
                catch(Cancelled e)
                {
                    return false;
                }
        }
    }
}

Status* Transaction::performInvalidPINExtension(int &pin)throw (Cancelled,CardRetained)
{
    Status* status;
    for (int i = 0; i < 3; i ++)
    {
        _atm->getCustomerConsole()->clearDisplay();
        _pin = _atm->getCustomerConsole()->readPIN(
            "PIN was incorrect\nPlease re-enter your PIN\nThen press ENTER");
        if(_message==nullptr)
        {
            Message * message=new Message(Message::INQUIRY,_card, _pin, -1, -1, -1, Money(0));
            message->setPIN(_pin);
            status = _atm->getNetworkToBank()->sendMessage(message);
        }
        else
        {
            _message->setPIN(_pin);
            status = _atm->getNetworkToBank()->sendMessage(_message);
        }
        if (! status->isInvalidPIN())
        {
            _session.setPIN(_pin);
            return status;
        }
    }
    pin=_pin;
    _atm->getCardReader()->retainCard();
    _atm->getCustomerConsole()->display(
        "Your card has been retained\nPlease contact the bank.");
    try
    {
        Sleep(5000);
    }
    catch(exception e)
    { }
    _atm->getCustomerConsole()->display("");

    throw  CardRetained();
}

Withdrawal::Withdrawal( Session session, Card card, int pin)
    :Transaction( session, card, pin)
{}

//
Message* Withdrawal::getSpecificsFromCustomer() throw (Cancelled)
{
    _from = _atm->getCustomerConsole()->readMenuChoice(
            "Account to withdraw from",
            AccountInformation::ACCOUNT_NAMES);

    vector<string>  amountOptions = { "$20", "$40", "$60", "$100", "$200" };
    Money  amountValues []= {
                               Money(20),  Money(40),  Money(60),
                               Money(100),  Money(200)
                            };

    string amountMessage = " ";
    bool validAmount = false;

    while (! validAmount)
    {
        _amount = amountValues [
            _atm->getCustomerConsole()->readMenuChoice(
                amountMessage + "Amount of cash to withdraw", amountOptions) ];

        validAmount = _atm->getCashDispenser()->checkCashOnHand(_amount);

        if (! validAmount)
            amountMessage = "Insufficient cash available\n";
    }

    return  new Message(Message::WITHDRAWAL,
                       _card, _pin, _serialNumber, _from, -1, _amount);
}

Receipt Withdrawal::completeTransaction()throw (Cancelled)
{
    _atm->getCashDispenser()->dispenseCash(_amount);
    Receipt receipt(_atm, _card, this, _balances);
    receipt._detailsPortion.resize(2);
    receipt._detailsPortion.push_back( "WITHDRAWAL FROM: " +
                        AccountInformation::ACCOUNT_ABBREVIATIONS[_from]);
    receipt._detailsPortion.push_back("AMOUNT: " + _amount.toString()) ;
    return receipt;

}

Deposit::Deposit( Session session, Card card, int pin)
    :Transaction( session, card, pin)
{}

Message* Deposit::getSpecificsFromCustomer() throw (Cancelled)
{
    _to = _atm->getCustomerConsole()->readMenuChoice(
        "Account to deposit to",
        AccountInformation::ACCOUNT_NAMES);

    _amount = _atm->getCustomerConsole()->readAmount("Amount to deposit");

    return  new Message(Message::INITIATE_DEPOSIT,
                       _card, _pin, _serialNumber, -1, _to, _amount);
}

Receipt Deposit::completeTransaction()throw (Cancelled)
{
    _atm->getEnvelopeAcceptor()->acceptEnvelope();
            Status* status = _atm->getNetworkToBank()->sendMessage(
                 new Message(Message::COMPLETE_DEPOSIT,
                            _card, _pin, _serialNumber, -1, _to, _amount),
                _balances);
    Receipt receipt(_atm, _card, this, _balances);
    receipt._detailsPortion.resize(2);
    receipt._detailsPortion.push_back( "DEPOSIT TO: " +
                        AccountInformation::ACCOUNT_ABBREVIATIONS[_to]);
    receipt._detailsPortion.push_back( "AMOUNT: " + _amount.toString());
    return receipt;

}
Transfer::Transfer( Session session, Card card, int pin)
    :Transaction( session, card, pin)
{}
Message* Transfer::getSpecificsFromCustomer() throw (Cancelled)
{
    _from = _atm->getCustomerConsole()->readMenuChoice(
                "Account to transfer from",
                AccountInformation::ACCOUNT_NAMES);
    _to = _atm->getCustomerConsole()->readMenuChoice(
        "Account to transfer to",
        AccountInformation::ACCOUNT_NAMES);

    _amount = _atm->getCustomerConsole()->readAmount("Amount to transfer");

    return new Message(Message::TRANSFER,
                    _card, _pin, _serialNumber, _from, _to, _amount);

}
Receipt Transfer::completeTransaction()throw (Cancelled)
{
    Receipt receipt(_atm, _card, this, _balances);
    receipt._detailsPortion.resize(2);
    receipt._detailsPortion.push_back( "TRANSFER FROM: " +
            AccountInformation::ACCOUNT_ABBREVIATIONS[_from] +
            " TO: " +  AccountInformation::ACCOUNT_ABBREVIATIONS[_to]) ;
    receipt._detailsPortion.push_back(  "AMOUNT: " + _amount.toString());
    return receipt;

}

Inquiry::Inquiry( Session session, Card card, int pin)
    :Transaction( session, card, pin)
{}
Receipt Inquiry::completeTransaction()throw (Cancelled)
{
    Receipt receipt(_atm, _card, this, _balances);
    receipt._detailsPortion.resize(2);
    receipt._detailsPortion.push_back("INQUIRY FROM: " +
                        AccountInformation::ACCOUNT_ABBREVIATIONS[_from]);
    receipt._detailsPortion.push_back( "");
    return receipt;

}
Message* Inquiry::getSpecificsFromCustomer() throw (Cancelled)
{
    _from = _atm->getCustomerConsole()->readMenuChoice(
                "Account to inquire from",
                AccountInformation::ACCOUNT_NAMES);
    return new Message(Message::INQUIRY,
                    _card, _pin, _serialNumber, _from, -1, Money(0));
}
