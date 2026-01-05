#include "pPack/windowManager.h"
#include <map>

#ifdef _PPM_VECTOR

#include "pPack/vector.h"

#endif


#ifdef _OPENGL_GLAD

#include "glad/glad.h"

#endif

using namespace ::pPack;
using namespace ::pPack::window_manager;

namespace {

std::map<GLFWwindow*, Window*> windowToWindowMap = std::map<GLFWwindow*, Window*>();

Window* currentContext = nullptr;

}; // namespace




namespace pPack {
namespace window_manager {

void _windowResizedCallback(GLFWwindow* glWindow, int Width, int Height) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];

  window->width = Width;
  window->height = Height;
  window->aspectRatio = (double)Width / Height;
  window->valid = false;
}


void _keyCallback(GLFWwindow* glWindow, int Key, int Scancode, int Action, int Mods) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];
  InputInfo& info = window->keyMapping[Key];
  info.mods = Mods;

  if (Action == GLFW_PRESS) {
    info.pressed = true;
    info.held = true;
    info.released = false;
  } else if (Action == GLFW_RELEASE) {
    info.pressed = false;
    info.held = false;
    info.released = true;
  }
}


void _closeCallback(GLFWwindow* glWindow) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];

  window->shouldClose = true;
}


void _mouseCallback(GLFWwindow* glWindow, int Button, int Action, int Mods) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];

  InputInfo& info = window->keyMapping[Button];
  info.mods = Mods;

  if (Action == GLFW_PRESS) {
    info.pressed = true;
    info.held = true;
    info.released = false;
  } else if (Action == GLFW_RELEASE) {
    info.pressed = false;
    info.held = false;
    info.released = true;
  }

}


void _mouseMoveCallback(GLFWwindow* glWindow, double xPos, double yPos) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];

  window->realMouseX = xPos;
  window->realMouseY = yPos;

  window->mouseX = (2 * xPos / window->width) - 1;
  window->mouseY = -((2 * yPos / window->height) - 1);
}


void _mouseEnterCallback(GLFWwindow* glWindow, int Entered) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];

  if (Entered == GLFW_TRUE) {
    window->mouseOver = true;
  } else {
    window->mouseOver = false;
  }
}


void _windowFocusCallback(GLFWwindow* glWindow, int Focused) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];

  if (Focused == GLFW_TRUE) {
    window->hasFocus = true;
  } else {
    window->hasFocus = false;
  }
}


void _windowMaximizeCallback(GLFWwindow* glWindow, int Maximized) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];

  if (Maximized == GLFW_TRUE) {
    window->maximized = true;
  } else {
    window->maximized = false;
  }
}


void _windowIconifyCallback(GLFWwindow* glWindow, int Iconified) {
  if (!windowToWindowMap.contains(glWindow)) {
    return;
  }

  Window* window = windowToWindowMap[glWindow];

  if (Iconified == GLFW_TRUE) {
    window->visible = false;
  } else {
    window->visible = true;
  }
}





//--------------------
Window* Window::GetCurrentContext() {
  return currentContext;
}






//--------------------
void Window::SetSize(int Width, int Height) {
  if (windowRef == nullptr) {
    return;
  }

  glfwSetWindowSize(windowRef, Width, Height);
}


//--------------------
void Window::SetTitle(std::string Title) {
  if (windowRef == nullptr) {
    return;
  }

  title = Title;
  glfwSetWindowTitle(windowRef, Title.c_str());
}


//--------------------
void Window::SetFocus() {
  if (windowRef == nullptr) {
    return;
  }

  glfwFocusWindow(windowRef);
}


//--------------------
void Window::SetMousePosition(double X, double Y) {
  if (windowRef == nullptr) {
    return;
  }

  double realX = (X + aspectRatio) / 2 * height;
  double realY = (Y + 1) / 2 * height;

  glfwSetCursorPos(windowRef, realX, realY);
}



//--------------------
void Window::SetValidity(bool Valid) {
  valid = Valid;
}




//--------------------
void Window::RequestFocus() {
  glfwRequestWindowAttention(windowRef);
}




//--------------------
int Window::GetWidth() const { return width; }
int Window::GetHeight() const { return height; }
bool Window::WidthLarger() const { return width > height; }

