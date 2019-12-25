#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "atm.h"
#include "session.h"
#include "receipt.h"
#include "message.h"
class CardRetained : public exception
{
public:

    const char * what () const throw ()
    {
      return "Card retained due to too many invalid PINs";
    }
};


class AccountInformation
{
public:
    /** Names for accounts to be offered in menu to customer
     */
    static const vector<string>  ACCOUNT_NAMES ;

    /** Abbreviations for account names to be printed on receipts.
     */
    static const vector<string>  ACCOUNT_ABBREVIATIONS ;
} ;





class Transaction
{
    /*
     * 作为特定交易类型的基类
     */
    friend class Receipt;
public:

    Transaction( Session session, Card card, int pin);
    static Transaction* makeTransaction(ATM* atm, Session session,Card card, int pin)throw (Cancelled);
    bool performTransaction()throw (CardRetained);
    Status* performInvalidPINExtension(int &pin)throw (Cancelled,CardRetained);
    int getSerialNumber()
    {
        return _serialNumber;
    }
    virtual ~Transaction();
protected:
    virtual Message* getSpecificsFromCustomer() throw (Cancelled);
    // 创建交易的会话；用于ATM与客户；与它通信的银行来执行工作
    virtual Receipt completeTransaction() throw (Cancelled);

protected:
    Session _session;
    ATM *_atm;
    Card _card;
    int _pin;
    // 每个交易都分配一个唯一的序列号以在与银行通讯和收据上进行识别。
    int _serialNumber;
    Balances* _balances;
    Message* _message;

private:
    static int _nextSerialNumber ;
    int _state;

    static const vector<string>  TRANSACTION_TYPES_MENU;
    static const int GETTING_SPECIFICS_STATE ;
    static const int SENDING_TO_BANK_STATE ;
    static const int INVALID_PIN_STATE ;
    static const int COMPLETING_TRANSACTION_STATE;
    static const int PRINTING_RECEIPT_STATE ;
    static const int ASKING_DO_ANOTHER_STATE ;
};

//


class Withdrawal : public Transaction
{
public:
    Withdrawal( Session session, Card card, int pin);
protected:
    Message* getSpecificsFromCustomer() throw (Cancelled);
    Receipt completeTransaction()throw (Cancelled);
private:
    int _from;
    Money _amount;
};

//

class Deposit : public Transaction
{
public:
    Deposit( Session session, Card card, int pin);
protected:
    Message* getSpecificsFromCustomer() throw (Cancelled);
    Receipt completeTransaction()throw (Cancelled);

private:

    int _to;
    Money _amount;
};

//

class Transfer: public Transaction
{
public:

    Transfer( Session session, Card card, int pin);

protected:
    Message* getSpecificsFromCustomer() throw (Cancelled);
    Receipt completeTransaction()throw (Cancelled);

private:

    int _from;
    int _to;
    Money   _amount;
};

//

class Inquiry: public Transaction
{
public:

    Inquiry( Session session, Card card, int pin);

protected:
    Message* getSpecificsFromCustomer() throw (Cancelled);
    Receipt completeTransaction()throw (Cancelled);

private:
    int _from;
};


#endif // TRANSACTION_H
