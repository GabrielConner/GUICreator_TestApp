#ifndef PPM_INCLUDE_PPACK_TIMER_H
#define PPM_INCLUDE_PPACK_TIMER_H

#include <chrono>


namespace pPack {
namespace timer {

class Timer {
  double totalTime;
  double deltaTime;
  double timeScale;
  size_t updateDelay;

  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> startPoint;


public:

  static Timer& GetSingleton();



  void Advance();
  void Advance(double delta);

  double GetDeltaTime() const;
  double GetTimeScale() const;
  size_t GetUpdateDelay() const;
  double GetTotalTime() const;

  void SetTimeScale(double Scale);
  void SetUpdateDelay(double Rate);

  void WaitForNextUpdate();
  void ResetDeltaTime();


  Timer();

};

}; // namespace timing
}; // namespace pPack


#endif