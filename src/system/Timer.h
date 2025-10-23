#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
    Timer();
    ~Timer();
    void update(const double &delta);
    void start(const double &duration, void *data);
    void set_callback(void (*callback)(void *data));
    bool is_ranning() { return _isRanning; }

private:
    void (*_callback)(void *data);
    void *_data;
    double _timeLeft;
    bool _isRanning;
};

#endif
