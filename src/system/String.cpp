#include "String.h"

using namespace sys;

int stringlength(const char *str)
{
    const char *ptrStr = str;
    while (*ptrStr)
        ptrStr++;
    return ptrStr - str;
}

void stringcopy(char *dest, const char *src)
{
    for (; *src; dest++, src++)
        *dest = *src;
    *dest = 0;
}

int stringcmp(const char *string1, const char *string2)
{
    for (; *string1 && *string2; string1++, string2++)
        if(*string1 != *string2)
            return *string1 - *string2;
    return *string1 - *string2;
}

Str::Str(const char* str)
: _size(stringlength(str))
{
    _data = new char[_size + 1];
    stringcopy(_data, str);
}

Str::Str(const Str& other)
{
    _size = other._size;
    _data = new char[_size + 1];
    stringcopy(_data, other._data);
}

Str& Str::operator=(const Str& other)
{
    if (this == &other) return *this;
    delete[] _data;
    _size = other._size;
    _data = new char[_size + 1];
    stringcopy(_data, other._data);
    return *this;
}

Str::Str(Str&& other) noexcept
: _data(other._data), _size(other._size)
{
    other._data = nullptr;
    other._size = 0;
}

Str& Str::operator=(Str&& other) noexcept
{
    if (this != &other)
    {
        delete[] _data;
        _data = other._data;
        _size = other._size;
        other._data = nullptr;
        other._size = 0;
    }
    return *this;
}

Str::~Str()
{
    delete[] _data;
}

const char *Str::sub_str(int size, unsigned int offset) const
{
    char *out = new char[size + 1];
    out[size] = 0;
    for (size--; size >= 0; size--)
        out[size] = _data[size] + offset;
    return out;
}

bool Str::operator==(const Str &other) const
{
    return stringcmp(_data, other._data) == 0;
}

bool Str::operator==(const char *other) const
{
    return stringcmp(_data, other) == 0;
}

bool Str::operator>(const Str &other) const
{
    return stringcmp(_data, other._data) > 0;
}

bool Str::operator<(const Str &other) const
{
    return stringcmp(_data, other._data) < 0;
}

const char *Str::operator+(const char *other) const
{
    char *out = new char[_size + stringlength(other) + 1];
    stringcopy(out, _data);
    stringcopy(out + _size, other);
    return out;
}

const char *Str::operator+(const Str &other) const
{
    char *out = new char[_size + other._size + 1];
    stringcopy(out, _data);
    stringcopy(out + _size, other._data);
    return out;
}