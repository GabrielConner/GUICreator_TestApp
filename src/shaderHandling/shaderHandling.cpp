#include "pPack/shaderHandling.h"

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>

#include "pPack/vector.h"
#include "glad/glad.h"


using namespace ::pPack;
using namespace ::pPack::shader_handling;


unsigned int ShaderHandler::currentShader = 0;

namespace {
std::map<std::string, unsigned int> globalShaders;
}


namespace pPack {
namespace shader_handling {

bool ShaderHandler::FileExists(std::string file) {
  struct stat buffer;
  if (stat(file.c_str(), &buffer))
    return false;
  return true;
}

void ShaderHandler::Active() {
  glUseProgram(internal);
  currentShader = internal;
}


void ShaderHandler::SetCurrentShader(unsigned int s) {
  glUseProgram(s);
  currentShader = s;
}

unsigned int ShaderHandler::GetShader(std::string loc) {
  if (!globalShaders.contains(loc)) {
    return 0u;
  }

  return globalShaders[loc];
}


unsigned int ShaderHandler::CreateShader(std::string name, ShaderCreateInfo* shaderInfo, size_t shaderCount) {
  if (name != "" && globalShaders.contains(name))
    return globalShaders[name];

  if (!shaderCount || !shaderInfo)
    return 0;
  for (size_t i = 0; i < shaderCount; i++)
    if (!shaderInfo[i].shaderCount || !shaderInfo[i].shaders)
      return 0;
    else for (size_t j = 0; j < shaderInfo[i].shaderCount; j++)
      if (!shaderInfo[i].shaders[j] || !FileExists(shaderInfo[i].shaders[j]))
        return 0;


  unsigned int shaderProgram = glCreateProgram();
  unsigned int* shaders = new unsigned int[shaderCount];
  for (size_t i = 0; i < shaderCount; i++)
    shaders[i] = 0;

  char err[255];
  int success = 0;

  for (size_t shaderIndex = 0; shaderIndex < shaderCount; shaderIndex++) {
    char** shaderStr = new char*[shaderInfo[shaderIndex].shaderCount];

    for (size_t shaderPieceIndex = 0; shaderPieceIndex < shaderInfo[shaderIndex].shaderCount; shaderPieceIndex++) {
      std::ifstream file(shaderInfo[shaderIndex].shaders[shaderPieceIndex], std::ios::binary | std::ios::in | std::ios::ate);

      size_t fileSize = 0;
      fileSize = file.tellg();
      file.seekg(std::ios::beg);


      shaderStr[shaderPieceIndex] = new char[fileSize + 1];

      std::stringstream sstream;
      std::string str;
      sstream << file.rdbuf();
      str = sstream.str();
      std::strcpy(shaderStr[shaderPieceIndex], str.data());

      file.close();
    }

    shaders[shaderIndex] = glCreateShader(shaderInfo[shaderIndex].type);
    glShaderSource(shaders[shaderIndex], shaderInfo[shaderIndex].shaderCount, shaderStr, 0);
    glCompileShader(shaders[shaderIndex]);
    glGetShaderiv(shaders[shaderIndex], GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shaders[shaderIndex], 255, 0, err);
      std::cout << err << '\n';
    }

    glAttachShader(shaderProgram, shaders[shaderIndex]);

    for (size_t j = 0; j < shaderInfo[shaderIndex].shaderCount; j++)
      delete[](shaderStr[j]);
    delete[](shaderStr);
  }

  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 255, 0, err);
    std::cout << err << '\n';
  }

  for (size_t i = 0; i < shaderCount; i++)
    glDeleteShader(shaders[i]);
  delete[](shaders);

  if (name != "")
    globalShaders.insert({name, shaderProgram});
  return shaderProgram;
}


void ShaderHandler::SetBool(std::string name, bool value) {
  glUniform1i(glGetUniformLocation(currentShader, name.c_str()), value);
}
void ShaderHandler::SetUInt(std::string name, unsigned int value) {
  glUniform1ui(glGetUniformLocation(currentShader, name.c_str()), value);
}void ShaderHandler::SetInt(std::string name, int value) {
  glUniform1i(glGetUniformLocation(currentShader, name.c_str()), value);
}
void ShaderHandler::SetFloat(std::string name, float value) {
  glUniform1f(glGetUniformLocation(currentShader, name.c_str()), value);
}
void ShaderHandler::SetVector2(std::string name, float x, float y) {
  glUniform2f(glGetUniformLocation(currentShader, name.c_str()), x, y);
}
void ShaderHandler::SetVector2(std::string name, vector::Vector2 vec) {
  glUniform2f(glGetUniformLocation(currentShader, name.c_str()), vec.x, vec.y);
}
void ShaderHandler::SetVector3(std::string name, float x, float y, float z) {
  glUniform3f(glGetUniformLocation(currentShader, name.c_str()), x, y, z);
}
void ShaderHandler::SetVector3(std::string name, vector::Vector3 vec) {
  glUniform3f(glGetUniformLocation(currentShader, name.c_str()), vec.x, vec.y, vec.z);
}
void ShaderHandler::SetVector4(std::string name, float x, float y, float z, float w) {
  glUniform4f(glGetUniformLocation(currentShader, name.c_str()), x, y, z, w);
}
void ShaderHandler::SetVector4(std::string name, vector::Vector4 vec) {
  glUniform4f(glGetUniformLocation(currentShader, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}
void ShaderHandler::SetMat4(std::string name, const float* Mat) {
  glUniformMatrix4fv(glGetUniformLocation(currentShader, name.c_str()), 1, false, Mat);
}


void ShaderHandler::SetBool(const char* name, bool value) {
  glUniform1i(glGetUniformLocation(currentShader, name), value);
}
void ShaderHandler::SetUInt(const char* name, unsigned int value) {
  glUniform1ui(glGetUniformLocation(currentShader, name), value);
}
void ShaderHandler::SetInt(const char* name, int value) {
  glUniform1i(glGetUniformLocation(currentShader, name), value);
}
void ShaderHandler::SetFloat(const char* name, float value) {
  glUniform1f(glGetUniformLocation(currentShader, name), value);
}
void ShaderHandler::SetVector2(const char* name, float x, float y) {
  glUniform2f(glGetUniformLocation(currentShader, name), x, y);
}
void ShaderHandler::SetVector2(const char* name, vector::Vector2 vec) {
  glUniform2f(glGetUniformLocation(currentShader, name), vec.x, vec.y);
}
void ShaderHandler::SetVector3(const char* name, float x, float y, float z) {
  glUniform3f(glGetUniformLocation(currentShader, name), x, y, z);
}
void ShaderHandler::SetVector3(const char* name, vector::Vector3 vec) {
  glUniform3f(glGetUniformLocation(currentShader, name), vec.x, vec.y, vec.z);
}
void ShaderHandler::SetVector4(const char* name, float x, float y, float z, float w) {
  glUniform4f(glGetUniformLocation(currentShader, name), x, y, z, w);
}
void ShaderHandler::SetVector4(const char* name, vector::Vector4 vec) {
  glUniform4f(glGetUniformLocation(currentShader, name), vec.x, vec.y, vec.z, vec.w);
}
void ShaderHandler::SetMat4(const char* name, const float* Mat) {
  glUniformMatrix4fv(glGetUniformLocation(currentShader, name), 1, false, Mat);
}


} //namespace shader_handling
} //namespace pPack