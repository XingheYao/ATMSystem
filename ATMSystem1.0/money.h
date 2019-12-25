#ifndef MONEY_H
#define MONEY_H


#include <string>
class Money
{
public:

    Money();                        // 初始化为0.00
    Money(unsigned int dollars);             // 确切的美元金额-没有美分
    Money(unsigned int dollars,unsigned long cents);
    Money(const Money &toCopy);

    Money operator +    (const Money& other) const;
    Money operator -    (const Money& other) const;
    Money& operator += (const Money& other);
    Money& operator -= (const Money& other);

    unsigned int dollars() const;
    unsigned int cents() const;

    bool operator == (const Money& other) const;
    bool operator <  (const Money& other) const;

    void subtract(Money amountToSubtract)
    {
        _cents -= amountToSubtract._cents;
    }
    bool lessEqual(Money compareTo)
    {
        return _cents <= compareTo._cents;
    }

    std::string toString()
    {
        std::string money="$"+std::to_string(_cents/100)+
                (_cents %100 >= 10  ? "." +std::to_string(_cents % 100)  : ".0" + std::to_string(_cents % 100));
        return money;
    }

private:

    unsigned long _cents;
};

#endif // MONEY_H
