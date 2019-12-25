#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <string>
#include <qdir.h>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QMainWindow>

#include "status.h"
#include "message.h"
#include "money.h"
#include "balances.h"
#include "card.h"
#include "simulatedbank.h"
#include "cardreader.h"
#include "atm.h"
namespace Ui {
class MainWindow;
}

//template<typename T>
//T& get_global(){
//    static T instance;
//    return instance;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class OperatorPanel;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    Status* sendMessage(Message* message, Balances* balances);
    Status* sendMessage(Message* message);
    bool acceptEnvelope();
    void display(QString text);
    void clearDisplay();
    string readInput(int mode, int maxValue);
    void PINDialog();
    void showDisdpenseCash();
    //Money getInitialCash();
    Card* readCard();
    void ejectCard();
    void retainCard();
    void dispenseCash(Money amount);
    void printReceiptLine(QString qText);
    void dealDone();   //线程槽函数
    void stopThread();  //停止线程

    void ejectCardBtn();
signals:
    void spinBoxSignal();
    void switchSignal();
    void cardSignal();
    void ejectCardSignal();
    void PINSignal();
    void dispenseCashSignal();
public:
    /* Possible values for mode parameter to readInput() */

    /** Read input in PIN mode - allow user to enter several characters,
     *  and to clear the line if the user wishes; echo as asterisks
     */
    static const int PIN_MODE = 1;

    /** Read input in amount mode - allow user to enter several characters,
     *  and to clear the line if the user wishes; echo what use types
     */
    static const int AMOUNT_MODE = 2;

    /** Read input in menu choice mode - wait for one digit key to be pressed,
     *  and return value immediately.
     */
    static const int MENU_MODE = 3;

private slots:
    int switchDialog();
    Money getInitialCash();
    void cardDialog();
    void setSwitch();
    void printLogLine();
    void retainCardBtn();
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    ATM* _atm;
    bool _state;
    int _tempValue;
    int _cardNumber;
    int _cardPIN;
    bool _cancelled;
    int _mode;
    int _maxValue;
    Money _tmpAmount;

    SimulatedBank* _simBank;
};

#endif // MAINWINDOW_H
