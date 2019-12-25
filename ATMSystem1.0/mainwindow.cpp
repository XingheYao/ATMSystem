#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "atm.h"
#include <iostream>
#include <fstream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _maxValue=-1;
    _cardNumber=0;
    _tempValue=0;
    _mode=0;
    _state=false;
    _cancelled=false;
    _tmpAmount=Money(0);
    _simBank=new SimulatedBank();
    ui->setupUi(this);
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    _atm = ATM::get_instance();
    _atm->start();
    connect(ui->actionSwitch,SIGNAL(triggered()),this,SLOT(setSwitch()));
    connect(ui->actionLog,SIGNAL(triggered()),this,SLOT(printLogLine()));
    //信号调用
    connect(this, &MainWindow::switchSignal, this,&MainWindow::switchDialog);
    connect(_atm, &ATM::retainCardSignal, this,&MainWindow::retainCardBtn);
    connect(this, &MainWindow::ejectCardSignal, this,&MainWindow::ejectCardBtn);
    connect(this, &MainWindow::PINSignal, this,&MainWindow::PINDialog);
    connect(this, &MainWindow::dispenseCashSignal, this,&MainWindow::showDisdpenseCash);
    //线程信号槽（传递信息）
    connect(_atm, &ATM::display, this, &MainWindow::display);
    connect(_atm, &ATM::clearDisplay, this, &MainWindow::clearDisplay);
    connect(_atm, &ATM::printReceiptLine, this, &MainWindow::printReceiptLine);
    //当按窗口右上角x时，触发destroyed信号
    connect(this, &MainWindow::destroyed, this, &MainWindow::stopThread);
}

MainWindow::~MainWindow()
{
    if(_simBank)
    {
        delete _simBank;
    }
    delete ui;
}

void MainWindow::stopThread()//停止atm线程运行
{
    //停止线程
    cout<<"atm is quiting"<<endl;
    _atm->quit();

    //等待线程处理完手头工作
    //_atm->wait();
    cout<<"atm has exited"<<endl;
}

void MainWindow::setSwitch()//系统打开操作，运行系统打开对话框
{
    if(_atm->_state==ATM::OFF_STATE)
    {

        if(_atm->_switchOn==false)
        {

            emit switchSignal();
        }
        else
        {

            _atm->switchOff();
        }
    }
}
int MainWindow::switchDialog()//系统打开对话框
{
    //输入整数
    QString dlgTitle="ATM系统";
    QString txtLabel="一个真正的自动取款机将有一个机制来感知或\n"
                     "允许操作人员在取款机中输入20美元的钞票数量。\n"
                     "为了进行模拟，请手动输入20美元钞票的数目。\n"
                     "然后按“OK”\n";
    int defaultValue=1; //现有字体大小
    int minValue=1, maxValue=50,stepValue=1; //范围，步长
    bool ok=false;
    int inputValue = QInputDialog::getInt(this, dlgTitle,txtLabel,
                               defaultValue, minValue,maxValue,stepValue,&ok);
    if (ok) //是否确认输入
    {
        //设置ATM存款20$的数量
        ui->pushButton_3->show();
        _tempValue=inputValue;
        _state=true;
        _atm->switchOn();

    }

    _atm->Mywake();
    return inputValue;
}

Money MainWindow::getInitialCash()//获取系统的初始金额
{
    return Money(20*_tempValue);
}


Card* MainWindow::readCard()
{
    //_atm->Mylock();
    //ui->pushButton_3->setText("插卡");
    emit cardSignal();
    //_atm->Mywait();
    int cardNumber=_cardNumber;
    cout<<"cardNum = "<<_cardNumber<<endl;
    if (cardNumber > 0)
    {
        //_atm->Myunlock();
        return new Card(cardNumber);
    }
    else
    {
        //_atm->Myunlock();
        //cout<<"cardNumber is invalid"<<endl;
        return nullptr;
    }
}

void MainWindow::ejectCard()
{
   // ui->pushButton_3->setText("退卡");
}

void MainWindow::retainCard()
{
    emit cardSignal();
}

void MainWindow::retainCardBtn()
{
    ui->pushButton_3->setText("插卡");
    ui->pushButton_3->show();
}

void MainWindow::ejectCardBtn()
{
    ui->pushButton_3->setText("退卡中");
    QThread::sleep(2);
    ui->pushButton_3->setText("插卡");
    ui->pushButton_3->show();
}



string MainWindow::readInput(int mode, int maxValue)//获取选项输入
{
    _atm->Mylock();
    _mode=mode;
    _maxValue=maxValue;
    cout<<"maxValue = "<<_maxValue<<endl;
    cout<<"mode = "<<mode<<endl;
    string currentInput;
    switch(mode)
    {
        case PIN_MODE:
        {
            emit PINSignal();
            break;
        }
        case AMOUNT_MODE:
        case MENU_MODE:
        {

            break;
        }

    }

    _atm->Mywait();

    currentInput=to_string(_tempValue);
    cout<<"currentInput = "<<currentInput<<endl;
    _atm->Myunlock();
    if (_cancelled)
        return "";
    else
        return currentInput;
}

