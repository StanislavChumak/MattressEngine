#ifndef INIT_H
#define INIT_H

#include <glm/fwd.hpp>
class GLFWwindow;

void window_size_callback(GLFWwindow *window, int width, int height);
void input_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void input_key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
bool init(GLFWwindow *&window, glm::uvec2 windowSize, const char *nameWindow);

#endif
