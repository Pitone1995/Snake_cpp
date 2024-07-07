#include "Timer.h"

Timer::Timer() {

}

void Timer::resetTimer() {

    m_begin = std::chrono::steady_clock::now();
    t_pause = {};
}

std::string Timer::getElapsedTime() {

    std::chrono::steady_clock::time_point t_now = std::chrono::steady_clock::now();

    // Get seconds elapsed from the beginning
    int min = std::chrono::duration_cast<std::chrono::minutes>(t_now - m_begin).count();
    int sec = std::chrono::duration_cast<std::chrono::seconds>(t_now - m_begin).count();

    // Stay in the range 0 - 60 s
    sec -= (60 * min);
    
    // Graphical representation mm::ss
    std::string minS = (min < 10 ? "0" + std::to_string(min) : std::to_string(min));
    std::string secS = (sec < 10 ? "0" + std::to_string(sec) : std::to_string(sec));

    return minS + ':' + secS;    
}

void Timer::pauseTimer() {

    t_pause = std::chrono::steady_clock::now();
}

void Timer::restartWDelay() {

    std::chrono::steady_clock::time_point t_now = std::chrono::steady_clock::now();
    const int64_t sec = std::chrono::duration_cast<std::chrono::seconds>(t_now - t_pause).count();

    m_begin += std::chrono::seconds(sec);
}