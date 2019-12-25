#ifndef SESSION_H
#define SESSION_H
#include "atm.h"
//#include "transaction.h"
class Session
{
    /* 管理一个用户的会话
    */

public:

    Session();

    void performSession();

    // 如果银行说原始PIN码无效，则强制用户重新输入PIN码。

    void setPIN(int pin);

private:

    ATM *_atm;
    int _pin;
    int _state;
    /** Reading the customer's card
     */
    static const int READING_CARD_STATE = 1;

    /** Asking the customer to enter a PIN
     */
    static const int READING_PIN_STATE = 2;

    /** Asking the customer to choose a transaction type
     */
    static const int CHOOSING_TRANSACTION_STATE = 3;

    /** Peforming a transaction
     */
    static const int PERFORMING_TRANSACTION_STATE = 4;

    /** Ejecting the customer's card
     */
    static const int EJECTING_CARD_STATE = 5;

    /** Session finished
     */
    static const int FINAL_STATE = 6;
};

#endif // SESSION_H
