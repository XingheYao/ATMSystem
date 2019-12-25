#include "receiptprinter.h"
#include "mainwindow.h"
void ReceiptPrinter::print(Receipt receipt)
{


    while (receipt.hasMoreElements())
    {
        ATM* atm=ATM::get_instance();
        atm->printReceiptLine(QString::fromStdString(receipt.nextElement()));
    }
}
