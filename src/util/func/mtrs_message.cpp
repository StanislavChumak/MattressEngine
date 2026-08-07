#ifndef FLAG_RELEASE
#include "util/func/mtrs_message.hpp"

#include <iostream>

namespace mtrs::util::detail
{

void show_message(TypeMessage tmsg, std::string&& message)
{
    switch (tmsg)
    {
    case TypeMessage::ERROR:
        std::cerr << message;
        break;
    case TypeMessage::WARNING:
        std::cout << message;
        break;
    case TypeMessage::INFO:
        std::cout << message;
        break;
    default:
        std::cerr << message;
        break;
    }
}

}

#endif