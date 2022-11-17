#include "tbitfield.h"

using namespace std;

TBitField::TBitField(int len) // конструктор с параметром - длина поля
{
    if (len > 0) {
        BitLen = len;
        MemLen = len / (sizeof(TELEM) * 8) + 1; // TELEM = unsigned int = 4 байта = 32 бита
        cout << MemLen << endl;
        pMem = new TELEM[MemLen];
        if (pMem != NULL) {
            for (int i = 0; i < MemLen; i++) {
                pMem[i] = 0; // заполняем нулями
            }
        }
    }
    else {
        throw exception("The length must be greater than 0!"); // Исключение в случае длины <= 0
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen]; // выделение памяти
    for (int i = 0; i < MemLen; i++){
        pMem[i] = bf.pMem[i]; // копируем данные поле - можно через memcpy 
    }
}

TBitField::~TBitField() // Деструктор
{
    delete[] pMem; // Освобождаем память
}

/*
* Unsigned int = 4 байта = 32 бита
* 1 unsigned int = 32 бита = 32 элемента; 00....000(Последовательность из 32 битов, если есть n-й элемент, то бит равен 1(читается слево-направо)
* Тоесть с 0 по 31 бит это 0-е число unsigned int в массиве
*/
int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8); 
}

/* 
* Возвращает n бит - степень 2 от 0 до 7;  1 2 4 8 16 32 64 128 
* 1 - 0000 0001
* 2 - 0000 0010
* 4 - 0000 0100
* 8 - 0000 1000 и т.д - функция нужна будет для установки очистки бита
*/
TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & 7); 
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

// |= побитовое "или" - ставит в n-бите 1
void TBitField::SetBit(const int n) // установить бит
{
    if ((n > -1) && (n < BitLen)) {
        pMem[GetMemIndex(n)] |= GetMemMask(n); 
    }
    else {
        throw("going out of bounds"); // Исключение
    }

}

// &= побитовое "и" - ставит в n-бите 0 - тоже самое, то и в установке, но теперь 1 - 1111 1110 т.к ~ - отрицание, меняем значения на противоположные
void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n >= 0) && (n < BitLen)) {
        pMem[GetMemIndex(n)] &= ~GetMemMask(n); 
    }
    else {
        throw("going out of bounds");
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n > -1) && (n < BitLen)) {
        return pMem[GetMemIndex(n)] & GetMemMask(n); // Ну тут всё понятно
    }
    else {
        throw("bit pos is out of range!"); 
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) { // Базовая проверка при присваивании
        TBitField tmp(bf); // копируем переданный массив, чтобы не портить данные
        std::swap(BitLen, tmp.BitLen); // меняем местами - tmp - общеее название для временных переменных
        std::swap(MemLen, tmp.MemLen);
        std::swap(pMem, tmp.pMem);
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    int result = 1;
    if (BitLen != bf.BitLen) { // не может быть истинной, если размеры не равны
        result = 0;
    }
    else {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                result = 0;
                break;
            }
        }
    }
    return result;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf); // отрицание == 
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    if (BitLen = bf.BitLen) { // if с инициализацией, по умолчанию условие true
        TBitField tmp(*this);
        for (int i = 0; i < bf.MemLen; i++) {
            tmp.pMem[i] |= bf.pMem[i];
        }
        return tmp;
    }
    else { throw("TBitField object have diff size"); }
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    if (BitLen = bf.BitLen) { // if с инициализацией, по умолчанию условие true
        TBitField out(*this);
        for (int i = 0; i < bf.MemLen; i++) {
            out.pMem[i] &= bf.pMem[i];
        }
        return out;
    }
    else { throw("TBitField object have diff size"); }
}

// Меняет значения на противоположные
TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 0) {
            tmp.SetBit(i);
        }
        else {
            tmp.ClrBit(i);
        }
     }
     return tmp;
}

// ввод/вывод

// Работает по #define - с номерами ASKII 0 - 48 Номер, 1 - 49
istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    int len = bf.GetLength();
    TELEM inpt;
    for (int i = 0; i < len; i++)
    {
        istr >> inpt;
        if (inpt == _ASKII_ONE_)
            bf.SetBit(i);
        else if (inpt == _ASKII_ZERO_)
            bf.ClrBit(i);
        else
            throw ("Bit can only be equal to 0 or 1");
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    int len = bf.GetLength();
    for (int i = 0; i < len; i++) {
        if (bf.GetBit(i)) { // берём i-й бит, если 1 - true - выводим 1 
            ostr << '1';
        }
        else {
            ostr << '0';
        }
    }
    return ostr;
    
}
