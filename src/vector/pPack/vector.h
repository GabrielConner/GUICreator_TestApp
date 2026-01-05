#ifndef PPM_INCLUDE_PPACK_VECTOR_H
#define PPM_INCLUDE_PPACK_VECTOR_H

#include <iostream>
#include <cstdint>


namespace pPack {
namespace vector {
//Vector2
//--------------------------------------------------
template <class T = float>
struct Vector2_s {
  T x;
  T y;

  Vector2_s<T> operator +(const Vector2_s<T>& t) const {
    Vector2_s<T> temp = *this;
    temp.x += t.x;
    temp.y += t.y;
    return temp;
  }
  Vector2_s<T> operator -(const Vector2_s<T>& t) const {
    Vector2_s<T> temp = *this;
    temp.x -= t.x;
    temp.y -= t.y;
    return temp;
  }
  Vector2_s<T> operator *(const Vector2_s<T>& t) const {
    Vector2_s<T> temp = *this;
    temp.x *= t.x;
    temp.y *= t.y;
    return temp;
  }
  Vector2_s<T> operator /(const Vector2_s<T>& t) const {
    Vector2_s<T> temp = *this;
    temp.x /= t.x;
    temp.y /= t.y;
    return temp;
  }

  Vector2_s<T> operator +(const T& t) const {
    Vector2_s<T> temp = *this;
    temp.x += t;
    temp.y += t;
    return temp;
  }
  Vector2_s<T> operator -(const T& t) const {
    Vector2_s<T> temp = *this;
    temp.x -= t;
    temp.y -= t;
    return temp;
  }
  Vector2_s<T> operator *(const T& t) const {
    Vector2_s<T> temp = *this;
    temp.x *= t;
    temp.y *= t;
    return temp;
  }
  Vector2_s<T> operator /(const T& t) const {
    Vector2_s<T> temp = *this;
    temp.x /= t;
    temp.y /= t;
    return temp;
  }



  Vector2_s<T>& operator +=(const Vector2_s<T>& t) {
    x += t.x;
    y += t.y;
    return *this;
  }
  Vector2_s<T>& operator -=(const Vector2_s<T>& t) {
    x -= t.x;
    y -= t.y;
    return *this;
  }
  Vector2_s<T>& operator *=(const Vector2_s<T>& t) {
    x *= t.x;
    y *= t.y;
    return *this;
  }
  Vector2_s<T>& operator /=(const Vector2_s<T>& t) {
    x /= t.x;
    y /= t.y;
    return *this;
  }

  Vector2_s<T>& operator +=(const T& t) {
    x += t;
    y += t;
    return *this;
  }
  Vector2_s<T>& operator -=(const T& t) {
    x -= t;
    y -= t;
    return *this;
  }
  Vector2_s<T>& operator *=(const T& t) {
    x *= t;
    y *= t;
    return *this;
  }
  Vector2_s<T>& operator /=(const T& t) {
    x /= t;
    y /= t;
    return *this;
  }



  bool operator ==(const Vector2_s<T>& v) {
    return x == v.x && y == v.y;
  }
  bool operator !=(const Vector2_s<T>& v) {
    return x != v.x || y != v.y;
  }
  bool operator ==(const T& v) {
    return x == v && y == v;
  }
  bool operator !=(const T& v) {
    return x != v || y != v;
  }



  float Magnitude() {
    return sqrt(x * x + y * y);
  }

  Vector2_s<T> Normal() {
    return *this / Magnitude();
  }

  void Normalize() {
    *this = Normal();
  }

  template<class O>
  Vector2_s<O> ConvertTo() {
    return Vector2_s<O>(O(x), O(y));
  }



  static T Dot(Vector2_s<T> a, Vector2_s<T> b) {
    return a.x * b.x + a.y * b.y;
  }



  Vector2_s() : x(T(0)), y(T(0)) {}
  Vector2_s(T t) : x(t), y(t) {}
  Vector2_s(T X, T Y) : x(X), y(Y) {}
};





//Vector3
//--------------------------------------------------
template <class T = float>
struct Vector3_s {
  T x;
  T y;
  T z;

