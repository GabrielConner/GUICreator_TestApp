#ifndef PPM_INCLUDE_PPACK_ANIMATE_VALUE_H
#define PPM_INCLUDE_PPACK_ANIMATE_VALUE_H

constexpr unsigned char ANIMATE_VALUE_TYPE_LONG_DOUBLE = 1;
constexpr unsigned char ANIMATE_VALUE_TYPE_DOUBLE = 2;
constexpr unsigned char ANIMATE_VALUE_TYPE_FLOAT = 3;
constexpr unsigned char ANIMATE_VALUE_TYPE_LONG_LONG = 4;
constexpr unsigned char ANIMATE_VALUE_TYPE_LONG = 5;
constexpr unsigned char ANIMATE_VALUE_TYPE_INT = 6;
constexpr unsigned char ANIMATE_VALUE_TYPE_SHORT = 7;
constexpr unsigned char ANIMATE_VALUE_TYPE_CHAR = 8;
constexpr unsigned char ANIMATE_VALUE_TYPE_UNSIGNED_LONG_LONG = 9;
constexpr unsigned char ANIMATE_VALUE_TYPE_UNSIGNED_LONG = 10;
constexpr unsigned char ANIMATE_VALUE_TYPE_UNSIGNED_INT = 11;
constexpr unsigned char ANIMATE_VALUE_TYPE_UNSIGNED_SHORT = 12;
constexpr unsigned char ANIMATE_VALUE_TYPE_UNSIGNED_CHAR = 13;

constexpr unsigned char ANIMATE_VALUE_MODIFIER_LOOP = 0B00000001;
constexpr unsigned char ANIMATE_VALUE_MODIFIER_BOUNCE = 0B00000010;

namespace pPack {
namespace animate_value {







union AnimatedPtrValueType {
  long double* valueLongDouble;
  double* valueDouble;
  float* valueFloat;
  long long* valueLongLong;
  long* valueLong;
  int* valueInt;
  short* valueShort;
  char* valueChar;
  unsigned long long* valueULongLong;
  unsigned long* valueULong;
  unsigned int* valueUInt;
  unsigned short* valueUShort;
  unsigned char* valueUChar;


  AnimatedPtrValueType(long double* v) : valueLongDouble(v) {}
  AnimatedPtrValueType(double* v) : valueDouble(v) {}
  AnimatedPtrValueType(float* v) : valueFloat(v) {}
  AnimatedPtrValueType(long long* v) : valueLongLong(v) {}
  AnimatedPtrValueType(long* v) : valueLong(v) {}
  AnimatedPtrValueType(int* v) : valueInt(v) {}
  AnimatedPtrValueType(short* v) : valueShort(v) {}
  AnimatedPtrValueType(char* v) : valueChar(v) {}
  AnimatedPtrValueType(unsigned long long* v) : valueULongLong(v) {}
  AnimatedPtrValueType(unsigned long* v) : valueULong(v) {}
  AnimatedPtrValueType(unsigned int* v) : valueUInt(v) {}
  AnimatedPtrValueType(unsigned short* v) : valueUShort(v) {}
  AnimatedPtrValueType(unsigned char* v) : valueUChar(v) {}
};






struct AnimatedValue {
  AnimatedPtrValueType value;
  long double cur;
  long double from;
  long double to;



  long double curTime;
  long double length;

  unsigned char modifiers;

  bool paused;
  bool shouldDelete;

  int valueType;


  void SetToTime();


  AnimatedValue() = delete;
  AnimatedValue(AnimatedPtrValueType Value, long double From, long double To, long double Length, unsigned char Modifiers, int ValueType);
};



void AnimateValueUpdate();

size_t AddAnimation(AnimatedValue animation);

AnimatedValue* GetAnimation(size_t anim);

void DeleteAnimation(size_t anim);

void DeleteAllAnimations();


} //namespace animate_value
} //namespace pPack

#endif