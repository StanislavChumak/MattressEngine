#ifndef MTRS_MESSAGE_HPP
#define MTRS_MESSAGE_HPP

#include <sstream>

#define TYPE_MESSAGE \
X(ERROR) \
X(WARNING) \
X(LOG)

namespace mtrs::util
{

enum class TipeMessage
{
#define X(TMSG) TMSG,
    TYPE_MESSAGE
    UNKNOWN
#undef X
};

namespace detail
{
    void show_message(TipeMessage tmsg, std::string&& message);
}

template<typename... Desp>
void mtrs_message(TipeMessage tmsg, Desp&&... args)
{
    std::stringstream ss;
    switch (tmsg)
    {
#define X(TMSG) case TipeMessage::TMSG: ss << #TMSG << ": "; break;
    TYPE_MESSAGE
#undef X  
    default:
        ss << "UNKNOWN: ";
        break;
    }
    ((ss << std::forward<Desp>(args)), ...) << std::endl;

    detail::show_message(tmsg, ss.str());
}

}

#endif
