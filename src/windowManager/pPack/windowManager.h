#ifndef PPM_INCLUDE_PPACK_WINDOW_MANAGER_H
#define PPM_INCLUDE_PPACK_WINDOW_MANAGER_H
#define GLFW_INCLUDE_NONE

#include "GLFW\glfw3.h"
#include <string>
#include <map>

#ifdef _PPM_VECTOR

#include "pPack/vector.h"

#endif


namespace pPack {
namespace window_manager {


#ifndef _PPM_VECTOR

struct DVector2 {
  double x;
  double y;
};

struct IVector2 {
  int x;
  int y;
};

#endif




struct InputInfo {
  bool pressed;
  bool held;
  bool released;
  int mods;

  InputInfo() : pressed(false), held(false), released(false), mods(0) {}
  InputInfo(bool Pressed, bool Held, bool Released, int Mods) : pressed(Pressed), held(Held), released(Released), mods(Mods) {}
};

struct WindowCreateHint {
  int hint;
  int value;
};



class Window {
  friend void _windowResizedCallback(GLFWwindow* Window, int Width, int Height);
  friend void _keyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);
  friend void _closeCallback(GLFWwindow* Window);
  friend  void _mouseCallback(GLFWwindow* glWindow, int Button, int Action, int Mods);
  friend void _mouseMoveCallback(GLFWwindow* glWindow, double xPos, double yPos);
  friend void _mouseEnterCallback(GLFWwindow* glWindow, int Entered);
  friend void _windowFocusCallback(GLFWwindow* glWindow, int Focused);
  friend void _windowMaximizeCallback(GLFWwindow* glWindow, int Maximized);
  friend void _windowIconifyCallback(GLFWwindow* glWindow, int Maximized);



  int width;
  int height;
  double aspectRatio;
  std::string title;

  std::map<int, InputInfo> keyMapping;
  std::map<int, InputInfo> mouseMapping;

  double realMouseX;
  double realMouseY;

  double mouseX;
  double mouseY;

  bool mouseOver;
  bool hasFocus;
  bool maximized;
  bool visible;

  bool valid;

  bool shouldClose;



public:
  GLFWwindow* windowRef;

  static Window* GetCurrentContext();



  InputInfo GetInput(int Key) {
    return keyMapping[Key];
  }



  void SetSize(int Width, int Height);
  void SetTitle(std::string Title);
  void SetFocus();
  void SetMousePosition(double X, double Y);
  void SetValidity(bool Valid);


  void RequestFocus();


  int GetWidth() const;
  int GetHeight() const;

  bool WidthLarger() const;


#ifndef _PPM_VECTOR
  IVector2
  #else
  ::pPack::vector::IVector2
  #endif 
    GetSize() const;


  double GetAspectRatio() const;

  std::string GetTitle() const;
  bool GetMaximized() const;
  bool GetVisible() const;
  bool GetValidity() const;

#ifndef _PPM_VECTOR
  DVector2
  #else
  ::pPack::vector::DVector2
  #endif
    GetMousePosition() const;

#ifndef _PPM_VECTOR
  DVector2
  #else
  ::pPack::vector::DVector2
  #endif
    GetRealMousePosition() const;





  void Open(int Width, int Height, std::string Title, WindowCreateHint* Hints, int HintCount);
  void Close();


  bool IsOpen();
  bool ShouldClose();


  void Show();


  void Maximize();
  void Restore();
  void Minimize();


  void ResetKeys();

  void Swap();


  void SetAsContext();


  #ifdef _OPENGL_GLAD
  void SetViewport();
  #endif


  Window();
  ~Window();
};




}; // namespace window_manager
}; // namespace pPack


#endif