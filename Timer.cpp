#include "Timer.h"

Timer::Timer() {
    
}

void Timer::resetTimer() {

    m_begin = std::chrono::steady_clock::now();
    m_min = {};
    m_sec = {};
    t_pause = {};
}

std::string Timer::getElapsedTime() {

    std::chrono::steady_clock::time_point t_now = std::chrono::steady_clock::now();

    // Get seconds elapsed from the beginning
    int m_sec = std::chrono::duration_cast<std::chrono::seconds>(t_now - m_begin).count();

    /* I choose to reset at every minute, so i can reset seconds variable and increment minute variable;
    maybe there is a better solution */
    if (m_sec == 60) {
     
        m_begin = t_now;
        m_min++;
        m_sec = 0;
    }

    // Graphical representation mm::ss
    std::string m_minS = (m_min < 10 ? "0" + std::to_string(m_min) : std::to_string(m_min));
    std::string m_secS = (m_sec < 10 ? "0" + std::to_string(m_sec) : std::to_string(m_sec));

    return m_minS + ':' + m_secS;    
}

void Timer::pauseTimer() {

    t_pause = std::chrono::steady_clock::now();
}

void Timer::restartWDelay() {

    std::chrono::steady_clock::time_point t_now = std::chrono::steady_clock::now();
    const int64_t sec = std::chrono::duration_cast<std::chrono::seconds>(t_now - t_pause).count();

    m_begin += std::chrono::seconds(sec);
}