#include "customerconsole.h"
#include "mainwindow.h"
#include "atm.h"
int CustomerConsole::readPIN(string prompt) throw (Cancelled)
{
    ATM* atm=ATM::get_instance();

    atm->display(QString::fromStdString(prompt));

    MainWindow & w1=get_global<MainWindow>();
    string input = w1.readInput(w1.PIN_MODE, 0);

//    w1.clearDisplay();

    if (input.empty())
        throw  Cancelled();
    else
        return stoi(input);
}

int CustomerConsole::readMenuChoice(string prompt, vector<string> menu) throw (Cancelled)
{
    ATM* atm=ATM::get_instance();
    atm->clearDisplay();
    atm->display(QString::fromStdString(prompt));
    for (int i = 0; i < menu.size(); i ++)
        atm->display(QString::fromStdString(to_string(i+1) + ") " + menu[i]));

    MainWindow & w1=get_global<MainWindow>();
    string input =
        w1.readInput(w1.MENU_MODE, menu.size());

    //w1.clearDisplay();

    if (input.empty())
        throw  Cancelled();
    else
        return stoi(input) - 1;
}

Money CustomerConsole::readAmount(string prompt) throw (Cancelled)
{
    ATM* atm=ATM::get_instance();

    atm->display(QString::fromStdString(prompt));
    //w1.display("");
    MainWindow & w1=get_global<MainWindow>();
    string input = w1.readInput(w1.AMOUNT_MODE, 0);

//    w1.clearDisplay();
    w1.display(QString::fromStdString(input));
    if (input.empty())
        throw  Cancelled();
    else
    {
        int dollars = stoi(input) / 100;
        int cents = stoi(input) % 100;
        return Money(dollars, cents);
    }
}

void CustomerConsole::display(string message)
{
    ATM* atm=ATM::get_instance();

    atm->display(QString::fromStdString(message));
}

void CustomerConsole::clearDisplay()
{
    ATM* atm=ATM::get_instance();

    atm->clearDisplay();
}
