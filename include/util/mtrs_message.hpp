#ifndef MTRS_MESSAGE_HPP
#define MTRS_MESSAGE_HPP

#include <sstream>

#define TYPE_MESSAGE \
X(ERROR) \
X(WARNING) \
X(INFO)

namespace mtrs::util
{

enum class TypeMessage
{
#define X(TMSG) TMSG,
    TYPE_MESSAGE
    UNKNOWN
#undef X
};

namespace detail
{
    void show_message(TypeMessage tmsg, std::string&& message);
}

template<typename... Args>
void mtrs_message(TypeMessage tmsg, Args&&... args)
{
    std::stringstream ss;
    switch (tmsg)
    {
#define X(TMSG) case TypeMessage::TMSG: ss << #TMSG << ": "; break;
    TYPE_MESSAGE
#undef X  
    default:
        ss << "UNKNOWN: ";
        break;
    }
    ((ss << std::forward<Args>(args)), ...) << std::endl;

    detail::show_message(tmsg, ss.str());
}

template<typename... Args>
void mtrs_info(Args&&... args)
{
    mtrs_message(TypeMessage::INFO, args...);
}

template<typename... Args>
void mtrs_warning(Args&&... args)
{
    mtrs_message(TypeMessage::WARNING, args...);
}

template<typename... Args>
void mtrs_error(Args&&... args)
{
    mtrs_message(TypeMessage::ERROR, args...);
}

}

#endif
