#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "comp/Component.hpp"

#include "comp/single/Camera.hpp"

#include "util/reactive/ReactiveValue.hpp"

namespace mtrs::comp
{

struct Cursor : public Component<Cursor>
{
    react::ReactiveValue<glm::vec2, 1> window_position;
    react::ReactiveValue<glm::vec2, 1, glm::vec2, glm::uvec4, glm::vec2> position;
    react::ReactiveValue<glm::vec2, 0, glm::vec2, glm::uvec2, glm::mat4, glm::mat4> glob_pos;
    std::vector<void(*)()> subscribers;

    Cursor() = delete;
    ~Cursor() = default;
    Cursor(const Cursor &) = delete;
    Cursor &operator=(const Cursor&) = delete;
    Cursor(Cursor &&other) = delete;
    Cursor &operator=(Cursor &&other) = delete;

    Cursor(Camera *camera);

    void subscribe(void(*callback)());
    void unsubscribe(void(*callback)());

    static constexpr const char *get_type_name_imp() noexcept { return "Cursor"; }
};

}

#endif
