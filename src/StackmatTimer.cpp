#include <Stream.h>
#include <Arduino.h>
#include <StackmatTimer.h>

Stream *serial;

StackmatTimer::StackmatTimer(Stream *mySerial)
{
    serial = mySerial;
}

bool StackmatTimer::Update()
{
    String data;
    while (serial->available() > 9)
    {
        data = serial->readStringUntil('\r');
    }

    if (data.length() >= 8)
    {
        return this->ParseTimerData(data);
    }

    if (millis() - LastUpdateMillis >= STACKMATTIMER_TIMEOUT)
    {
        Connected = false;
    }

    return false;
}

bool StackmatTimer::IsConnected() { return Connected; }

uint64_t StackmatTimer::GetLastUpdateMillis() { return LastUpdateMillis; }

StackmatTimerState StackmatTimer::GetState() { return State; }

uint32_t StackmatTimer::GetTime() { return TotalTime; }

uint32_t StackmatTimer::GetInterpolatedTime()
{
    // Only interpolate if currently running!
    if (State != ST_Running) {
        return TotalTime;
    }

    return TotalTime + (millis() - LastUpdateMillis);
}


uint8_t StackmatTimer::GetDisplayMinutes()
{
    return TotalTime / 60000;
}

uint8_t StackmatTimer::GetInterpolatedDisplayMinutes()
{
    return GetInterpolatedTime() / 60000;
}

uint8_t StackmatTimer::GetDisplaySeconds()
{
    return (TotalTime - ((TotalTime / 60000) * 60000)) / 1000;
}

uint8_t StackmatTimer::GetInterpolatedDisplaySeconds()
{
    uint32_t interpolatedTime = GetInterpolatedTime();
    return (interpolatedTime - ((interpolatedTime / 60000) * 60000)) / 1000;
}

uint16_t StackmatTimer::GetDisplayMilliseconds()
{
    uint32_t time = TotalTime;
    time -= ((time / 60000) * 60000);
    time -= ((time / 1000) * 1000);
    return time;
}

uint16_t StackmatTimer::GetInterpolatedDisplayMilliseconds()
{
    uint32_t interpolatedTime = GetInterpolatedTime();
    interpolatedTime -= ((interpolatedTime / 60000) * 60000);
    interpolatedTime -= ((interpolatedTime / 1000) * 1000);
    return interpolatedTime;
}

bool StackmatTimer::ParseTimerData(String data)
{
    uint64_t lastUpdated = millis();
    StackmatTimerState newState = static_cast<StackmatTimerState>(data[0]);
    uint32_t minutes = data.substring(1, 2).toInt();
    uint32_t seconds = data.substring(2, 4).toInt();
    uint32_t millis = data.substring(4, 7).toInt();
    uint32_t checksum = (int)data[7];
    uint32_t totalMs = (((minutes * 60) + seconds) * 1000) + millis;

    // Checksum is the sum of all digits + 64
    uint32_t sum = 64;
    for (int i = 1; i < 7; i++)
    {
        sum += data.substring(i, i + 1).toInt();
    }

    if (sum != checksum)
    {
        return false;
    }

    #ifndef STACKMATTIMER_DISABLE_NORMALIZE_RESET
    // Some timers does not support Stopped as a state,
    // and will simply report Stopped as Reset, but with a non-zero time.
    // Here we simply normalize this so it is always Stopped
    if (totalMs > 0 && newState == ST_Reset)
    {
        newState = ST_Stopped;
    }
    #endif

    // Update data
    Connected = true;
    State = newState;
    TotalTime = totalMs;
    LastUpdateMillis = lastUpdated;

    return true;
}

StackmatTimer::~StackmatTimer()
{
}