  Vector3_s<T> operator +(const Vector3_s<T>& t) const {
    Vector3_s<T> temp = *this;
    temp.x += t.x;
    temp.y += t.y;
    temp.z += t.z;
    return temp;
  }
  Vector3_s<T> operator -(const Vector3_s<T>& t) const {
    Vector3_s<T> temp = *this;
    temp.x -= t.x;
    temp.y -= t.y;
    temp.z -= t.z;
    return temp;
  }
  Vector3_s<T> operator *(const Vector3_s<T>& t) const {
    Vector3_s<T> temp = *this;
    temp.x *= t.x;
    temp.y *= t.y;
    temp.z *= t.z;
    return temp;
  }
  Vector3_s<T> operator /(const Vector3_s<T>& t) const {
    Vector3_s<T> temp = *this;
    temp.x /= t.x;
    temp.y /= t.y;
    temp.z /= t.z;
    return temp;
  }

  Vector3_s<T> operator +(const T& t) const {
    Vector3_s<T> temp = *this;
    temp.x += t;
    temp.y += t;
    temp.z += t;
    return temp;
  }
  Vector3_s<T> operator -(const T& t) const {
    Vector3_s<T> temp = *this;
    temp.x -= t;
    temp.y -= t;
    temp.z -= t;
    return temp;
  }
  Vector3_s<T> operator *(const T& t) const {
    Vector3_s<T> temp = *this;
    temp.x *= t;
    temp.y *= t;
    temp.z *= t;
    return temp;
  }
  Vector3_s<T> operator /(const T& t) const {
    Vector3_s<T> temp = *this;
    temp.x /= t;
    temp.y /= t;
    temp.z /= t;
    return temp;
  }



  Vector3_s<T>& operator +=(const Vector3_s<T>& t) {
    x += t.x;
    y += t.y;
    z += t.z;
    return *this;
  }
  Vector3_s<T>& operator -=(const Vector3_s<T>& t) {
    x -= t.x;
    y -= t.y;
    z -= t.z;
    return *this;
  }
  Vector3_s<T>& operator *=(const Vector3_s<T>& t) {
    x *= t.x;
    y *= t.y;
    z *= t.z;
    return *this;
  }
  Vector3_s<T>& operator /=(const Vector3_s<T>& t) {
    x /= t.x;
    y /= t.y;
    z /= t.z;
    return *this;
  }

  Vector3_s<T>& operator +=(const T& t) {
    x += t;
    y += t;
    z += t;
    return *this;
  }
  Vector3_s<T>& operator -=(const T& t) {
    x -= t;
    y -= t;
    z -= t;
    return *this;
  }
  Vector3_s<T>& operator *=(const T& t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
  }
  Vector3_s<T>& operator /=(const T& t) {
    x /= t;
    y /= t;
    z /= t;
    return *this;
  }



  bool operator ==(const Vector3_s<T>& v) {
    return x == v.x && y == v.y && z == v.z;
  }
  bool operator !=(const Vector3_s<T>& v) {
    return x != v.x || y != v.y || z != v.z;
  }
  bool operator ==(const T& v) {
    return x == v && y == v && z == v;
  }
  bool operator !=(const T& v) {
    return x != v || y != v && z == v;
  }



  float Magnitude() {
    return sqrt(x * x + y * y + z * z);
  }

  Vector3_s<T> Normal() {
    return *this / Magnitude();
  }

  void Normalize() {
    *this = Normal();
  }

  template<class O>
  Vector3_s<O> ConvertTo() {
    return Vector3_s<O>(O(x), O(y), O(z));
  }



  static T Dot(Vector3_s<T> a, Vector3_s<T> b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }



  Vector3_s() : x(T(0)), y(T(0)), z(T(0)) {}
  Vector3_s(T t) : x(t), y(t), z(t) {}
  Vector3_s(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
  Vector3_s(Vector2_s<T> vec) : x(vec.x), y(vec.y), z(T(0)) {}
};





//Vector4
//--------------------------------------------------
template <class T = float>
struct Vector4_s {
  T x;
  T y;
  T z;
  T w;

