#include "money.h"


Money::Money()
    : _cents(0)
{ }

Money::Money(unsigned int dollars)
    : _cents(100L * dollars)
{ }

Money::Money(unsigned int dollars, unsigned long cents)
    : _cents(100L * dollars + cents)
{ }

Money::Money(const Money &toCopy)
    :_cents(toCopy._cents)
{}

Money Money::operator + (const Money& other) const
{
    return Money(0, _cents + other._cents);
}

Money Money::operator - (const Money& other) const
{
    return Money(0, _cents - other._cents);
}

Money& Money::operator += (const Money& other)
{
    _cents += other._cents;
    return *this;
}

Money& Money::operator -= (const Money& other)
{
    _cents -= other._cents;
    return *this;
}

unsigned int Money::dollars() const
{
    return (unsigned int)_cents / 100;
}

unsigned int Money::cents() const
{
    return (unsigned int)_cents % 100;
}


bool Money::operator == (const Money& other) const
{
    return _cents == other._cents;
}

bool Money::operator <  (const Money& other) const
{
    return _cents < other._cents;
}
