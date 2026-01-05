

#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include "pPack/vector.h"
#include "pPack/GUICreator.h"
#include "pPack/WindowManager.h"
#include "pPack/shaderHandling.h"
#include "pPack/textRendering.h"
#include "pPack/timer.h"
#include "pPack/animateValue.h"

#include "stb_image.h"

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include <fstream>
#include <sstream>
#include <algorithm>


#include <Windows.h>


using namespace ::pPack;
using namespace ::pPack::gui_creator;
using namespace ::pPack::window_manager;
using namespace ::pPack::vector;
using namespace ::pPack::shader_handling;
using namespace ::pPack::timer;


namespace {

bool ToBool(std::string str);
Vector4 ToVector4(const char* str);
Vector2 ToVector2(const char* str);
void (*ToFunction(std::string pageName, const char* str))(GUIElement*);

bool PositionInElement(Vector2 pos, GUIElement* ele);
float SignOf(float i);
bool FileExists(const char* fileName);
bool PrecompileData(std::string fileName, void** ppBuffer, uint32_t* bufferSize);
char const* ToNextInstanceOf(char const* str, char what);
void HandleGeneralAttributes(GUIBase* body, int attributeCount, StringHandler& handler);
void HandleElementAttributes(std::string pageName, GUIElement* element, int attributeCount, StringHandler& handler);
std::string TryGetStringFrom(char const* const str);

void DrawBodyObject(GUIBody* base, float aspect, bool widthWay);
void DrawObject(GUIElement* obj, float aspect, IVector2 windowSize, bool widthWay);


Window window = Window();
ShaderHandler shader;

text_rendering::Bitmap bitmap;


unsigned int VAO, VBO, UBO;
float square[] = {
  -1.0f, -1.0f, 0.f, 0.f,
  1.0f, -1.0f, 1.f, 0.f,
  -1.0f, 1.0f, 0.f, 1.f,
  1.0f, 1.0f, 1.f, 1.f
};


std::map<std::string, std::map<std::string, void(*)(GUIElement*)>> functionMapping = std::map<std::string, std::map<std::string, void(*)(GUIElement*)>>();

std::map<std::string, GUIBody*> pageMapping = std::map<std::string, GUIBody*>();



GUIBody* selectedPage = nullptr;
std::string selectedPageName = std::string();


std::vector<GUIElement*> clickList = std::vector<GUIElement*>();
std::vector<GUIElement*> hoverList = std::vector<GUIElement*>();


Timer* singletonTimer = nullptr;



void (*updateFunc)(double) = nullptr;


}; // namespace







namespace pPack {
namespace gui_creator {



bool Start(StartAppInfo info) {
  if (!glfwInit()) {
    return false;
  }

  window.Open(info.width, info.height, info.title, info.windowHints, info.windowHintCount);
  window.SetAsContext();

  if (!gladLoadGL()) {
    window.Close();
    glfwTerminate();

    return false;
  }


  text_rendering::Start();
  bitmap = text_rendering::GenerateBitmap(info.font.c_str(), info.fontSize, info.fontLow, info.fontHigh, info.fontFiltering);


  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  singletonTimer = &Timer::GetSingleton();
  singletonTimer->SetUpdateDelay(1.0 / 120.0);



  const char* vertLocs[] = { "./shaders/shader.vert" };
  const char* fragLocs[] = { "./shaders/shader.frag" };

  ShaderCreateInfo shaderInfo[] = { ShaderCreateInfo(vertLocs, 1, GL_VERTEX_SHADER), ShaderCreateInfo(fragLocs, 1, GL_FRAGMENT_SHADER) };

  shader = ShaderHandler::CreateShader("shader", shaderInfo, 2);


  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  glGenBuffers(1, &UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, UBO);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);