  Vector4_s<T> operator +(const Vector4_s<T>& t) const {
    Vector4_s<T> temp = *this;
    temp.x += t.x;
    temp.y += t.y;
    temp.z += t.z;
    temp.w += t.w;
    return temp;
  }
  Vector4_s<T> operator -(const Vector4_s<T>& t) const {
    Vector4_s<T> temp = *this;
    temp.x -= t.x;
    temp.y -= t.y;
    temp.z -= t.z;
    temp.w -= t.w;
    return temp;
  }
  Vector4_s<T> operator *(const Vector4_s<T>& t) const {
    Vector4_s<T> temp = *this;
    temp.x *= t.x;
    temp.y *= t.y;
    temp.z *= t.z;
    temp.w *= t.w;
    return temp;
  }
  Vector4_s<T> operator /(const Vector4_s<T>& t) const {
    Vector4_s<T> temp = *this;
    temp.x /= t.x;
    temp.y /= t.y;
    temp.z /= t.z;
    temp.w /= t.w;
    return temp;
  }

  Vector4_s<T> operator +(const T& t) const {
    Vector4_s<T> temp = *this;
    temp.x += t;
    temp.y += t;
    temp.z += t;
    temp.w += t;
    return temp;
  }
  Vector4_s<T> operator -(const T& t) const {
    Vector4_s<T> temp = *this;
    temp.x -= t;
    temp.y -= t;
    temp.z -= t;
    temp.w -= t;
    return temp;
  }
  Vector4_s<T> operator *(const T& t) const {
    Vector4_s<T> temp = *this;
    temp.x *= t;
    temp.y *= t;
    temp.z *= t;
    temp.w *= t;
    return temp;
  }
  Vector4_s<T> operator /(const T& t) const {
    Vector4_s<T> temp = *this;
    temp.x /= t;
    temp.y /= t;
    temp.z /= t;
    temp.w /= t;
    return temp;
  }



  Vector4_s<T>& operator +=(const Vector4_s<T>& t) {
    x += t.x;
    y += t.y;
    z += t.z;
    w += t.w;
    return *this;
  }
  Vector4_s<T>& operator -=(const Vector4_s<T>& t) {
    x -= t.x;
    y -= t.y;
    z -= t.z;
    w -= t.w;
    return *this;
  }
  Vector4_s<T>& operator *=(const Vector4_s<T>& t) {
    x *= t.x;
    y *= t.y;
    z *= t.z;
    w *= t.w;
    return *this;
  }
  Vector4_s<T>& operator /=(const Vector4_s<T>& t) {
    x /= t.x;
    y /= t.y;
    z /= t.z;
    w /= t.w;
    return *this;
  }

  Vector4_s<T>& operator +=(const T& t) {
    x += t;
    y += t;
    z += t;
    z += t;
    return *this;
  }
  Vector4_s<T>& operator -=(const T& t) {
    x -= t;
    y -= t;
    z -= t;
    z -= t;
    return *this;
  }
  Vector4_s<T>& operator *=(const T& t) {
    x *= t;
    y *= t;
    z *= t;
    z *= t;
    return *this;
  }
  Vector4_s<T>& operator /=(const T& t) {
    x /= t;
    y /= t;
    z /= t;
    z /= t;
    return *this;
  }



  bool operator ==(const Vector4_s<T>& v) {
    return x == v.x && y == v.y && z == v.z && w == v.w;
  }
  bool operator !=(const Vector4_s<T>& v) {
    return x != v.x || y != v.y || z != v.z || w != v.w;
  }
  bool operator ==(const T& v) {
    return x == v && y == v && z == v && w == v;
  }
  bool operator !=(const T& v) {
    return x != v || y != v && z == v || w != v;
  }



  float Magnitude() {
    return sqrt(x * x + y * y + z * z + w * w);
  }

  Vector4_s<T> Normal() {
    return *this / Magnitude();
  }

  void Normalize() {
    *this = Normal();
  }

  template<class O>
  Vector4_s<O> ConvertTo() {
    return Vector4_s<O>(O(x), O(y), O(z), O(w));
  }



