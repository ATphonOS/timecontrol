/**
 * @file timecontrol.cpp
 * @brief The timecontrol library offers precise timing in milliseconds and microseconds, time unit conversions, 
 * callback execution for scheduled events, and support for external interrupts. 
 * Features include countdowns, configurable repetitions, dynamic interval adjustments, and time formatting.
 * @author ATphonOS 
 * @version v1.0.0
 * @date 2024
 * MIT license
 */

#include "timecontrol.h"

char timecontrol::_buffer[16];
timecontrol* timecontrol::_instance = nullptr;

timecontrol::timecontrol() 
  : _timelapse(0), _state(true), pMillis(0), _count(0), _callback(nullptr), 
    _startTime(millis()), _repeatCount(0), _pMicros(0), _lastElapsedTime(0), 
    _elapsedCallback(nullptr), _useElapsedFirst(false), _elapsedIndex(0) {
  pMillis = _startTime;
  _pMicros = micros();
  _instance = this;
  memset(_elapsedTimes, 0, sizeof(_elapsedTimes));  // Initialize the buffer
}

timecontrol::timecontrol(uint32_t timelapse) 
  : _timelapse(timelapse), _state(true), pMillis(0), _count(0), _callback(nullptr), 
    _startTime(millis()), _repeatCount(0), _pMicros(0), _lastElapsedTime(0), 
    _elapsedCallback(nullptr), _useElapsedFirst(false), _elapsedIndex(0) {
  pMillis = _startTime;
  _pMicros = micros();
  _instance = this;
  memset(_elapsedTimes, 0, sizeof(_elapsedTimes));
}

timecontrol::timecontrol(uint32_t timelapse, bool state, uint32_t previousMillis) 
  : _timelapse(timelapse), _state(state), pMillis(previousMillis), _count(0), 
    _callback(nullptr), _startTime(millis()), _repeatCount(0), _pMicros(0), 
    _lastElapsedTime(0), _elapsedCallback(nullptr), _useElapsedFirst(false), _elapsedIndex(0) {
  _pMicros = micros();
  _instance = this;
  memset(_elapsedTimes, 0, sizeof(_elapsedTimes));
}

bool timecontrol::elapsed() {
  if (!_state) return false;
  uint32_t current = millis();
  if (current - pMillis >= _timelapse) {
    _lastElapsedTime = current - pMillis;
    pMillis = current;
    _pMicros = micros();
    _elapsedTimes[_elapsedIndex] = _lastElapsedTime;
    _elapsedIndex = (_elapsedIndex + 1) % 10;  // Circular buffer
    _count++;
    if (_useElapsedFirst) {
      if (_elapsedCallback) _elapsedCallback(_lastElapsedTime);
      if (_callback) _callback();
    } else {
      if (_callback) _callback();
      if (_elapsedCallback) _elapsedCallback(_lastElapsedTime);
    }
    if (_repeatCount > 0 && _count >= _repeatCount) stop();
    return true;
  }
  return false;
}

void timecontrol::elapsedExec(void (*function)(void)) {
  if (elapsed()) {
    function();
  }
}

