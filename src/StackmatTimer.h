#ifndef STACKMATTIMER_H
#define STACKMATTIMER_H

#define STACKMATTIMER_TIMEOUT 1500

#include <Arduino.h>

static const long STACKMAT_TIMER_BAUD_RATE = 1200;

enum StackmatTimerState {
    ST_Reset = 'I', // Timer was reset
    ST_Running = ' ', // Timer is currently running
    ST_LeftHandOnTimer = 'L', // Left hand is on timer (Not supported by all timers!)
    ST_RightHandOnTimer = 'R', // Right hand is on timer (Not supported by all timers!)
    ST_BothHandsOnTimer = 'C', // Both hand is on timer, preparing to start (Not supported by all timers!)
    ST_ReadyToStart = 'A', // Ready to start (Not supported by all timers!)
    ST_Stopped = 'S' // The timer has been stopped
};

class StackmatTimer
{
private:
  bool ParseTimerData(String data);
  StackmatTimerState State;
  uint32_t TotalTime;
  uint64_t LastUpdateMillis;
  bool Connected;

public:
  StackmatTimer(Stream *serial);
  ~StackmatTimer();
  // Fetch latest data from timer
  bool Update();

  // Check if Timer is connected
  bool IsConnected();

  // Get current timer state
  StackmatTimerState GetState();

  // Get last reported time from timer (in ms)
  uint32_t GetTime();

  // Last update timestamp
  uint64_t GetLastUpdateMillis();

  // Get interpolated time from timer (in ms). This takes the last reported time from timer 
  // and adds the milliseconds since it was reported.
  // Will probably not be as accurate as GetTime() but is very good for printing
  // current time to a display that refreshes faster than data is sent from timer,
  // making for a much smoother display.
  // NOTE: If timer is not currently running, then this will simply return
  // the time as last reported by the timer.
  uint32_t GetInterpolatedTime();

  // Get minutes on the display (For instance, 1:02:555 will return 1)
  uint8_t GetDisplayMinutes();
  // Get seconds on the display (For instance, 1:02.555 will return 2)
  uint8_t GetDisplaySeconds();
  // Get milliseconds on the display (For instance, 1:02.555 will return 555)
  uint16_t GetDisplayMilliseconds();

  // Get minutes on the display (For instance, 1:02:555 will return 1), but interpolated (see GetInterpolatedTime() for more)
  uint8_t GetInterpolatedDisplayMinutes();
  // Get seconds on the display (For instance, 1:02.555 will return 2), but interpolated (see GetInterpolatedTime() for more)
  uint8_t GetInterpolatedDisplaySeconds();
  // Get milliseconds on the display (For instance, 1:02.555 will return 555), but interpolated (see GetInterpolatedTime() for more)
  uint16_t GetInterpolatedDisplayMilliseconds();
};

#endif