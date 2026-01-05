#include "pPack/timer.h"

using namespace ::pPack;
using namespace ::pPack::timer;

namespace {

Timer singleton;

} // namespace


namespace pPack {
namespace timer {

Timer& Timer::GetSingleton() {
  return singleton;
}




void Timer::Advance() {
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> cur = std::chrono::steady_clock::now();

  deltaTime = (double)(cur - startPoint).count() * 0.000000001;
  deltaTime *= timeScale;

  totalTime += deltaTime;

  startPoint = cur;
}

void Timer::Advance(double delta) {
  deltaTime = delta;
  deltaTime *= timeScale;

  totalTime += deltaTime;

  startPoint = std::chrono::steady_clock::now();
}


void Timer::SetTimeScale(double Scale) { timeScale = Scale; }
void Timer::SetUpdateDelay(double Rate) { updateDelay = Rate * 1000000000; }

double Timer::GetDeltaTime() const { return deltaTime; }
double Timer::GetTimeScale() const { return timeScale; }
size_t Timer::GetUpdateDelay() const { return updateDelay; }
double Timer::GetTotalTime() const { return totalTime; }



void Timer::WaitForNextUpdate() {
  while ((std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - startPoint).count() * timeScale) < updateDelay) {}
}

void Timer::ResetDeltaTime() {
  startPoint = std::chrono::steady_clock::now();
  deltaTime = 0;
}




Timer::Timer() : totalTime(0), deltaTime(0), timeScale(1.0), updateDelay(0), startPoint(std::chrono::steady_clock::now()) {}

}; // namespace timing
}; // namespace pPack