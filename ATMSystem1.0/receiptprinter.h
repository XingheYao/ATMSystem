#ifndef RECEIPTPRINTER_H
#define RECEIPTPRINTER_H
#include "log.h"
#include "balances.h"
#include "receipt.h"
//#include "mainwindow.h"
class ReceiptPrinter
{
public:
    ReceiptPrinter(){}
    void print(Receipt receipt);
};


#endif // RECEIPTPRINTER_H
