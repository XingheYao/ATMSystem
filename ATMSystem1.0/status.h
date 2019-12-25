#ifndef STATUS_H
#define STATUS_H
#include <string>
#include <iostream>


using namespace std;
class Status
{
public:


    virtual bool isSuccess()
    {
        std::cout<<"Status::isSuccess"<<std::endl;
        return false;
    }
    virtual bool isInvalidPIN()
    {
        std::cout<<"Status::isInvalidPIN"<<std::endl;
        return false;
    }
    virtual string getMessage()
    {
        std::cout<<"Status::getMessage"<<std::endl;
        string str="Status getMessage error";
        return str;
    }
    virtual ~Status(){}
    virtual string toString()
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

#endif // STATUS_H
