#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>

class InputContext {
public:
    InputContext();
    ~InputContext() = default;

    bool CreateContext(GLFWwindow* window);
    void Update();

    glm::vec2 GetMousePosition() const;
    bool IsMouseDown(int button) const;
    bool WasMouseClicked(int button) const;

private:
    GLFWwindow* m_Window;
    glm::vec2 m_MousePosition{};
    std::unordered_map<int, bool> m_CurrentMouseState;
    std::unordered_map<int, bool> m_PreviousMouseState;
    void SetMousePosition();
};