#ifndef _PPM_VECTOR
IVector2
#else
::pPack::vector::IVector2
#endif
Window::GetSize() const { return { width, height }; }

double Window::GetAspectRatio() const { return aspectRatio; }
std::string Window::GetTitle() const { return title; }
bool Window::GetMaximized() const { return maximized; }
bool Window::GetVisible() const { return visible; }
bool Window::GetValidity() const { return valid; }

#ifndef _PPM_VECTOR
DVector2
#else
::pPack::vector::DVector2
#endif
Window::GetMousePosition() const { return { mouseX, mouseY }; }

#ifndef _PPM_VECTOR
DVector2
#else
::pPack::vector::DVector2
#endif
Window::GetRealMousePosition() const { return { realMouseX, realMouseY }; }




//--------------------
void Window::Open(int Width, int Height, std::string Title, WindowCreateHint* Hints, int HintCount) {
  if (windowRef != nullptr) {
    Close();
  }

  glfwDefaultWindowHints();
  for (int i = 0; i < HintCount; i++) {
    glfwWindowHint(Hints[i].hint, Hints[i].value);
  }

  windowRef = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
  width = Width;
  height = Height;
  title = Title;
  aspectRatio = (double)width / height;
  hasFocus = true;



  windowToWindowMap[windowRef] = this;

  glfwSetWindowSizeCallback(windowRef, _windowResizedCallback);
  glfwSetKeyCallback(windowRef, _keyCallback);
  glfwSetWindowCloseCallback(windowRef, _closeCallback);
  glfwSetMouseButtonCallback(windowRef, _mouseCallback);
  glfwSetCursorPosCallback(windowRef, _mouseMoveCallback);
  glfwSetCursorEnterCallback(windowRef, _mouseEnterCallback);
  glfwSetWindowFocusCallback(windowRef, _windowFocusCallback);
  glfwSetWindowMaximizeCallback(windowRef, _windowMaximizeCallback);
  glfwSetWindowIconifyCallback(windowRef, _windowIconifyCallback);

}


//--------------------
void Window::Close() {
  if (windowRef == nullptr) {
    return;
  }
  if (windowToWindowMap.contains(windowRef))
    windowToWindowMap.erase(windowRef);

  glfwDestroyWindow(windowRef);
  windowRef = nullptr;
  shouldClose = false;
}




//--------------------
bool Window::IsOpen() {
  return windowRef != nullptr;
}


bool Window::ShouldClose() {
  return shouldClose;
}




//--------------------
void Window::Show() {
  if (windowRef == nullptr) {
    return;
  }

  glfwShowWindow(windowRef);
}




//--------------------
void Window::Maximize() {
  if (windowRef == nullptr) {
    return;
  }

  glfwMaximizeWindow(windowRef);
}


void Window::Restore() {
  if (windowRef == nullptr) {
    return;
  }

  glfwRestoreWindow(windowRef);
}


//--------------------
void Window::Minimize() {
  if (windowRef == nullptr) {
    return;
  }

  glfwIconifyWindow(windowRef);
}



//--------------------
void Window::ResetKeys() {
  if (windowRef == nullptr) {
    return;
  }

  for (auto& keyI : keyMapping) {
    keyI.second.pressed = false;
    keyI.second.released = false;
  }
  for (auto& mouseI : mouseMapping) {
    mouseI.second.pressed = false;
    mouseI.second.released = false;
  }
}



//--------------------
void Window::Swap() {
  if (windowRef == nullptr) {
    return;
  }

  glfwSwapBuffers(windowRef);
}


//--------------------
void Window::SetAsContext() {
  if (windowRef == nullptr) {
    return;
  }
  currentContext = this;

  glfwMakeContextCurrent(windowRef);
}




//--------------------
#ifdef _OPENGL_GLAD
void Window::SetViewport() {
  glViewport(0, 0, width, height);
}
#endif




//--------------------
Window::Window() : windowRef(nullptr), width(0), height(0), aspectRatio(0), title(), keyMapping(), mouseMapping(), realMouseX(0), realMouseY(0), mouseX(0), mouseY(0), mouseOver(false), hasFocus(false), maximized(false), visible(true), valid(false), shouldClose(false) {}


//--------------------
Window::~Window() {
  Close();
}


}; // namespace window_manager
}; // namepspace pPack