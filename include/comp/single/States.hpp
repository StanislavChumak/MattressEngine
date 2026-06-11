#ifndef STATES_HPP
#define STATES_HPP

#include <string>

namespace mtrs::comp
{

struct States
{
    std::string current_system_state;
    States(std::string start_state) : current_system_state(start_state) {}
};

}

#endif
