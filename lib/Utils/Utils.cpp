#include "Arduino.h"
#include "Utils.h"


Stopwatch::Stopwatch() {
    startTime = millis();
}

unsigned long Stopwatch::getStartTime() {
    return startTime;
}

unsigned long Stopwatch::time() {
    return millis() - startTime;
}

void Stopwatch::reset() {
    startTime = millis();
}

Timer::Timer(unsigned long duration) : Stopwatch() {
    this->duration = duration;
}

bool Timer::isComplete() {
    return millis() >= startTime + duration;
}

unsigned long Timer::timeRemaining() {
    return startTime + duration - millis();
}

float Timer::progress() {
    return (float) time() / duration;
}

void Timer::setDuration(unsigned long newDuration) {
    duration = newDuration;
}

void Timer::reset(){
    startTime = millis();
}

void Timer::reset(unsigned long newDuration){
    setDuration(newDuration);
    reset();
}

