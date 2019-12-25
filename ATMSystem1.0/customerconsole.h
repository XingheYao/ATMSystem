#ifndef CUSTOMERCONSOLE_H
#define CUSTOMERCONSOLE_H
#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include "money.h"
//#include "mainwindow.h"

using namespace std;
class Cancelled : public exception
{
public:

    const char * what () const throw ()
    {
      return "Cancelled by customer\n";
    }
};
class CustomerConsole
{
public:
    CustomerConsole(){}
    void display(string message);
    void clearDisplay();
    int readPIN(string prompt)throw (Cancelled);
    int readMenuChoice(string prompt, vector<string> menu)throw (Cancelled);
    Money readAmount(string prompt)throw (Cancelled);
};

#endif // CUSTOMERCONSOLE_H
