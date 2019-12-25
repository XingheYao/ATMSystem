#include "simulatedbank.h"
const int SimulatedBank::PIN [3] =
{
    0,  // dummy for nonexistent card 0
    42,
    1234
};
const int SimulatedBank::ACCOUNT_NUMBER [3] [3] =
{
    { 0, 0, 0 },    // dummies for nonexistent card 0
    { 1, 2, 0 },
    { 1, 0, 3 }
};
Money SimulatedBank::WITHDRAWALS_TODAY [3] =
{
     Money(0),   // dummy for nonexistent card 0
     Money(0),
     Money(0)
};
const Money SimulatedBank::DAILY_WITHDRAWAL_LIMIT =  Money(300);

SimulatedBank::SimulatedBank()
{

}

Status* SimulatedBank::handleMessage(Message* message, Balances* balances)
{
    int cardNumber = message->getCard().getNumber();
    if (cardNumber < 1 || cardNumber > 3)
        return  new Failure("Invalid card");

    if (message->getPIN() != PIN [ cardNumber ] )
        return  new InvalidPIN();

    switch(message->getMessageCode())
    {
        case Message::WITHDRAWAL:

            return withdrawal(message, balances);

        case Message::INITIATE_DEPOSIT:

            return initiateDeposit(message);

        case Message::COMPLETE_DEPOSIT:

            return completeDeposit(message, balances);

        case Message::TRANSFER:

            return transfer(message, balances);

        case Message::INQUIRY:

            return inquiry(message, balances);

//        case Message::VERIIFYPIN:
//            return verifyPin(message);
    }

    // Need to keep compiler happy

    //new Status();
}

Status* SimulatedBank::withdrawal(Message* message, Balances* balances)
{
    int cardNumber = message->getCard().getNumber();

    int accountNumber = ACCOUNT_NUMBER [ cardNumber ] [ message->getFromAccount() ];
    if (accountNumber == 0)
        return  new Failure("Invalid account type");

    Money amount = message->getAmount();

    Money limitRemaining(DAILY_WITHDRAWAL_LIMIT);
    limitRemaining.subtract(WITHDRAWALS_TODAY[ cardNumber ]);
    if (! amount.lessEqual(limitRemaining))
        return  new Failure("Daily withdrawal limit exceeded");

    if (! amount.lessEqual(AVAILABLE_BALANCE [ accountNumber ]))
         return  new Failure("Insufficient available balance");

    // Update withdrawals today and account balances once we know everything is
    // OK

    WITHDRAWALS_TODAY [ cardNumber ]+amount;
    BALANCE [ accountNumber ].subtract(amount);
    AVAILABLE_BALANCE [ accountNumber ].subtract(amount);

    // Return updated balances

    balances->setBalances(BALANCE [ accountNumber ],
                         AVAILABLE_BALANCE [ accountNumber ]);
    return new Success();
}

Status* SimulatedBank::initiateDeposit(Message* message)
{
    int cardNumber = message->getCard().getNumber();

    int accountNumber = ACCOUNT_NUMBER [ cardNumber ] [ message->getToAccount() ];
    if (accountNumber == 0)
        return  new Failure("Invalid account type");

    // Don't update anything yet

    return new Success();
}

Status* SimulatedBank::completeDeposit(Message* message, Balances* balances)
{
    int cardNumber = message->getCard().getNumber();

    int accountNumber = ACCOUNT_NUMBER [ cardNumber ] [ message->getToAccount() ];
    if (accountNumber == 0)
        return  new Failure("Invalid account type");

    // Now we can update the balance

    Money amount = message->getAmount();
    BALANCE [ accountNumber ]+amount;

    // Return updated balances

    balances->setBalances(BALANCE [ accountNumber ],
                         AVAILABLE_BALANCE [ accountNumber ]);

    return new Success();
}

Status* SimulatedBank::transfer(Message* message, Balances* balances)
{
    int cardNumber = message->getCard().getNumber();

    int fromAccountNumber = ACCOUNT_NUMBER [ cardNumber ] [ message->getFromAccount() ];
    if (fromAccountNumber == 0)
        return  new Failure("Invalid from account type");

    int toAccountNumber = ACCOUNT_NUMBER [ cardNumber ] [ message->getToAccount() ];
    if (toAccountNumber == 0)
        return  new Failure("Invalid to account type");
    if (fromAccountNumber == toAccountNumber)
        return  new Failure("Can't transfer money from\nan account to itself");

    Money amount = message->getAmount();

    if (! amount.lessEqual(AVAILABLE_BALANCE [ fromAccountNumber ]))
         return  new Failure("Insufficient available balance");

    // Update account balances once we know everything is OK

    BALANCE [ fromAccountNumber ].subtract(amount);
    AVAILABLE_BALANCE [ fromAccountNumber ].subtract(amount);
    BALANCE [ toAccountNumber] + amount;
    AVAILABLE_BALANCE [ toAccountNumber ]+amount;

    // Return updated balances

    balances->setBalances(BALANCE [ toAccountNumber ],
                         AVAILABLE_BALANCE [ toAccountNumber ]);


    return new Success();
}
Status* SimulatedBank::inquiry(Message* message, Balances* balances)
{
    int cardNumber = message->getCard().getNumber();

    int accountNumber = ACCOUNT_NUMBER [ cardNumber ] [ message->getFromAccount() ];
    if (accountNumber == 0)
    {

        return  new Failure("Invalid account type");
    }
    // Return requested balances

    balances->setBalances(BALANCE[ accountNumber ],
                         AVAILABLE_BALANCE [ accountNumber ]);

    return new Success();
}

Status* SimulatedBank::verifyPin(Message* message)
{
    int cardNumber = message->getCard().getNumber();
    if (cardNumber < 1 || cardNumber > 3)
        return  new Failure("Invalid card");

    if (message->getPIN() != PIN [ cardNumber ] )
        return  new InvalidPIN();

    return new Success();
}
