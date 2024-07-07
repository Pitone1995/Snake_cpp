#ifndef Timer_H
#define Timer_H

#include <chrono>
#include <string>

class Timer {

public:

    Timer();
    
    void resetTimer();
    std::string getElapsedTime();
    void restartWDelay();
    void pauseTimer();

private:

    int m_min{};
    int m_sec{};
    std::chrono::steady_clock::time_point m_begin{};
    std::chrono::steady_clock::time_point t_pause{};
};

#endif