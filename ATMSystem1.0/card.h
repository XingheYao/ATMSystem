#ifndef CARD_H
#define CARD_H
class Card
{
public:
    Card(int number)
        :_number(number)
    {}
//    Card()
//    {}

    int getNumber() const
    {
        return _number;
    }

private:
    int _number;
    //QString _passwd;
};
#endif // CARD_H