  return true;
}



void Update() {
  while (!window.ShouldClose()) {
    if (selectedPage == nullptr) {
      return;
    }

    window.ResetKeys();

    singletonTimer->WaitForNextUpdate();
    window.SetAsContext();

    glfwPollEvents();

    if (!window.GetValidity()) {
      window.SetViewport();
      window.SetValidity(true);
    }



    shader.Active();
    glBindVertexArray(VAO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    selectedPage->Update();
    glBindVertexArray(0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    Vector2 mousePos = window.GetMousePosition().ConvertTo<float>();

    if (window.GetInput(GLFW_MOUSE_BUTTON_1).released) {
      for (GUIElement* elem : clickList) {
        if (elem->onRelease != nullptr) {
          elem->onRelease(elem);
        }
      }

      clickList.clear();
    }


    for (GUIElement* elem : hoverList) {
      if (!PositionInElement(mousePos, elem)) {
        if (elem->onLeave) {
          elem->onLeave(elem);
        }
        elem->_isHovered = false;
      }
    }
    hoverList.clear();


    for (GUIElement* elem : selectedPage->elements) {
      if (!elem->TreeEnabled()) {
        continue;
      }
      if (PositionInElement(mousePos, elem)) {
        if (!elem->_isHovered && elem->onEnter != nullptr) {
          elem->onEnter(elem);
        }
        elem->_isHovered = true;
        hoverList.push_back(elem);
      }
    }




    if (window.GetInput(GLFW_MOUSE_BUTTON_1).pressed) {
      for (GUIElement* elem : hoverList) {
        if (elem->onClick != nullptr) {
          elem->onClick(elem);
        }

        clickList.push_back(elem);
      }
    }

    if (updateFunc != nullptr) {
      updateFunc(singletonTimer->GetDeltaTime());
    }


    window.Swap();
    singletonTimer->Advance();
    animate_value::AnimateValueUpdate();

  }
}



void End() {
  animate_value::DeleteAllAnimations();

  for (auto& [name, page] : pageMapping) {
    for (GUIElement* elem : page->elements) {
      glDeleteTextures(1, &elem->texture);
      delete(elem);
    }

    delete(page);
  }

  bitmap.Destroy();

  window.Close();

  text_rendering::End();

  glfwTerminate();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader);
}






void SetFunction(std::string space, std::string name, void(*func)(GUIElement*)) {
  if (func == nullptr) {
    return;
  }

  functionMapping[space][name] = func;
}



void SetPage(std::string pageName, std::string file) {
  if (pageName.size() == 0 || pageName == "") {
    return;
  }

  if (pageMapping.contains(pageName)) {
    GUIBody* page = pageMapping[pageName];

    for (GUIElement* elem : page->elements) {
      glDeleteTextures(1, &elem->texture);
      delete(elem);
    }

    delete(page);
  }


  uint8_t* fileBuffer = nullptr;
  uint32_t bufferSize = 0;

  if (!PrecompileData(file, (void**)&fileBuffer, &bufferSize) || bufferSize == 0 || fileBuffer == nullptr) {
    return;
  }


  int objectCount = *(int*)fileBuffer - 1;


  std::vector<GUIBase*> objsCreated = std::vector<GUIBase*>();

  GUIRawObject* curObj = (GUIRawObject*)(fileBuffer + sizeof(int));

  GUIBody* createdBody = new GUIBody();
  objsCreated.push_back(createdBody);
  pageMapping[pageName] = createdBody;

  StringHandler stringHandler = curObj->GetStringStart();

  HandleGeneralAttributes(createdBody, curObj->attributeCount, stringHandler);

  createdBody->type = TryGetStringFrom(stringHandler.GetNext());
  createdBody->data = TryGetStringFrom(stringHandler.GetNext());

  createdBody->SetCurrent();

  curObj = (GUIRawObject*)stringHandler.GetCurrentEnd();

  GUIElement* createdElement = nullptr;
  for (int i = 0; i < objectCount; i++) {
    createdElement = new GUIElement();

    objsCreated.push_back(createdElement);
    createdBody->elements.push_back(createdElement);
    stringHandler.SetTo(curObj->GetStringStart());


    createdElement->transform.position = curObj->position;
    createdElement->transform.scale = curObj->scale;

    if (curObj->parentIndex != -1) {
      createdElement->SetParent(objsCreated[curObj->parentIndex]);
    }

    HandleGeneralAttributes(createdElement, curObj->attributeCount, stringHandler);
    stringHandler.Reset();
    HandleElementAttributes(pageName, createdElement, curObj->attributeCount, stringHandler);


    createdElement->type = TryGetStringFrom(stringHandler.GetNext());
    createdElement->data = TryGetStringFrom(stringHandler.GetNext());

    createdElement->SetCurrent();

    curObj = (GUIRawObject*)stringHandler.GetCurrentEnd();
  }



  free(fileBuffer);
}



void OpenPage(std::string pageName) {
  if (!pageMapping.contains(pageName)) {
    return;
  }

  selectedPageName = pageName;
  selectedPage = pageMapping[pageName];
}



GUIBase* GetObjectByID(std::string id) {
  if (selectedPage == nullptr) {
    return nullptr;
  }

  if (id == "body") {
    return selectedPage;
  }

  for (GUIElement* elem : selectedPage->elements) {
    if (elem->id == id) {
      return elem;
    }
  }

  return nullptr;
}



void SetUpdateFunction(void(*func)(double deltaTime)) {
  updateFunc = func;
}





// GUIBase
//------------------------------------------------------------------------------------------

void GUIBase::RemoveChild(GUIElement* Child) {
  if (Child == nullptr) {
    return;
  }

  std::vector<GUIElement*>::iterator search = std::find(children.begin(), children.end(), Child);
  if (search == children.end()) {
    return;
  }
  children.erase(search);
}


GUIShaderInformation GUIBase::ShaderInformation() {
  return GUIShaderInformation {primaryColor, secondaryColor, borderColor,
  gradientStart, borderThickness, gradientStep, gradientDistance, gradientX,
  gradientY, manhattanGradient, borderEnable};
}


void GUIBase::Reset() {
  primaryColor = _primaryColor;
  secondaryColor = _secondaryColor;
  borderColor = _borderColor;
  gradientStart = _gradientStart;
  borderThickness = _borderThickness;
  gradientStep = _gradientStep;
  gradientDistance = _gradientDistance;
  gradientX = _gradientX;
  gradientY = _gradientY;
  manhattanGradient = _manhattanGradient;
  borderEnable = _borderEnable;
}

void GUIBase::SetCurrent() {
  _primaryColor = primaryColor;
  _secondaryColor = secondaryColor;
  _borderColor = borderColor;
  _gradientStart = gradientStart;
  _borderThickness = borderThickness;
  _gradientStep = gradientStep;
  _gradientDistance = gradientDistance;
  _gradientX = gradientX;
  _gradientY = gradientY;
  _manhattanGradient = manhattanGradient;
  _borderEnable = borderEnable;
}



bool GUIBase::TreeEnabled() {
  if (parent == nullptr) {
    return true;
  }
  if (!enabled) {
    return false;
  }

  return parent->TreeEnabled();
}



GUIBase::GUIBase() : children(), primaryColor(0.f), secondaryColor(0.f), borderColor(0.f, 0.f, 0.f, 1.0f), gradientStart(0, 0),
type(), data(), parent(nullptr), texture(0), borderThickness(0.05f), gradientStep(0.f), gradientDistance(1.0f), gradientX(false),
gradientY(false), manhattanGradient(false), borderEnable(false), enabled(true),
_primaryColor(0.f), _secondaryColor(0.f), _borderColor(0.f,0.f,0.f,1.f),
_gradientStart(), _borderThickness(0.05f), _gradientStep(0.f),
_gradientDistance(1.f), _gradientX(false), _gradientY(false),
_manhattanGradient(false), _borderEnable(false), scrollableTexture(0)
{}


//------------------------------------------------------------------------------------------



// GUIBody
//------------------------------------------------------------------------------------------

void GUIBody::Update() {
  glClearColor(0, 0, 0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  float aspect = window.GetAspectRatio();
  bool widthWay = window.WidthLarger();

  DrawBodyObject(this, aspect, widthWay);

  for (GUIElement* child : children) {
    child->drawTransform = Transform();
    DrawObject(child, aspect, window.GetSize(), widthWay);
  }
}


GUIBody::GUIBody() : GUIBase() {}


//------------------------------------------------------------------------------------------



// GUIElement
//------------------------------------------------------------------------------------------

void GUIElement::SetParent(GUIBase* Parent) {
  if (Parent == nullptr) {
    return;
  }
  if (parent != nullptr) {
    parent->RemoveChild(this);
  }

  parent = Parent;
  Parent->children.push_back(this);
}


void GUIElement::Reset() {
  transform = _transform;
  textSize = _textSize;
  textColor = _textColor;
  stretch = _stretch;
  stick = _stick;
  stuck = _stuck;
  centerTextX = _centerTextX;
  centerTextY = _centerTextY;

  GUIBase::Reset();
}

void GUIElement::SetCurrent() {
  _transform = transform;
  _textSize = textSize;
  _textColor = textColor;
  _stretch = stretch;
  _stick = stick;
  _stuck = stuck;
  _centerTextX = centerTextX;
  _centerTextY = centerTextY;

  GUIBase::SetCurrent();
}



GUIElement::GUIElement() : GUIBase(), id(), textColor(0,0,0,1), padding(), textSize(1.0f),
transform(), drawTransform(), stretch(true), stick(false), stuck(false), centerTextX(true), centerTextY(true),
onClick(nullptr), onRelease(nullptr), onEnter(nullptr), onLeave(nullptr),
_transform(), _textSize(1.0f), _textColor(0,0,0,1), _padding(), _stretch(true), _stick(false), _stuck(false), _centerTextX(true),
_centerTextY(true), _isHovered(false)
{}

//------------------------------------------------------------------------------------------



// StringHandler
//------------------------------------------------------------------------------------------

char const* const StringHandler::GetNext() {
  if (curString == nullptr)
    return nullptr;

  char const* temp = curString;

  while (true) {
    if (*curString == '\0') {
      curString++;
      foundEnd = curString;

      break;
    }
    curString++;
  }


  return temp;
}


char const* StringHandler::GetCurrentEnd() const {
  return foundEnd;
}


void StringHandler::SetTo(char const* ptr) {
  stringList = ptr;
  curString = ptr;
  foundEnd = ptr;
}


void StringHandler::Reset() {
  curString = stringList;
  foundEnd = curString;
}


StringHandler::StringHandler(char const* StringList) : stringList(StringList), curString(StringList), foundEnd(StringList) {}

//------------------------------------------------------------------------------------------



// GUIRawObject
//------------------------------------------------------------------------------------------

char const* GUIRawObject::GetStringStart() {
  return (char const*)(this + 1);
}

//------------------------------------------------------------------------------------------



// StartAppInfo
//------------------------------------------------------------------------------------------

StartAppInfo::StartAppInfo() : width(1280), height(720), title("GUI App"), windowHints(nullptr),
windowHintCount(0), font("./fonts/CascadiaMono.ttf"), fontSize(64), fontLow(0), fontHigh(127),
fontFiltering(GL_LINEAR) {}

//------------------------------------------------------------------------------------------



}; // namespace gui_creator
}; // namespace pPack












// Helper functions


namespace {


bool PositionInElement(Vector2 pos, GUIElement* ele) {
  Vector2 bl = ele->drawTransform.position - ele->drawTransform.scale;
  Vector2 tr = ele->drawTransform.position + ele->drawTransform.scale;

  return pos.x >= bl.x && pos.y >= bl.y && pos.x <= tr.x && pos.y <= tr.y;
}



float SignOf(float i) {
  return i >= 0 ? 1 : -1;
}



bool ToBool(std::string str) {

  // https://stackoverflow.com/a/3613424
  // https://stackoverflow.com/a/2942442
  std::transform(str.begin(), str.end(), str.begin(), static_cast<int(*)(int)>(std::tolower));

  std::istringstream s(str);
  bool b;
  s >> std::boolalpha >> b;
  return b;
}



Vector4 ToVector4(const char* str) {

  Vector4 vec;
  if (str != nullptr)
    vec.x = atof(str);
  str = ToNextInstanceOf(str, ',');
  if (str != nullptr) {
    str++;
    vec.y = atof(str);
  }
  str = ToNextInstanceOf(str, ',');
  if (str != nullptr) {
    str++;
    vec.z = atof(str);
  }
  str = ToNextInstanceOf(str, ',');
  if (str != nullptr) {
    str++;
    vec.w = atof(str);
  }

  return vec;
}



Vector2 ToVector2(const char* str) {

  Vector2 vec;
  if (str != nullptr)
    vec.x = atof(str);
  str = ToNextInstanceOf(str, ',');
  if (str != nullptr) {
    str++;
    vec.y = atof(str);
  }

  int (*f(int))(double);

  return vec;
}



// Awfulness
void (*ToFunction(std::string pageName, const char* str))(GUIElement*) {
  const char* funcName = ToNextInstanceOf(str, ':');
  if (funcName == nullptr) {
    return functionMapping[pageName][str];
  }
  funcName++;
  return functionMapping[std::string(str, (funcName - str - 1))][funcName];
}



bool FileExists(const char* fileName) {
  struct _stat buffer;
  if (_stat(fileName, &buffer) == 0) {
    return true;
  }
  return false;
}



char const* ToNextInstanceOf(char const* str, char what) {
  if (str == nullptr) {
    return nullptr;
  }

  while (true) {
    if (*str == what)
      return str;
    if (*str == '\0')
      return nullptr;
    str++;
  }
}



void HandleGeneralAttributes(GUIBase* obj, int attributeCount, StringHandler& handler) {
  for (int i = 0; i < attributeCount; i++) {
    char const* key = handler.GetNext();
    char const* value = handler.GetNext();

    if (strcmp(key, "primaryColor") == 0) {
      obj->primaryColor = ToVector4(value);
    } else if (strcmp(key, "secondaryColor") == 0) {
      obj->secondaryColor = ToVector4(value);
    } else if (strcmp(key, "borderColor") == 0) {
      obj->borderColor = ToVector4(value);
    } else if (strcmp(key, "gradientStart") == 0) {
      obj->gradientStart = ToVector2(value);
    } else if (strcmp(key, "gradientX") == 0) {
      obj->gradientX = ToBool(value);
    } else if (strcmp(key, "gradientY") == 0) {
      obj->gradientY = ToBool(value);
    } else if (strcmp(key, "manhattanGradient") == 0) {
      obj->manhattanGradient = ToBool(value);
    } else if (strcmp(key, "border") == 0) {
      obj->borderEnable = ToBool(value);
    } else if (strcmp(key, "borderThickness") == 0) {
      obj->borderThickness = atof(value);
    } else if (strcmp(key, "gradientStep") == 0) {
      obj->gradientStep = atof(value);
    } else if (strcmp(key, "gradientDistance") == 0) {
      obj->gradientDistance = atof(value);
    } else if (strcmp(key, "image") == 0) {
      if (!FileExists(value)) {
        continue;
      }

      int width, height, channels, rChannels = 4;
      void* pixelData = stbi_load(value, &width, &height, &channels, rChannels);

      glGenTextures(1, &obj->texture);
      glBindTexture(GL_TEXTURE_2D, obj->texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      glBindTexture(GL_TEXTURE_2D, 0);
      stbi_image_free(pixelData);
    }
  }


}



void HandleElementAttributes(std::string pageName, GUIElement* element, int attributeCount, StringHandler& handler) {
  for (int i = 0; i < attributeCount; i++) {
    char const* key = handler.GetNext();
    char const* value = handler.GetNext();
    if (key == nullptr || value == nullptr)
      return;


    if (strcmp(key, "id") == 0) {
      element->id = value;
    } else if (strcmp(key, "stick") == 0) {
      element->stick = ToBool(value);
    } else if (strcmp(key, "stretch") == 0) {
      element->stretch = ToBool(value);
    } else if (strcmp(key, "stuck") == 0) {
      element->stuck = ToBool(value);
    } else if (strcmp(key, "centerTextX") == 0) {
      element->centerTextX = ToBool(value);
    } else if (strcmp(key, "centerTextY") == 0) {
      element->centerTextY = ToBool(value);
    } else if (strcmp(key, "enabled") == 0) {
      element->enabled = ToBool(value);
    } else if (strcmp(key, "textColor") == 0) {
      element->textColor = ToVector4(value);
    } else if (strcmp(key, "padding") == 0) {
      element->padding = ToVector2(value);
    } else if (strcmp(key, "textSize") == 0) {
      element->textSize = atof(value);
    } else if (strcmp(key, "onClick") == 0) {
      element->onClick = ToFunction(pageName, value);
    } else if (strcmp(key, "onRelease") == 0) {
      element->onRelease = ToFunction(pageName, value);
    } else if (strcmp(key, "onEnter") == 0) {
      element->onEnter = ToFunction(pageName, value);
    } else if (strcmp(key, "onLeave") == 0) {
      element->onLeave = ToFunction(pageName, value);
    }
  }
}



std::string TryGetStringFrom(char const* const str) {
  if (str == nullptr)
    return std::string();
  return std::string(str);
}



bool PrecompileData(std::string fileName, void** ppBuffer, uint32_t* bufferSize) {
  if (!FileExists(fileName.c_str()) || ppBuffer == nullptr || bufferSize == nullptr) {
    return false;
  }

  // I would like to use SEH, but it doesn't allow stuff that need unwinding to be created
  // Could fix by creating them all beforehand, including precompiler by starting in suspended state
  // Not a huge issue though since SEH would only be needed if file mapping is with real files/networks/stuff that can fail
  // This, should, be in-memory through Windows so basic try-catch should work if any odd things occur
  try {

    // Use system page file
    HANDLE mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 32768, "Local\\guiCreatorFileMapping");
    if (mapping == nullptr) {
      std::cerr << "Failed to create file mapping";
      return false;
    }


    void* view = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (view == nullptr) {
      std::cerr << "Failed to create view of mapping";
      CloseHandle(mapping);
      return false;
    }

    *(uint32_t*)view = 0;



    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    if (CreateProcessA(
      "GUIPrecompiler.exe",
      ("GUIPrecompiler " + fileName).data(),
      nullptr,
      nullptr,
      true,
      0,
      nullptr,
      nullptr,
      &si,
      &pi
      )) {


      WaitForSingleObject(pi.hProcess, INFINITE);

      // Process runs on thread, so close it first
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);


      // First item is 4-byte length of data
      *bufferSize = *(uint32_t*)view;

      if (*bufferSize == 0) {
        // Close
        UnmapViewOfFile(view);
        CloseHandle(mapping);
        return false;
      }


      *ppBuffer = malloc(*bufferSize);
      memcpy(*ppBuffer, ((uint8_t*)view + 4), *bufferSize);


      // Close
      UnmapViewOfFile(view);
      CloseHandle(mapping);
      return true;
    }


    // Close
    UnmapViewOfFile(view);
    CloseHandle(mapping);
    return false;


    // Get any exception
  } catch (const std::exception& ex) {
    std::cerr << "PrecompileData --- " << ex.what() << '\n';
  }


