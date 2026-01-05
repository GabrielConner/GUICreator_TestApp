#ifndef PPM_INCLUDE_PPACK_SHADER_HANDLING_H
#define PPM_INCLUDE_PPACK_SHADER_HANDLING_H

#include <string>

#include "pPack/vector.h"


namespace pPack {
namespace shader_handling {
struct ShaderCreateInfo {
  const char** shaders;
  unsigned int shaderCount;
  unsigned int type;

  ShaderCreateInfo(const char** s, unsigned int sC, unsigned int t) : shaders(s), shaderCount(sC), type(t) {}
};

class ShaderHandler {
  unsigned int internal;

  static unsigned int currentShader;

public:


  void Active();

  operator int() {
    return internal;
  }

  ShaderHandler() {
    internal = 0;
  }
  ShaderHandler(unsigned int s) {
    internal = s;
  }



  static bool FileExists(std::string file);

  static unsigned int GetCurrentShader() {
    return currentShader;
  }
  static void SetCurrentShader(unsigned int s);

  static unsigned int GetShader(std::string loc);

  static unsigned int CreateShader(std::string name, ShaderCreateInfo* shaderInfo, size_t shaderCount);

  static void SetBool(std::string name, bool value);
  static void SetUInt(std::string name, unsigned int value);
  static void SetInt(std::string name, int value);
  static void SetFloat(std::string name, float value);
  static void SetVector2(std::string name, float x, float y);
  static void SetVector2(std::string name, vector::Vector2 vec);
  static void SetVector3(std::string name, float x, float y, float z);
  static void SetVector3(std::string name, vector::Vector3 vec);
  static void SetVector4(std::string name, float x, float y, float z, float w);
  static void SetVector4(std::string name, vector::Vector4 vec);
  static void SetMat4(std::string name, const float* Mat);


  static void SetBool(const char* name, bool value);
  static void SetUInt(const char* name, unsigned int value);
  static void SetInt(const char* name, int value);
  static void SetFloat(const char* name, float value);
  static void SetVector2(const char* name, float x, float y);
  static void SetVector2(const char* name, vector::Vector2 vec);
  static void SetVector3(const char* name, float x, float y, float z);
  static void SetVector3(const char* name, vector::Vector3 vec);
  static void SetVector4(const char* name, float x, float y, float z, float w);
  static void SetVector4(const char* name, vector::Vector4 vec);
  static void SetMat4(const char* name, const float* Mat);
};
} //namespace shader_handling
} //namespace pPack

#endif