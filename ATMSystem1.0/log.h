#ifndef LOG_H
#define LOG_H
#include "message.h"
#include "status.h"
#include <iostream>
#include <fstream>

//#include "mainwindow.h"
//class ATM;
class Log
{
public:
    Log();
    Log(const Log &log);
    ~Log();
    void logSend(Message message);

    void logResponse(Status* response);

    void logCashDispensed(Money amount);

    void logEnvelopeAccepted();

private:
    //ATM *_atm;
    std::ofstream _ofs;
};
#endif // LOG_H
