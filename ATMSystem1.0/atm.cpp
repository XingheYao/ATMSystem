
#include "status.h"
#include "money.h"
#include "session.h"
#include "atm.h"
#include "cashdispenser.h"
#include "envelopeacceptor.h"
#include "networktobank.h"
#include "mainwindow.h"
ATM::ATM(int id, string location, string bankName)
    : _id(id)
    ,_location(location)
    ,_bankName(bankName)
{

    _log =new Log();
    _cardReader =new  CardReader(/*this*/);
    _cashDispenser =new  CashDispenser(*_log);
    _customerConsole =new  CustomerConsole();
    _envelopeAcceptor = new EnvelopeAcceptor(*_log);
    _networkToBank = new NetworkToBank(*_log, "nullptr");
    _operatorPanel = new OperatorPanel(/*this*/);
    _receiptPrinter = new ReceiptPrinter();
    _state = OFF_STATE;
    _switchOn = false;
    _cardInserted = false;

}

ATM* ATM::get_instance()
{
    static ATM instance(42, "Gordon College", "First National Bank of Podunk");
    return &instance;
}

void ATM::run()
{
    Session *currentSession=nullptr ;
    while (true)
    {
        switch(_state)
        {
            case OFF_STATE:
                _customerConsole->clearDisplay();
                _customerConsole->display("Not currently available");
                Mylock();
                Mywait();
                Myunlock();
                if (_switchOn)
                {
                    performStartup();
                    _state = IDLE_STATE;
                    break;
                }



                break;

            case IDLE_STATE:
                Mylock();

                _customerConsole->clearDisplay();
                _customerConsole->display("Please insert your card");
                //sleep(1);

                _cardInserted = false;
                Mywait();
                Myunlock();

                    if (_cardInserted)
                    {

                        //cout<<"new session"<<endl;
                        currentSession = new Session(/**this*/);
                        _state = SERVING_CUSTOMER_STATE;
                        break;
                    }
                    else if (! _switchOn)
                    {
                        performShutdown();
                        _state = OFF_STATE;
                        break;
                    }

                break;

            case SERVING_CUSTOMER_STATE:

                // The following will not return until the session has
                // completed

                currentSession->performSession();

                _state = IDLE_STATE;

                break;
        }

    }

}
void ATM::switchOn()
{
    QMutexLocker locker(&_lock);
    _switchOn = true;
    //notify();
}

void ATM::switchOff()
{
    QMutexLocker locker(&_lock);
    _switchOn = false;
    //notify();
}

void ATM::cardInserted()
{
    //QMutexLocker locker(&_lock);
    _cardInserted = true;
    cout<<"cardInserted"<<endl;
    //notify();
}

void ATM::Mylock()
{
    _lock.lock();
}
void ATM::Myunlock()
{
    _lock.unlock();
}
void ATM::Mywait()
{
    _cond.wait(&_lock);
}
void ATM::Mywake()
{
    _cond.wakeOne();
}

int ATM::getID()
{
    return _id;
}

string ATM::getPlace()
{
    return _location;
}

string ATM::getBankName()
{
    return _bankName;
}
CardReader* ATM::getCardReader()
{
    return _cardReader;
}

CashDispenser* ATM::getCashDispenser()
{
    return _cashDispenser;
}
CustomerConsole* ATM::getCustomerConsole()
{
    return _customerConsole;
}

EnvelopeAcceptor* ATM::getEnvelopeAcceptor()
{
    return _envelopeAcceptor;
}
Log* ATM::getLog()
{
    return _log;
}
NetworkToBank* ATM::getNetworkToBank()
{
    return _networkToBank;
}
OperatorPanel* ATM::getOperatorPanel()
{
    return _operatorPanel;
}
ReceiptPrinter* ATM::getReceiptPrinter()
{
    return _receiptPrinter;
}
void ATM::performStartup()
{
    //_lock.lock();
    //_cond.wait(&_lock);//实现同步
    Money initialCash = _operatorPanel->getInitialCash();


    _cashDispenser->setInitialCash(initialCash);
    _networkToBank->openConnection();
    //_lock.unlock();
}
void ATM::performShutdown()
{
    _networkToBank->closeConnection();
}

ATM::~ATM()
{
    if(_cardReader)
    {
        delete _cardReader;
    }
    if(_cashDispenser)
    {
        delete _cashDispenser;
    }
    if(_customerConsole)
    {
        delete _customerConsole;
    }
    if(_envelopeAcceptor)
    {
        delete _envelopeAcceptor;
    }
    if(_log)
    {
        delete _log;
    }
    if(_networkToBank)
    {
        delete _networkToBank;
    }
    if(_operatorPanel)
    {
        delete _operatorPanel;
    }
    if(_receiptPrinter)
    {
        delete _receiptPrinter;
    }

}
