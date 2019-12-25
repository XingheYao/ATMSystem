#ifndef SIMULATEDBANK_H
#define SIMULATEDBANK_H


#include "status.h"
#include "money.h"
#include "message.h"
#include "balances.h"

template<typename T>
T& get_global(){
    static T instance;
    return instance;
}

class Success : public Status
    {
public:
           bool isSuccess()
        {
               std::cout<<"Success::isSuccess"<<std::endl;
            return true;
        }

           bool isInvalidPIN()
        {
               std::cout<<"Success::isInvalidPIN"<<std::endl;
            return false;
        }

           string getMessage()
        {
               std::cout<<"Success::getMessage"<<std::endl;
            return nullptr;
        }
           string toString()
           {
               if(isSuccess())
               {
                   return "SUCCESS";
               }
               else if(isInvalidPIN())
               {
                   return "INVALID PIN";
               }
               else
               {
                   return "FAILURE" + getMessage();
               }
           }
    };
class Failure : public Status
    {
public:
        Failure(string message)
        {
            _message = message;
        }

          bool isSuccess()
        {
              std::cout<<"Failure::isSuccess"<<std::endl;
            return false;
        }

          bool isInvalidPIN()
        {
              std::cout<<"Failure::isInvalidPIN"<<std::endl;
            return false;
        }

          string getMessage()
        {
              std::cout<<"Failure::getMessage"<<std::endl;
            return _message;
        }
          string toString()
          {
              if(isSuccess())
              {
                  return "SUCCESS";
              }
              else if(isInvalidPIN())
              {
                  return "INVALID PIN";
              }
              else
              {
                  return "FAILURE" + getMessage();
              }
          }
private:
        string _message;
    };

 class InvalidPIN : public Failure
    {
 public:
        InvalidPIN()
        :Failure("Invalid PIN")
        {
        }

          bool isInvalidPIN()
        {
              std::cout<<"InvalidPIN::isInvalidPIN"<<std::endl;
            return true;
        }
    };

class SimulatedBank
{
public:
    SimulatedBank();
    Status* handleMessage(Message* message, Balances* balances);
    Status* verifyPin(Message* message);
private:
    Status* withdrawal(Message* message, Balances* balances);
    Status* initiateDeposit(Message* message);
    Status* completeDeposit(Message* message, Balances* balances);
    Status* transfer(Message* message, Balances *balances);
    Status* inquiry(Message* message, Balances* balances);



private:
     static const int PIN [3] ;
     static const int ACCOUNT_NUMBER [3] [3] ;
     static Money WITHDRAWALS_TODAY [3];
     static const Money DAILY_WITHDRAWAL_LIMIT ;
     Money BALANCE [4] =
     {
          Money(0),   // dummy for nonexistent account 0
          Money(100),
          Money(1000),
          Money(5000)
     };
     Money AVAILABLE_BALANCE [4] =
     {
          Money(0),   // dummy for nonexistent account 0
          Money(100),
          Money(1000),
          Money(5000)
     };
};


#endif // SIMULATEDBANK_H
