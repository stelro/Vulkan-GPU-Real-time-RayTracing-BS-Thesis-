/* =======================================================================
   $File: IOManager.hh
   $Date: 7/1/2019	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */


#pragma once

// Engine Includes
#include "core.hh"
// C++ Includes
#include <unordered_map>
// GLFW Includes
#include <GLFW/glfw3.h>

namespace ost {

  class VulkanEngine;

  class IOManager {
  private:
                                    IOManager()                                   = default;
                                    ~IOManager()                                  = default;
    static IOManager*               m_instance;

    GLFWwindow*                     m_window                                      = nullptr;

    std::unordered_map<int, bool>   m_pressedKeys;
    std::unordered_map<int, bool>   m_previousKeys;

    bool                            wasKeyDown(int key)                           const noexcept;
    double                          m_MousePosX                                   = 0.0;
    double                          m_MousePoxY                                   = 0.0;

    struct {
      bool left       = false;
      bool right      = false;
      bool middle     = false;
    } m_mouseButtons;

    static void                     key_callback(GLFWwindow *window, int key,
                                    int scan_code, int action, int mods);
    static void                     mouse_callback(GLFWwindow * window, int button,
                                    int action, int mods);

    bool                            m_inputIsActive                               = false;

  public:
    static IOManager*               getInstance()                                 noexcept;
    static void                     destroy()                                     noexcept;

                                    OST_DISABLE_COPY(IOManager)
                                    OST_DISABLE_MOVE(IOManager)

    void                            update(float dt)                              noexcept;
    void                            setWindow(GLFWwindow *w)                      noexcept;

    OST_FORCE_INLINE void         pressKey(int key)                               noexcept;
    OST_FORCE_INLINE void         releaseKey(int key)                             noexcept;

    void                            pressLeftMouse(bool value)                    noexcept;
    void                            pressRightMouse(bool value)                   noexcept;
    void                            pressMiddleMouse(bool value)                  noexcept;

    bool                            isKeyPressed(int key)                         const noexcept;
    bool                            isKeyHoldDown(int key)                        const noexcept;

    bool                            isLeftMousePressed()                          const noexcept;
    bool                            isRightMousePressed()                         const noexcept;
    bool                            isMiddleMousePressed()                        const noexcept;

    float                           getMousePosX()                                const noexcept;
    float                           getMousePosY()                                const noexcept;

    bool                            inputIsActive()                               const noexcept;

  };

}



