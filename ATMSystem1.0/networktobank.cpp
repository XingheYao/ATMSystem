#include "networktobank.h"
#include "mainwindow.h"
Status* NetworkToBank::sendMessage(Message* message, Balances* balances)
{
    // Log sending of the message

    _log.logSend(*message);

    // Simulate the sending of the message - here is where the real code
    // to actually send the message over the network would go


    MainWindow & w1=get_global<MainWindow>();
    Status* result = w1.sendMessage(message, balances);
    // Log the response gotten back

    _log.logResponse(result);

    return result;
}

Status* NetworkToBank::sendMessage(Message* message)
{
    MainWindow & w1=get_global<MainWindow>();
    Status* result = w1.sendMessage(message);
    return result;
}