  return false;
}







// Rendering


void DrawBodyObject(GUIBody* base, float aspect, bool widthWay) {
  Vector2 sizeFix = Vector2(1);

  if (widthWay) {
    sizeFix.x /= aspect;
  } else {
    sizeFix.y *= aspect;
  }


  ShaderHandler::SetVector2("position", 0, 0);
  ShaderHandler::SetVector2("scale", 1, 1);
  ShaderHandler::SetVector2("sizeFix", sizeFix);

  GUIShaderInformation info = base->ShaderInformation();

  glBufferData(GL_UNIFORM_BUFFER, sizeof(GUIShaderInformation), nullptr, GL_STREAM_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GUIShaderInformation), &info);

  if (base->texture != 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, base->texture);
    ShaderHandler::SetInt("texTarget", 0);
    ShaderHandler::SetBool("useTexture", true);
  } else {
    ShaderHandler::SetBool("useTexture", false);
  }

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}



void DrawObject(GUIElement* obj, float aspect, IVector2 windowSize, bool widthWay) {
  if (!obj->enabled) {
    return;
  }

  obj->drawTransform += obj->transform;

  float fixedPosX = obj->drawTransform.position.x;
  float scaleX = obj->drawTransform.scale.x;

  float fixedPosY = obj->drawTransform.position.y;
  float scaleY = obj->drawTransform.scale.y;

  float fontWindow = bitmap.GetFontSize() * 20.f;
  Vector2 sizeFix = Vector2(1, 1);


  if (widthWay) {
    if (obj->stick && obj->drawTransform.position.x != 0) {
      float one = SignOf(obj->drawTransform.position.x);

      fixedPosX = (one - scaleX / aspect) - ((one - (fixedPosX + scaleX)) / aspect);
    } else if (obj->stuck) {
      fixedPosX /= aspect;
    }

    sizeFix.x /= aspect;

    if (!obj->stretch) {
      scaleX /= aspect;
    }
  } else {
    if (obj->stick && obj->drawTransform.position.y != 0) {
      float one = SignOf(obj->drawTransform.position.y);

      fixedPosY = (one - scaleY * aspect) - ((one - (fixedPosY + scaleY)) * aspect);

    } else if (obj->stuck) {
      fixedPosY *= aspect;
    }

    sizeFix.y = aspect;


    if (!obj->stretch) {
      scaleY *= aspect;
    }
  }



  ShaderHandler::SetVector2("position", fixedPosX, fixedPosY);
  ShaderHandler::SetVector2("scale", scaleX, scaleY);
  ShaderHandler::SetVector2("sizeFix", sizeFix * 0.5f);

  GUIShaderInformation info = obj->ShaderInformation();

  glBufferData(GL_UNIFORM_BUFFER, sizeof(GUIShaderInformation), nullptr, GL_STREAM_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GUIShaderInformation), &info);

  if (obj->texture != 0) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->texture);
    ShaderHandler::SetInt("texTarget", 0);
    ShaderHandler::SetBool("useTexture", true);
  } else {
    ShaderHandler::SetBool("useTexture", false);
  }

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



  if (obj->data.size() != 0) {
    text_rendering::RenderTextInfo info = text_rendering::RenderTextInfo(windowSize);
    info.color = obj->textColor;
    info.startPos = {fixedPosX - (scaleX * (1 - obj->padding.x)), fixedPosY + (obj->centerTextY ? 0.f : scaleY) - (scaleY * obj->padding.y)};
    info.fontScale = (obj->textSize * (float)(widthWay ? windowSize.y : windowSize.x)) / fontWindow;
    info.centerX = obj->centerTextX;
    info.centerY = obj->centerTextY;
    info.textWidth = 2 * scaleX * (1 - obj->padding.x);


    text_rendering::Render(bitmap, obj->data, info);
  }



  for (GUIElement* child : obj->children) {
    child->drawTransform = Transform();
    child->drawTransform += obj->drawTransform;
    DrawObject(child, aspect, windowSize, widthWay);
  }

  obj->drawTransform.position = {fixedPosX, fixedPosY};
  obj->drawTransform.scale = {scaleX, scaleY};
}



} // namespace