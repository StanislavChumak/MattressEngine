#ifndef STATES_HPP
#define STATES_HPP

#include <string>

namespace mtrs::comp
{

struct States
{
    std::string current_system_state;

    States() = delete;
    ~States() = default;
    States(const States &) = delete;
    States &operator=(const States&) = delete;
    States(States&&) = delete;
    States &operator=(States&&) = delete;

    States(std::string start_state);
};

}

#endif
