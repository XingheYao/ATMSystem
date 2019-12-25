#ifndef ATM_H
#define ATM_H
#include <Qthread>
#include <QMutex>
#include <QWaitCondition>

#include "cardreader.h"
#include "cashdispenser.h"
#include "customerconsole.h"
#include "envelopeacceptor.h"
#include "log.h"
#include "money.h"
#include "networktobank.h"
#include "operatorpanel.h"
#include "receiptprinter.h"

class ATM:  public QThread
{
    Q_OBJECT
    /*管理整个ATM*/
private:
     ATM(int id, string location, string bankName);
public:
     ATM(const ATM&)=delete;
     ATM& operator=(const ATM&)=delete;
     static ATM* get_instance();

    ~ATM();
    //void run();
    void switchOn();
    void switchOff();
    void cardInserted();
    void Mylock();
    void Myunlock();
    void Mywait();
    void Mywake();

    int getID();
    string getPlace();
    string getBankName();
    CardReader* getCardReader();
    CashDispenser* getCashDispenser();
    CustomerConsole* getCustomerConsole();
    EnvelopeAcceptor* getEnvelopeAcceptor();
    Log* getLog();
    NetworkToBank* getNetworkToBank();
    OperatorPanel* getOperatorPanel();
    ReceiptPrinter* getReceiptPrinter();
protected:
    //QThread的虚函数
    //线程处理函数
    //不能直接调用，通过start()间接调用
    void run();
    friend class Receipt;
    friend class MainWindow;
signals:
    //void isDone();  //处理完成信号
    void display(QString text);
    void clearDisplay();
    //void sendMessage(Message message,Balances balances);
    void printReceiptLine(QString qText);
    void retainCardSignal();
private:
    void performStartup();
    void performShutdown();
private:

    int _id;//ATM机编号
    string _location;//ATM机位置
    string _bankName;

    CardReader* _cardReader;
    CashDispenser* _cashDispenser;
    CustomerConsole* _customerConsole;
    EnvelopeAcceptor* _envelopeAcceptor;
    Log* _log;
    NetworkToBank* _networkToBank;
    OperatorPanel* _operatorPanel;
    ReceiptPrinter* _receiptPrinter;
    int _state;
    bool _switchOn;
    bool _cardInserted;
    static const int OFF_STATE = 0;
    static const int IDLE_STATE = 1;
    static const int SERVING_CUSTOMER_STATE = 2;


private:
    QMutex _lock;
    QWaitCondition _cond;
    //bool _isCanRun;
};
#endif // ATM_H
