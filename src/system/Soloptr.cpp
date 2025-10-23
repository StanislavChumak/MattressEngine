#include "Soloptr.h"

using namespace sys;

template<typename T, typename ...Args>
Soloptr<T> make_soloptr(Args... args)
{
    
}

template<typename T>
Soloptr<T>::Soloptr(T *ptr)
: _ptr(ptr)
{
}

template<typename T>
Soloptr<T>::Soloptr(Soloptr<T>&& other) noexcept
: _ptr(other._ptr)
{
    other.clear();
}

template<typename T>
Soloptr<T>& Soloptr<T>::operator=(Soloptr<T>&& other) noexcept
{
    if (this == &other) return *this;
    _ptr = other._ptr;
    other.clear();
}

template<typename T>
void Soloptr<T>::set(T *ptr)
{
    delete _ptr;
    _ptr = ptr;
}

template<typename T>
void Soloptr<T>::clear()
{
    delete _ptr;
    _ptr = nullptr;
}