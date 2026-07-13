#ifndef STATES_HPP
#define STATES_HPP

#include "comp/Component.hpp"

#include <string>

namespace mtrs::comp
{

struct States : public Component<States>
{
    std::string current_system_state;

    States() = delete;
    ~States() = default;
    States(const States &) = delete;
    States &operator=(const States&) = delete;
    States(States&&) = delete;
    States &operator=(States&&) = delete;

    States(std::string start_state);

    static constexpr const char *get_type_name_imp() noexcept { return "States"; }
};

}

#endif
