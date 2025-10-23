#ifndef SOLOPTR_H
#define SOLOPTR_H

namespace sys
{
    template<typename T>
    class Soloptr
    {
    public:
        Soloptr(T *ptr) noexcept :_ptr(ptr) {}
        Soloptr(const Soloptr& other) = delete;
        Soloptr& operator=(const Soloptr& other) = delete;
        Soloptr(Soloptr<T>&& other) noexcept :_ptr(other._ptr) { other._ptr = nullptr; }
        Soloptr<T>& operator=(Soloptr<T>&& other) noexcept
        {
            if (this != &other)
            {
                delete _ptr;
                _ptr = other._ptr;
                other._ptr = nullptr;
            }
            return *this;
        }
        ~Soloptr() { delete _ptr; }


        void reset(T *ptr = nullptr) noexcept { delete _ptr; _ptr = ptr;}
        T *get() const noexcept { return _ptr;}


        explicit operator bool() const noexcept { return _ptr != nullptr; }
        T& operator*() const { return *_ptr; }
        T* operator->() const { return _ptr; }
    private:
        T *_ptr;
    };

    template<typename T, typename ...Args>
    Soloptr<T> make_soloptr(Args... args)
    {
        return Soloptr<T>(new T(args...));
    }
}

#endif
