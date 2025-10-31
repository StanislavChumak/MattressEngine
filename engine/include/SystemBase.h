#ifndef SYSTEM_BASE_H
#define SYSTEM_BASE_H

class ECSWorld;

class SystemBase
{
private:
    
public:
    ~SystemBase() = default;

    virtual void update(ECSWorld &world, const double &delta) = 0;
};

#endif