#include "pch.h"
#include "Input/InputContext.h"

InputContext::InputContext()
    : m_Window(nullptr) {
}

bool InputContext::CreateContext(GLFWwindow* window) {
    if (window) {
        m_Window = window;
        return true;
    }

    return false;
}

void InputContext::Update() {
    if (!m_Window) return;

    // Save previous state
    m_PreviousMouseState = m_CurrentMouseState;

    // Update current state
    for (int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; ++button) {
        m_CurrentMouseState[button] = glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
    }

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);
    m_MousePosition = glm::vec2(xpos, ypos);
}

glm::vec2 InputContext::GetMousePosition() const {
    return m_MousePosition;
}

bool InputContext::IsMouseDown(int button) const {
    auto it = m_CurrentMouseState.find(button);
    return it != m_CurrentMouseState.end() && it->second;
}

bool InputContext::WasMouseClicked(int button) const {
    bool prev = m_PreviousMouseState.find(button) != m_PreviousMouseState.end() && m_PreviousMouseState.at(button);
    bool curr = m_CurrentMouseState.find(button) != m_CurrentMouseState.end() && m_CurrentMouseState.at(button);
    return !prev && curr; // click = down now, but not before
}