char* timecontrol::secToTime(uint32_t sec) const {
  uint16_t days = sec / SECONDS_PER_DAY;
  uint8_t hours = (sec % SECONDS_PER_DAY) / SECONDS_PER_HOUR;
  uint8_t minutes = (sec % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
  uint8_t seconds = sec % SECONDS_PER_MINUTE;

  if (days > 0) {
    snprintf(_buffer, sizeof(_buffer), "%d:%02d:%02d:%02d", days, hours, minutes, seconds);
  } else {
    snprintf(_buffer, sizeof(_buffer), "%02d:%02d:%02d", hours, minutes, seconds);
  }
  return _buffer;
}

uint32_t timecontrol::remainingTime() {
  if (!_state || elapsed()) return 0;
  uint32_t current = millis();
  uint32_t elapsedTime = current - pMillis;
  return (elapsedTime < _timelapse) ? (_timelapse - elapsedTime) : 0;
}

bool timecontrol::elapsedSeconds() {
  if (!_state) return false;
  uint32_t currentSec = millis() / 1000;
  uint32_t previousSec = pMillis / 1000;
  uint32_t timelapseSec = _timelapse / 1000;
  if (currentSec - previousSec >= timelapseSec) {
    pMillis = millis();
    _pMicros = micros();
    _lastElapsedTime = (currentSec - previousSec) * 1000;
    _elapsedTimes[_elapsedIndex] = _lastElapsedTime;
    _elapsedIndex = (_elapsedIndex + 1) % 10;
    _count++;
    if (_useElapsedFirst) {
      if (_elapsedCallback) _elapsedCallback(_lastElapsedTime);
      if (_callback) _callback();
    } else {
      if (_callback) _callback();
      if (_elapsedCallback) _elapsedCallback(_lastElapsedTime);
    }
    if (_repeatCount > 0 && _count >= _repeatCount) stop();
    return true;
  }
  return false;
}

void timecontrol::pauseAndResumeLater(uint32_t& elapsedOut) {
  if (_state) {
    elapsedOut = getElapsedTime();
    stop();
  } else if (elapsedOut > 0) {
    pMillis = millis() - elapsedOut;
    _pMicros = micros();
    resume();
    elapsedOut = 0;
  }
}

bool timecontrol::elapsedMicros() {
  if (!_state) return false;
  uint32_t current = micros();
  if (current - _pMicros >= _timelapse) {
    _lastElapsedTime = (current - _pMicros) / 1000;
    _pMicros = current;
    pMillis = millis();
    _elapsedTimes[_elapsedIndex] = _lastElapsedTime;
    _elapsedIndex = (_elapsedIndex + 1) % 10;
    _count++;
    if (_useElapsedFirst) {
      if (_elapsedCallback) _elapsedCallback(_lastElapsedTime);
      if (_callback) _callback();
    } else {
      if (_callback) _callback();
      if (_elapsedCallback) _elapsedCallback(_lastElapsedTime);
    }
    if (_repeatCount > 0 && _count >= _repeatCount) stop();
    return true;
  }
  return false;
}

void timecontrol::attachInterrupt(uint8_t pin, uint8_t mode) {
  ::attachInterrupt(digitalPinToInterrupt(pin), interruptHandler, mode);
}

void timecontrol::interruptHandler() {
  if (_instance && _instance->_callback) {
    _instance->_count++;
    _instance->_lastElapsedTime = _instance->getElapsedTime();
    _instance->pMillis = millis();
    _instance->_pMicros = micros();
    if (_instance->_useElapsedFirst) {
      if (_instance->_elapsedCallback) _instance->_elapsedCallback(_instance->_lastElapsedTime);
      if (_instance->_callback) _instance->_callback();
    } else {
      if (_instance->_callback) _instance->_callback();
      if (_instance->_elapsedCallback) _instance->_elapsedCallback(_instance->_lastElapsedTime);
    }
    _instance->_elapsedTimes[_instance->_elapsedIndex] = _instance->_lastElapsedTime;
    _instance->_elapsedIndex = (_instance->_elapsedIndex + 1) % 10;
    if (_instance->_repeatCount > 0 && _instance->_count >= _instance->_repeatCount) 
      _instance->stop();
    else
      _instance->resumeFromInterrupt();
  }
}

void timecontrol::wait(uint32_t duration) {
  uint32_t start = millis();
  while (millis() - start < duration) {}
}

void timecontrol::formatElapsedTime(char* buffer, uint8_t bufferSize) const {
  uint32_t elapsedSec = getElapsedTime() / 1000;
  uint8_t hours = elapsedSec / SECONDS_PER_HOUR;
  uint8_t minutes = (elapsedSec % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
  uint8_t seconds = elapsedSec % SECONDS_PER_MINUTE;
  snprintf(buffer, bufferSize, "%02d:%02d:%02d", hours, minutes, seconds);
}

uint32_t timecontrol::getAverageElapsedTime(uint8_t samples) {
  if (samples > 10) samples = 10;  // Maximum 10 samples
  if (_count == 0) return 0;
  uint32_t sum = 0;
  uint8_t validSamples = min(samples, _count);
  for (uint8_t i = 0; i < validSamples; i++) {
    uint8_t idx = (_elapsedIndex - 1 - i + 10) % 10;
    sum += _elapsedTimes[idx];
  }
  return sum / validSamples;
}

void timecontrol::pauseAll() {
  if (_instance) _instance->stop();
}

uint32_t timecontrol::countdown(uint32_t duration, void (*callback)(void)) {
    if (!_state) {  // Start countdown if not running
        pMillis = millis();
        _timelapse = duration;
        _state = true;
    }
    
    uint32_t elapsed = millis() - pMillis;
    if (elapsed >= _timelapse) {
        _state = false;  // Stop when it reaches zero
        if (callback) callback();  // Execute callback if it exists
        return 0;
    }
    return _timelapse - elapsed;  // Return remaining time
}
