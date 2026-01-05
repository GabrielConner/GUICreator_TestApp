#ifndef PPM_INCLUDE_PPACK_GUI_CREATOR_H
#define PPM_INCLUDE_PPACK_GUI_CREATOR_H

#include <vector>
#include <string>

#include "pPack/vector.h"
#include "pPack/textRendering.h"
#include "pPack/WindowManager.h"


namespace pPack {
namespace gui_creator {

struct Transform {
  vector::Vector2 position;
  vector::Vector2 scale;

  Transform& operator += (const Transform& transform) {
    position += transform.position * scale;
    scale *= transform.scale;

    return *this;
  }

  Transform() : position(), scale(1) {}
};


struct StringHandler {
  char const* stringList;
  char const* curString;
  char const* foundEnd;

  char const* const GetNext();

  char const* GetCurrentEnd() const;

  void SetTo(char const* ptr);

  void Reset();


  StringHandler() = delete;
  StringHandler(char const* StringList);
};



struct GUIRawObject {
  vector::Vector2 position;
  vector::Vector2 scale;

  int attributeCount;
  int parentIndex;


  char const* GetStringStart();
};


struct GUIShaderInformation {
  vector::Vector4 primaryColor;
  vector::Vector4 secondaryColor;
  vector::Vector4 borderColor;
  vector::Vector2 gradientStart;

  float borderThickness;
  float gradientStep;
  float gradientDistance;

  int gradientX;
  int gradientY;
  int manhattan;
  int borderEnable;

  //padding
  int a,b,c;
};



class GUIElement;
struct GUIBase {
  vector::Vector4 _primaryColor;
  vector::Vector4 _secondaryColor;
  vector::Vector4 _borderColor;
  vector::Vector2 _gradientStart;

  float _borderThickness;

  float _gradientStep;
  float _gradientDistance;

  bool _gradientX;
  bool _gradientY;
  bool _manhattanGradient;
  bool _borderEnable;

  unsigned int scrollableTexture;

public:
  std::vector<GUIElement*> children;
  vector::Vector4 primaryColor;
  vector::Vector4 secondaryColor;
  vector::Vector4 borderColor;
  vector::Vector2 gradientStart;

  std::string type;
  std::string data;

  GUIBase* parent;


  unsigned int texture;
  float borderThickness;

  float gradientStep;
  float gradientDistance;

  bool gradientX;
  bool gradientY;
  bool manhattanGradient;
  bool borderEnable;

  bool enabled;


  bool TreeEnabled();

  void RemoveChild(GUIElement* Child);

  GUIShaderInformation ShaderInformation();

  virtual void Reset();
  virtual void SetCurrent();

  GUIBase();
};



struct GUIBody : public GUIBase {
public:

  std::vector<GUIElement*> elements;

  void Update();


  GUIBody();
};



class GUIElement : public GUIBase {
  Transform _transform;

  float _textSize;
  vector::Vector4 _textColor;
  vector::Vector2 _padding;

  bool _stretch;
  bool _stick;
  bool _stuck;

  bool _centerTextX;
  bool _centerTextY;

  bool _isHovered;

  friend void Update();

public:
  bool stretch;
  bool stick;
  bool stuck;


  bool centerTextX;
  bool centerTextY;


  float textSize;

  std::string id;
  vector::Vector4 textColor;
  vector::Vector2 padding;

  Transform transform;
  Transform drawTransform;




  void (*onClick)(GUIElement* element);
  void (*onRelease)(GUIElement* element);

  void (*onEnter)(GUIElement* element);
  void (*onLeave)(GUIElement* element);


  void SetParent(GUIBase* Parent);

  void Reset() override;
  void SetCurrent() override;


  GUIElement();
};



struct StartAppInfo {
  int width;
  int height;
  std::string title;
  window_manager::WindowCreateHint* windowHints;
  int windowHintCount;

  std::string font;
  int fontSize;
  unsigned char fontLow;
  unsigned char fontHigh;
  int fontFiltering;

  StartAppInfo();
};



bool Start(StartAppInfo info);
void Update();
void End();

void SetFunction(std::string space, std::string name, void(*func)(GUIElement*));

void SetPage(std::string pageName, std::string file);
void OpenPage(std::string pageName);


void SetUpdateFunction(void(*func)(double deltaTime));


GUIBase* GetObjectByID(std::string id);

}; // namespace gui_creator
}; // namespace pPack



#endif