#include "log.h"

Log::Log()
    :_ofs("Log.txt", std::ios::trunc)
{

    if(!_ofs)
    {
        std::cout << "ofstream error!" << std::endl;
        //return -1;
    }
}

Log::Log(const Log &log)
    :_ofs("Log.txt", std::ios::trunc)
{

    if(!_ofs)
    {
        std::cout << "ofstream error!" << std::endl;
        //return -1;
    }

}

Log::~Log()
{

    if(_ofs.is_open())
        _ofs.close();

}
void Log::logSend(Message message)
{


    //std::cout << ofs.tellp() << std::endl;

    _ofs <<"Message:   " <<message.toString() << std::endl;

    //ofs.close();
}

void Log::logResponse(Status* response)
{
    _ofs <<"Response:  " <<response->toString() << std::endl;
}

void Log::logCashDispensed(Money amount)
{
    _ofs <<"Dispensed: " <<amount.toString() << std::endl;
}

void Log::logEnvelopeAccepted()
{
    _ofs <<"Response:  " << std::endl;
}


