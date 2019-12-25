#ifndef NETWORKTOBANK_H
#define NETWORKTOBANK_H
#include "log.h"
#include "balances.h"
class NetworkToBank
{
public:
    NetworkToBank(Log log, std::string bankAddress)
        :_log(log)
        ,_bankAddress(bankAddress)
    {}
    void openConnection()
    {
        // Since the network is simulated, we don't have to do anything
    }
    void closeConnection()
    {
        // Since the network is simulated, we don't have to do anything
    }
    Status* sendMessage(Message* message, Balances* balances);
    Status* sendMessage(Message* message);
private:
    Log _log;
    std::string _bankAddress;
};

#endif // NETWORKTOBANK_H
