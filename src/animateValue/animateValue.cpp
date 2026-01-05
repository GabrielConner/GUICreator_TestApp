#include "pPack/animateValue.h"
#include "pPack/timer.h"

#include <vector>
#include <map>
#include <chrono>
#include <iostream>

using namespace ::pPack;
using namespace ::pPack::timer;
using namespace ::pPack::animate_value;


namespace {

std::map<size_t, AnimatedValue*> animations = std::map<size_t, AnimatedValue*>();

size_t animationIterator = 0;

} // namespace




namespace pPack {
namespace animate_value {


void AnimatedValue::SetToTime() {


  cur = from + (((to - from) / length) * curTime);

  switch (valueType) {
    case ANIMATE_VALUE_TYPE_LONG_DOUBLE:
      *value.valueLongDouble = cur;
      break;

    case ANIMATE_VALUE_TYPE_DOUBLE:
      *value.valueDouble = static_cast<double>(cur);
      break;

    case ANIMATE_VALUE_TYPE_FLOAT:
      *value.valueFloat = static_cast<float>(cur);
      break;

    case ANIMATE_VALUE_TYPE_LONG_LONG:
      *value.valueLongLong = static_cast<long long>(cur);
      break;

    case ANIMATE_VALUE_TYPE_LONG:
      *value.valueLong = static_cast<long>(cur);
      break;

    case ANIMATE_VALUE_TYPE_INT:
      *value.valueInt = static_cast<int>(cur);
      break;

    case ANIMATE_VALUE_TYPE_SHORT:
      *value.valueShort = static_cast<short>(cur);
      break;

    case ANIMATE_VALUE_TYPE_CHAR:
      *value.valueChar = static_cast<char>(cur);
      break;

    case ANIMATE_VALUE_TYPE_UNSIGNED_LONG_LONG:
      *value.valueULongLong = static_cast<unsigned long long>(cur);
      break;

    case ANIMATE_VALUE_TYPE_UNSIGNED_LONG:
      *value.valueULong = static_cast<unsigned long>(cur);
      break;

    case ANIMATE_VALUE_TYPE_UNSIGNED_INT:
      *value.valueUInt = static_cast<unsigned int>(cur);
      break;

    case ANIMATE_VALUE_TYPE_UNSIGNED_SHORT:
      *value.valueUShort = static_cast<unsigned short>(cur);
      break;

    case ANIMATE_VALUE_TYPE_UNSIGNED_CHAR:
      *value.valueUChar = static_cast<unsigned char>(cur);
      break;
  }
}





void AnimateValueUpdate() {

  Timer& timer = Timer::GetSingleton();
  double deltaTime = timer.GetDeltaTime();


  std::vector<size_t> removeValues = std::vector<size_t>();

  for (auto& [i, val] : animations) {
    if (val->paused) {
      continue;
    }


    if (val->curTime >= val->length) {
      val->curTime = val->length;
      val->SetToTime();

      if (val->modifiers & ANIMATE_VALUE_MODIFIER_LOOP) {
        val->curTime = 0;
      } else if (val->modifiers & ANIMATE_VALUE_MODIFIER_BOUNCE) {
        val->curTime = 0;
        long double temp = val->from;
        val->from = val->to;
        val->to = temp;
      } else {
        removeValues.push_back(i);
        continue;
      }
    }

    val->SetToTime();

    val->curTime += deltaTime;
  }


  for (size_t i : removeValues) {
    delete(animations[i]);
    animations.erase(i);
  }
}




size_t AddAnimation(AnimatedValue animation) {
  AnimatedValue* newAnim = new AnimatedValue(animation);

  animations.insert(std::pair<size_t, AnimatedValue*>(animationIterator, newAnim));

  size_t temp = animationIterator;
  animationIterator++;

  return temp;
}



AnimatedValue* GetAnimation(size_t anim) {
  if (animations.contains(anim)) {
    return animations[anim];
  }

  return nullptr;
}



void DeleteAnimation(size_t anim) {
  if (animations.contains(anim)) {
    delete(animations[anim]);
    animations.erase(anim);
  }
}


void DeleteAllAnimations() {
  for (auto& [i, anim] : animations) {
    if (anim != nullptr)
      delete(anim);
  }

  animations.clear();
}




AnimatedValue::AnimatedValue(AnimatedPtrValueType Value, long double From, long double To, long double Length, unsigned char Modifiers, int ValueType) :
  value(Value), cur(From), from(From), to(To), curTime(0), length(Length), modifiers(Modifiers), paused(false), shouldDelete(false), valueType(ValueType) {}


} //namespace animate_value
} //namespace pPack
