/* =======================================================================
   $File: IOManager.cc
   $Date: 7/1/2019	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#include <cassert>
#include "IOManager.hh"
#include "VulkanEngine.hh"

namespace ost {

  IOManager *IOManager::m_instance = nullptr;

  IOManager *IOManager::getInstance() noexcept {

    if ( m_instance )
      return m_instance;

    m_instance = new IOManager();
    return m_instance;
  }

  void IOManager::destroy() noexcept {

    OST_ASSERT_LOG(m_instance != nullptr, "IOManager already destroyed!");
    delete m_instance;
    m_instance = nullptr;
  }

  void IOManager::pressKey(int key) noexcept {
    m_pressedKeys[key] = true;
  }

  void IOManager::releaseKey(int key) noexcept {
    m_pressedKeys[key] = false;
  }

  bool IOManager::isKeyPressed(int key) const noexcept {
    return isKeyHoldDown(key);
  }

  void IOManager::update([[maybe_unused]] float dt) noexcept {
    OST_ASSERT_LOG(m_window != nullptr, "Window has not been set in IOManager class!");

    glfwPollEvents();

    for ( auto &it : m_pressedKeys ) {
      m_previousKeys[it.first] = it.second;
    }

    // Handle special case where we should close window when
    // ESCAPE is pressed
    if ( glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
      glfwSetWindowShouldClose(m_window, true);
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouse_callback);

    glfwGetCursorPos(m_window, &m_MousePosX, &m_MousePoxY);
  }

  void IOManager::setWindow(GLFWwindow* w) noexcept {
    m_window = w;
  }

  void IOManager::key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scan_code, int action, [[maybe_unused]] int mods) {
    auto *io_manager = static_cast<IOManager*>(glfwGetWindowUserPointer(window));
    io_manager->m_inputIsActive = ( action != GLFW_RELEASE );

    switch (action) {
      case GLFW_PRESS:
        io_manager->pressKey(key);
        break;
      case GLFW_RELEASE:
        io_manager->releaseKey(key);
        break;
      case GLFW_REPEAT:
        //io_manager->pressKey(key);
        break;
      default:
        break;
    }
  }

  bool IOManager::wasKeyDown(int key) const noexcept {
    auto it = m_previousKeys.find(key);
    if ( it != m_previousKeys.end() ) {
      return it->second;
    } else {
      return false;
    }
  }

  bool IOManager::isKeyHoldDown(int key) const noexcept {
    auto it = m_pressedKeys.find(key);
    if ( it != m_pressedKeys.end() ) {
      return it->second;
    } else {
      return false;
    }
  }

  float IOManager::getMousePosX() const noexcept {
    return static_cast<float>(m_MousePosX);
  }

  float IOManager::getMousePosY() const noexcept {
    return static_cast<float>(m_MousePoxY);
  }

  void IOManager::mouse_callback(GLFWwindow *window, int button, int action, int mods) {

    auto *io_manager = static_cast<IOManager*>(glfwGetWindowUserPointer(window));
    auto flag = ( action != GLFW_RELEASE );
    io_manager->m_inputIsActive = flag;

    switch (button) {
      case GLFW_MOUSE_BUTTON_RIGHT:
        io_manager->pressRightMouse( flag );
        break;
      case GLFW_MOUSE_BUTTON_LEFT:
        io_manager->pressLeftMouse( flag );
        break;
      case GLFW_MOUSE_BUTTON_MIDDLE:
        io_manager->pressMiddleMouse( flag );
        break;

    }
  }

  void IOManager::pressLeftMouse(bool value) noexcept {
    m_mouseButtons.left = value;
  }

  void IOManager::pressRightMouse(bool value) noexcept {
    m_mouseButtons.right = value;
  }

  void IOManager::pressMiddleMouse(bool value) noexcept {
    m_mouseButtons.middle = value;
  }

  bool IOManager::isLeftMousePressed() const noexcept {
    return m_mouseButtons.left;
  }

  bool IOManager::isRightMousePressed() const noexcept {
    return m_mouseButtons.right;
  }

  bool IOManager::isMiddleMousePressed() const noexcept {
    return m_mouseButtons.middle;
  }

  bool IOManager::inputIsActive() const noexcept {
    return m_inputIsActive;
  }

}