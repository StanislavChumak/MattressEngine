#ifndef STRING_H
#define STRING_H

namespace sys
{
    class Str
    {
    public:
        Str(const char *str  = "");
        Str(const Str& other);
        Str& operator=(const Str& other);
        Str(Str&& other) noexcept;
        Str& operator=(Str&& other) noexcept;
        ~Str();

        unsigned int size() const { return _size; }
        const char *c_str() const { return _data; }
        bool empty() const { return _data == 0;}
        const char *sub_str(int size, unsigned int offset = 0) const;

        bool operator==(const Str &other) const;
        bool operator==(const char *other) const;
        bool operator>(const Str &other) const;
        bool operator<(const Str &other) const;
        const char *operator+(const char *other) const;
        const char *operator+(const Str &other) const;
        char &operator[](unsigned int n) const { return _data[n]; }

    private:
        char *_data;
        unsigned int _size;
    };
}
#endif
