//
// Created by constantin on 08/11/22.
//
#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

class Timer {
public:
    void Start() {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }

    void Stop() {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }

    double ElapsedNanoSeconds() const {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if (m_bRunning) {
            endTime = std::chrono::system_clock::now();
        } else {
            endTime = m_EndTime;
        }

        return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - m_StartTime).count();
    }

    double ElapsedMicroSeconds() const {
        return ElapsedNanoSeconds()/1000.0;
    }

    double ElapsedMilliSeconds() const {
        return ElapsedMicroSeconds()/1000.0;
    }

    double ElapsedSeconds() const {
        return ElapsedMilliSeconds() / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool m_bRunning = false;
};
