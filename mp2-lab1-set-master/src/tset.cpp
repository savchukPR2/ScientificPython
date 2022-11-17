

#include "tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) //  Прямая инициализация
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) //  Прямая инициализация
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) //  Прямая инициализация
{
}

TSet::operator TBitField() // - преобразование к битовому полю
{
    TBitField tmp(BitField);
    return tmp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества - наличие или отсутствие
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem >= MaxPower) || (Elem < 0)) {
        throw ("Elem is out of universe");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem >= MaxPower) || (Elem < 0)) {
        throw ("Elem is out of universe");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s) {
        this->MaxPower = s.MaxPower;
        this->BitField = s.BitField;
    }
    return *this;
}

bool TSet::operator== (const TSet& s) const // сравнение
{
    return BitField == s.BitField;
}

bool TSet::operator!= (const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    if (this->MaxPower >= s.MaxPower)
    {
        TSet tmp(this->MaxPower);
        tmp.BitField = this->BitField | s.BitField;
        return tmp;
    }
    else
    {
        TSet tmp(s.MaxPower);
        tmp.BitField = this->BitField | s.BitField;
        return tmp;
    }
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet tmp(BitField);
    tmp.InsElem(Elem);
    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(BitField);
    tmp.DelElem(Elem);
    return tmp;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    if (this->MaxPower >= s.MaxPower)
    {
        TSet tmp(this->MaxPower);
        tmp.BitField = this->BitField & s.BitField;
        return tmp;
    }
    else
    {
        TSet tmp(s.MaxPower);
        tmp.BitField = this->BitField & s.BitField;
        return tmp;
    }
}

TSet TSet::operator~(void) // дополнение
{
    TSet tmp(~BitField);
    return tmp;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    string inpt;
    int n = s.GetMaxPower();
    for (int i = 0; i < n; i++)
    {
        cout << i << "The element is included in the set?" << endl;
        istr >> inpt;
        if (inpt == "+") {
            s.InsElem(i);
        }
        else if (inpt == "-") {
            s.DelElem(i);
        }
        else {
            throw ("You can only answer: '+' or  '-'");
        }
        cout << endl;
    }
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    char ch = '{';
    int n = s.GetMaxPower();
    for (int i = 0; i < n; i++)
    {
        if (s.IsMember(i))
        {
            ostr << ch << i;
            ch = ',';
        }
    }
    ostr << " }";
    return ostr;
}