  static T Dot(Vector4_s<T> a, Vector4_s<T> b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }



  Vector4_s() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
  Vector4_s(T t) : x(t), y(t), z(t), w(t) {}
  Vector4_s(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
  Vector4_s(Vector2_s<T> vec) : x(vec.x), y(vec.y), z(T(0)), w(T(0)) {}
  Vector4_s(Vector3_s<T> vec) : x(vec.x), y(vec.y), z(vec.z), w(T(0)) {}
};


typedef struct Vector2_s<float> Vector2;
typedef struct Vector3_s<float> Vector3;
typedef struct Vector4_s<float> Vector4;

typedef struct Vector2_s<double> DVector2;
typedef struct Vector3_s<double> DVector3;
typedef struct Vector4_s<double> DVector4;





typedef struct Vector2_s<int> IVector2;
typedef struct Vector2_s<int8_t> I8Vector2;
typedef struct Vector2_s<int16_t> I16Vector2;
typedef struct Vector2_s<int16_t> I16Vector2;
typedef struct Vector2_s<int32_t> I32Vector2;
typedef struct Vector2_s<int64_t> I64Vector2;


typedef struct Vector3_s<int> IVector3;
typedef struct Vector3_s<int8_t> I8Vector3;
typedef struct Vector3_s<int16_t> I16Vector3;
typedef struct Vector3_s<int32_t> I32Vector3;
typedef struct Vector3_s<int64_t> I64Vector3;


typedef struct Vector4_s<int> IVector4;
typedef struct Vector4_s<int8_t> I8Vector4;
typedef struct Vector4_s<int16_t> I16Vector4;
typedef struct Vector4_s<int32_t> I32Vector4;
typedef struct Vector4_s<int64_t> I64Vector4;





typedef struct Vector2_s<unsigned int> UIVector2;
typedef struct Vector2_s<uint8_t> UI8Vector2;
typedef struct Vector2_s<uint16_t> UI16Vector2;
typedef struct Vector2_s<uint32_t> UI32Vector2;
typedef struct Vector2_s<uint64_t> UI64Vector2;


typedef struct Vector3_s<unsigned int> UIVector3;
typedef struct Vector3_s<uint8_t> UI8Vector3;
typedef struct Vector3_s<uint16_t> UI16Vector3;
typedef struct Vector3_s<uint32_t> UI32Vector3;
typedef struct Vector3_s<uint64_t> UI64Vector3;


typedef struct Vector4_s<unsigned int> UIVector4;
typedef struct Vector4_s<uint8_t> UI8Vector4;
typedef struct Vector4_s<uint16_t> UI16Vector4;
typedef struct Vector4_s<uint32_t> UI32Vector4;
typedef struct Vector4_s<uint64_t> UI64Vector4;


} //namespace vector
} //namespace pPack

std::ostream& operator << (std::ostream& str, const pPack::vector::Vector2& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::Vector3& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::Vector4& vec);

std::ostream& operator << (std::ostream& str, const pPack::vector::DVector2& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::DVector3& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::DVector4& vec);

std::ostream& operator << (std::ostream& str, const pPack::vector::I8Vector2& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I16Vector2& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I32Vector2& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I64Vector2& vec);

std::ostream& operator << (std::ostream& str, const pPack::vector::I8Vector3& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I16Vector3& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I32Vector3& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I64Vector3& vec);

std::ostream& operator << (std::ostream& str, const pPack::vector::I8Vector4& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I16Vector4& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I32Vector4& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::I64Vector4& vec);

std::ostream& operator << (std::ostream& str, const pPack::vector::UI8Vector2& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI16Vector2& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI32Vector2& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI64Vector2& vec);

std::ostream& operator << (std::ostream& str, const pPack::vector::UI8Vector3& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI16Vector3& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI32Vector3& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI64Vector3& vec);

std::ostream& operator << (std::ostream& str, const pPack::vector::UI8Vector4& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI16Vector4& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI32Vector4& vec);
std::ostream& operator << (std::ostream& str, const pPack::vector::UI64Vector4& vec);

#endif