#include "util/mtrs_message.hpp"

#include <iostream>

namespace mtrs::util::detail
{

void show_message(TipeMessage tmsg, std::string&& message)
{
    switch (tmsg)
    {
    case TipeMessage::ERROR:
        std::cerr << message;
        break;
    case TipeMessage::WARNING:
        std::cout << message;
        break;
    case TipeMessage::LOG:
        std::cout << message;
        break;
    default:
        std::cerr << message;
        break;
    }
}

}