void MainWindow::display(QString text)//显示框：显示内容
{
    ui->textBrowser->append(text);
    //QString qText = QString::fromStdString(text);
    //ui->textBrowser->clear();
    //ui->textBrowser->setText(text);
    //QApplication::processEvents();
}
void MainWindow::clearDisplay()//显示框：清除显示
{
    ui->textBrowser->clear();
}


void MainWindow::on_pushButton_clicked()//OK键点击响应
{
    _cancelled=false;
    cout<<"box num = "<<_tempValue<<endl;
    _tempValue=ui->spinBox->text().toInt();

    if(_tempValue >_maxValue&&_maxValue!=0)
        return;
    _atm->Mywake();
}

void MainWindow::on_pushButton_2_clicked()//取消键点击响应
{
    switch(_mode)
    {
        // It is possible to press the cancel key when requested
        // to insert an envelope - so notify the envelope acceptor
        // of this fact (notification is ignored if acceptor is
        // not waiting for an envelope)

    //            synchronized(envelopeAcceptor)
    //            {
    //                envelopeAcceptor.notify();
    //            }

        case AMOUNT_MODE:
        case MENU_MODE:

            _cancelled = true;
            _atm->Mywake();
    }
}

void MainWindow::on_pushButton_3_clicked()//插卡键点击响应
{
    cardDialog();
    //ui->pushButton_3->hide();
}
void MainWindow::cardDialog()//插卡界面对话框
{
    QString dlgTitle="ATM系统";
    QString txtLabel="一台真正的自动柜员机会有一个磁条读卡器来读取模拟卡，\n"
                     "请手动输入卡号。然后按“OK”键\n"
                     "(一个无效的整数或一个不大于零的整数将被视为不可读卡)";
    //QString txtLabel1="请输入密码\n";
    QString defaultInput="";
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    //QLineEdit::EchoMode echoMode1=QLineEdit::Password;//密码输入
    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,defaultInput, &ok);
    //QString text1 = QInputDialog::getText(this, dlgTitle,txtLabel1, echoMode1,defaultInput, &ok);
    if (ok && !text.isEmpty())
    {
        _cardNumber=text.toInt();
        _atm->cardInserted();
        _atm->Mywake();
        //cout<<"Dialog cardNum = "<<_cardNumber<<endl;
        ui->pushButton_3->hide();
    }

}

void MainWindow::PINDialog()//PIN对话框
{
    QString dlgTitle="ATM系统";
    QString txtLabel1="请输入密码\n然后按“OK”键\n";
    QString defaultInput="";
    _tempValue=0;
    //QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    QLineEdit::EchoMode echoMode1=QLineEdit::Password;//密码输入
    bool ok=false;
    QString text1 = QInputDialog::getText(this, dlgTitle,txtLabel1, echoMode1,defaultInput, &ok);
    if (ok && !text1.isEmpty())
    {
//        display("world");
        _tempValue=text1.toInt();
    }
    else if(!ok)
    {
        _tempValue=-9999;
    }
    _atm->Mywake();
}

Status* MainWindow::sendMessage(Message* message, Balances *balances)//交易信息传递处理
{

    return _simBank->handleMessage(message, balances);
}

Status* MainWindow::sendMessage(Message* message)//交易信息传递处理（PIN专用）
{
    SimulatedBank simBank;
    return simBank.verifyPin(message);
}

void MainWindow::dispenseCash(Money amount)//交易处理函数
{
    _tmpAmount=amount;
    emit dispenseCashSignal();
}
void MainWindow::showDisdpenseCash()//交易处理显示
{
    //cout<<"dispenseCash"<<endl;
    QString dlgTitle="取钱";
    QString strInfo="取钱成功";
    strInfo=strInfo+QString::fromStdString(_tmpAmount.toString());
    QMessageBox::information(this, dlgTitle, strInfo,
                                         QMessageBox::Ok,QMessageBox::NoButton);
    //QString qText = QString::fromStdString(amount.toString());
    //ui->textBrowser->setText(qText);
}

void MainWindow::printReceiptLine(QString qText)//打印凭条（收据）
{
    //QString qText = QString::fromStdString(text);
    //ui->textBrowser_2->append(qText);
    ui->textBrowser_2->append(qText);
    ui->pushButton_4->show();
}
void MainWindow::on_pushButton_4_clicked()//打印收据对话框
{
    QString dlgTitle="收据";
    QString strInfo="打印成功！";
    strInfo=strInfo+QString::fromStdString(_tmpAmount.toString());
    QMessageBox::information(this, dlgTitle, strInfo,
                                         QMessageBox::Ok,QMessageBox::NoButton);
    ui->pushButton_4->hide();
}

void MainWindow::printLogLine()//日志显示
{
    QString dlgTitle="日志记录";
    QString strInfo;

    std::ifstream ifs("Log.txt", std::ios::in);
    if (!ifs)
    {
        std::cout << "ifstream error!" << std::endl;
        return ;
    }
    char buffer[256]={0};
    while (!ifs.eof())
    {
        ifs.getline(buffer, 100);
        //std::cout << buffer << std::endl;
        strInfo.append(buffer);
        strInfo.append("\n");
    }
    ifs.close();
    QMessageBox::information(this, dlgTitle, strInfo,
                                         QMessageBox::Ok,QMessageBox::NoButton);
}

bool MainWindow::acceptEnvelope()

{
    return true;
}

void MainWindow::on_pushButton_5_clicked()
{

}
