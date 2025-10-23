#include "Timer.h"

Timer::Timer()
: _isRanning(false), _timeLeft(0)
{
}

Timer::~Timer()
{
}

void Timer::update(const double &delta)
{
    if (_isRanning)
    {
        _timeLeft -= delta;
        if(_timeLeft <= 0)
        {
            _isRanning = false;
            _callback(_data);
        }
    }
}

void Timer::start(const double &duration, void *data)
{
    _data = data;
    _timeLeft = duration;
    _isRanning = true;
}

void Timer::set_callback(void (*callback)(void *data))
{
    _callback = callback;
}