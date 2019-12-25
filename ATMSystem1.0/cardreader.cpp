#include "cardreader.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "atm.h"
CardReader::CardReader(/*ATM atm*/)
    //: _atm( _atm->get_instance())
{ }

Card* CardReader::readCard()
{
    MainWindow & w1=get_global<MainWindow>();
    Card* card=w1.readCard();
    if( card==nullptr)
    {
        //cout<<"card is nullptr"<<endl;
        return nullptr;
    }
    return card;
}

void CardReader::ejectCard()
{
    MainWindow & w1=get_global<MainWindow>();
    w1.ejectCard();
}

void CardReader::retainCard()
{
    MainWindow & w1=get_global<MainWindow>();
    w1.retainCard();
}
