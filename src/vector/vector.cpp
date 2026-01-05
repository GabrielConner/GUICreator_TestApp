#include "pPack/vector.h"
using namespace ::pPack;
using namespace ::pPack::vector;

std::ostream& operator << (std::ostream& str, const Vector2& vec) {
  str << vec.x << ", " << vec.y;
  return str;
}

std::ostream& operator << (std::ostream& str, const Vector3& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z;
  return str;
}

std::ostream& operator << (std::ostream& str, const Vector4& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
  return str;
}





std::ostream& operator << (std::ostream& str, const DVector2& vec) {
  str << vec.x << ", " << vec.y;
  return str;
}

std::ostream& operator << (std::ostream& str, const DVector3& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z;
  return str;
}

std::ostream& operator << (std::ostream& str, const DVector4& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
  return str;
}





std::ostream& operator << (std::ostream& str, const I8Vector2& vec) {
  str << (int)vec.x << ", " << (int)vec.y;
  return str;
}

std::ostream& operator << (std::ostream& str, const I16Vector2& vec) {
  str << vec.x << ", " << vec.y;
  return str;
}

std::ostream& operator << (std::ostream& str, const I32Vector2& vec) {
  str << vec.x << ", " << vec.y;
  return str;
}

std::ostream& operator << (std::ostream& str, const I64Vector2& vec) {
  str << vec.x << ", " << vec.y;
  return str;
}



std::ostream& operator << (std::ostream& str, const I8Vector3& vec) {
  str << (int)vec.x << ", " << (int)vec.y << ", " << (int)vec.z;
  return str;
}

std::ostream& operator << (std::ostream& str, const I16Vector3& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z;
  return str;
}

std::ostream& operator << (std::ostream& str, const I32Vector3& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z;
  return str;
}

std::ostream& operator << (std::ostream& str, const I64Vector3& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z;
  return str;
}



std::ostream& operator << (std::ostream& str, const I8Vector4& vec) {
  str << (int)vec.x << ", " << (int)vec.y << ", " << (int)vec.z << ", " << (int)vec.w;
  return str;
}

std::ostream& operator << (std::ostream& str, const I16Vector4& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
  return str;
}

std::ostream& operator << (std::ostream& str, const I32Vector4& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
  return str;
}

std::ostream& operator << (std::ostream& str, const I64Vector4& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
  return str;
}





std::ostream& operator << (std::ostream& str, const UI8Vector2& vec) {
  str << (unsigned int)vec.x << ", " << (unsigned int)vec.y;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI16Vector2& vec) {
  str << vec.x << ", " << vec.y;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI32Vector2& vec) {
  str << vec.x << ", " << vec.y;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI64Vector2& vec) {
  str << vec.x << ", " << vec.y;
  return str;
}



std::ostream& operator << (std::ostream& str, const UI8Vector3& vec) {
  str << (unsigned int)vec.x << ", " << (unsigned int)vec.y << ", " << (unsigned int)vec.z;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI16Vector3& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI32Vector3& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI64Vector3& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z;
  return str;
}



std::ostream& operator << (std::ostream& str, const UI8Vector4& vec) {
  str << (unsigned int)vec.x << ", " << (unsigned int)vec.y << ", " << (unsigned int)vec.z << ", " << (unsigned int)vec.w;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI16Vector4& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI32Vector4& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
  return str;
}

std::ostream& operator << (std::ostream& str, const UI64Vector4& vec) {
  str << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
  return str;
}