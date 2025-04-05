/**
 * @file timecontrol.h
 * @brief Header file for the timecontrol class and related functionalities.
 * This file declares the timecontrol class, which provides functionalities for time management.
 * @author ATphonOS 
 * @version v1.0.0
 * @date 2024
 * MIT license
 */

#ifndef TIMECONTROL_H
#define TIMECONTROL_H

//#include <stdint.h> included in Arduino.h
#include <Arduino.h>

/**
 * @brief Enumerates the direction of time conversion.
 */
enum TimeDirection {
  MillisecondsToSeconds, /**< Convert milliseconds to seconds. */
  SecondsToMilliseconds  /**< Convert seconds to milliseconds. */
};

// time constants
const uint16_t SECONDS_PER_DAY = 86400; /**< The number of seconds in a day. */
const uint16_t SECONDS_PER_HOUR = 3600; /**< The number of seconds in an hour. */
const uint8_t SECONDS_PER_MINUTE = 60;  /**< The number of seconds in a minute. */

class timecontrol {
public:
  timecontrol();                                                        /**< Default constructor. */
  timecontrol(uint32_t timelapse);                                      /**< Constructor with timelapse parameter. */
  timecontrol(uint32_t timelapse, bool state, uint32_t previousMillis); /**< Constructor with timelapse, state, and previousMillis parameters. */
  ~timecontrol(){};                                                     /**< Destructor. */

  /**
   * @brief Get the timelapse value.
   * @return The timelapse value in milliseconds.
   */
  inline uint32_t getTimelapse() const {
    return _timelapse;
  }

  /**
   * @brief Set the state of timecontrol. 
   * @param state The state to set (true for active, false for inactive).
   */
  inline void setState(bool state) {
    _state = state;
  }

  /**
   * @brief Get the state of timecontrol.
   * @return The state of timecontrol (true for active, false for inactive).
   */
  inline bool getState() const {
    return _state;
  }

  /**
   * @brief Stop the timecontrol.
   */
  inline void stop() {
    _state = false;
  }

  /**
   * @brief Resume the timecontrol.
   */
  inline void resume() {
    _state = true;
  }

  /**
   * @brief Check if the timelapse has elapsed.
   * @return True if the timelapse has elapsed, false otherwise.
   */
  bool elapsed();

  /**
   * @brief Execute a function if the timelapse has elapsed.
   * @param function The function to execute.
   */
  void elapsedExec(void (*function)(void));

  /**
   * @brief Convert seconds to time format (HH:MM:SS).
   * @param sec The time in seconds.
   * @return A character array representing the time in HH:MM:SS format.
   */
  char* secToTime(uint32_t sec) const;

  /**
   * @brief Print the current runtime. 
   * @return A character array representing the current runtime in HH:MM:SS format.
   */
  char* printRunTime() const {
    return secToTime(millis() / 1000);
  }

  /**
   * @brief Print the time in HH:MM:SS format.
   * @param sec The time in seconds.
   * @return A character array representing the time in HH:MM:SS format.
   */
  char* printTime(uint32_t sec) const {
    return secToTime(sec);
  }

  /**
   * @brief Convert milliseconds to seconds. 
   * @return The time in seconds.
   */
  inline uint32_t millisToSeconds() const {
    return millis() / 1000;
  }

  /**
   * @brief Convert time between milliseconds and seconds. 
   * @param time The time value to convert.
   * @param direction The direction of conversion (MillisecondsToSeconds or SecondsToMilliseconds).
   * @return The converted time value.
   */
  inline uint32_t convertTime(uint32_t time, TimeDirection direction) {
    return (direction == MillisecondsToSeconds) ? (time / 1000) : (time * 1000);
  }

  /**
   * @brief Reset the timer to its initial state without altering its running status or timelapse.
   * 
   * This function resets the reference times for both milliseconds and microseconds, 
   * clears the event counter, and resets the last elapsed time record. It does not 
   * affect the timer's state (_state) or configured timelapse (_timelapse).
   */
  inline void reset() {
    pMillis = millis();
    _pMicros = micros();
    _count = 0;
    _lastElapsedTime = 0;
  }

  /**
   * @brief Get the elapsed time since the last reset or event, in milliseconds.
   * 
   * This function returns the time elapsed since the last reset or elapsed event, 
   * but only if the timer is currently running. If the timer is stopped, it returns 0.
   * 
   * @return The elapsed time in milliseconds if running, 0 otherwise.
   */
  inline uint32_t getElapsedTime() const {
    return _state ? (millis() - pMillis) : 0;
  }

  /**
   * @brief Set the timelapse interval for the timer.
   * 
   * This function updates the timelapse value that determines how often the timer 
   * triggers elapsed events.
   * 
   * @param timelapse The new timelapse value in milliseconds (or microseconds for elapsedMicros).
   */
  inline void setTimelapse(uint32_t timelapse) {
    _timelapse = timelapse;
  }

  /**
   * @brief Check if the timer is currently running.
   * 
   * This function returns the current state of the timer, indicating whether it is 
   * actively counting time or paused.
   * 
   * @return True if the timer is running, false if paused.
   */
  inline bool isRunning() const {
    return _state;
  }

  /**
   * @brief Get the remaining time until the next elapsed event, in milliseconds.
   * 
   * This function calculates how much time remains until the timelapse is reached, 
   * based on the current elapsed time. If the timer is stopped or the timelapse has 
   * already elapsed, it returns 0.
   * 
   * @return The remaining time in milliseconds, or 0 if stopped or elapsed.
   */
  uint32_t remainingTime();

  /**
   * @brief Toggle the timer state between running and stopped.
   */
  inline void toggleState() {
    _state = !_state;
  }

  /**
   * @brief Pause the timer and return the elapsed time.
   * @return The elapsed time in milliseconds before pausing.
   */
  inline uint32_t pauseAndGetElapsed() {
    uint32_t elapsed = getElapsedTime();
    stop();
    return elapsed;
  }

  /**
   * @brief Restart the timer by resetting and ensuring it is running.
   */
  inline void restart() {
    reset();
    resume();
  }

  /**
   * @brief Get the number of times elapsed() has occurred since last reset.
   * @return The count of elapsed events.
   */
  inline uint32_t elapsedCount() const {
    return _count;
  }

  /**
   * @brief Set a permanent callback function to be executed when elapsed() is true.
   * @param callback The function to execute.
   */
  inline void setCallback(void (*callback)(void)) {
    _callback = callback;
  }

  /**
   * @brief Check if the timelapse has elapsed, using seconds instead of milliseconds.
   * @return True if the timelapse (in seconds) has elapsed, false otherwise.
   */
  bool elapsedSeconds();

  /**
   * @brief Configure the timer to run once and stop after the first elapse.
   */
  inline void runOnce() {
    _repeatCount = 1;
    resume();
  }

  /**
   * @brief Set the number of times the timer should repeat before stopping.
   * @param count Number of repetitions (0 for infinite).
   */
  inline void setRepeatCount(uint32_t count) {
    _repeatCount = count;
  }

  /**
   * @brief Get the total elapsed time since the timer was created or fully reset.
   * @return Total elapsed time in milliseconds.
   */
  inline uint32_t getTotalElapsedTime() const {
    return millis() - _startTime;
  }

  /**
   * @brief Adjust the timelapse by adding or subtracting a value.
   * @param adjustment Amount to adjust (positive to increase, negative to decrease).
   */
  inline void adjustTimelapse(int32_t adjustment) {
    int32_t newTimelapse = (int32_t)_timelapse + adjustment;
    _timelapse = (newTimelapse > 0) ? newTimelapse : 0;
  }

  /**
   * @brief Check if the timer is significantly overdue (exceeds timelapse by 2x).
   * @return True if overdue, false otherwise.
   */
  inline bool isOverdue() const {
    return _state && (millis() - pMillis > _timelapse * 2);
  }

  /**
   * @brief Set the start time for total elapsed time calculation.
   * @param startTime The external start time in milliseconds.
   */
  inline void setStartTime(uint32_t startTime) {
    _startTime = startTime;
  }

  /**
   * @brief Pause the timer and store elapsed time; resume later with stored value.
   * @param elapsedOut Reference to store the elapsed time when paused.
   */
  void pauseAndResumeLater(uint32_t& elapsedOut);

  /**
   * @brief Check if the timelapse has elapsed, using microseconds instead of milliseconds.
   * @return True if the timelapse (in microseconds) has elapsed, false otherwise.
   */
  bool elapsedMicros();

  /**
   * @brief Fully reset the timer, including start time, to initial state.
   */
  inline void fullReset() {
    _startTime = millis();
    pMillis = _startTime;
    _pMicros = micros();
    _count = 0;
    _lastElapsedTime = 0;
    _state = true;
  }

  /**
   * @brief Get the configured repeat count.
   * @return The number of repetitions set (0 for infinite).
   */
  inline uint32_t getRepeatCount() const {
    return _repeatCount;
  }

  /**
   * @brief Get the duration of the last elapsed event, in milliseconds.
   * 
   * This function returns the time recorded for the most recent elapsed event, 
   * which is the duration between the previous and current trigger points. The 
   * value is stored in _lastElapsedTime and updated by elapsed(), elapsedSeconds(), 
   * elapsedMicros(), or interruptHandler().
   * 
   * @return The duration of the last elapsed event in milliseconds, or 0 if no event has occurred yet.
   */
  inline uint32_t getLastElapsedTime() const {
    return _lastElapsedTime;
  }

  /**
   * @brief Attach an interrupt to trigger the timer's callback on a pin event.
   * 
   * This function configures an external interrupt on the specified pin, linking it 
   * to the timer's interrupt handler. When the interrupt occurs, it executes the 
   * callback function set by setCallback() and updates the timer's state accordingly.
   * 
   * @param pin The digital pin number to monitor for interrupts (must support interrupts on the board).
   * @param mode The interrupt mode (e.g., RISING, FALLING, CHANGE) that triggers the callback.
   */
  void attachInterrupt(uint8_t pin, uint8_t mode);

  /**
   * @brief Wait for a specified duration without affecting the main timer.
   * @param 'duration' Duration to wait in milliseconds.
   */
  void wait(uint32_t duration);

  /**
   * @brief Check if a specified time has elapsed since a reference time.
   * @param referenceTime The reference time in milliseconds.
   * @return True if timelapse has elapsed since referenceTime, false otherwise.
   */
  inline bool elapsedSince(uint32_t referenceTime) {
    return (millis() - referenceTime >= _timelapse);
  }

  /**
   * @brief Toggle between infinite repeat and single execution.
   */
  inline void toggleRepeat() {
    _repeatCount = (_repeatCount == 0) ? 1 : 0;
  }

  /**
   * @brief Set a callback that receives the elapsed time as a parameter.
   * @param callback The function to execute with elapsed time.
   */
  inline void setElapsedCallback(void (*callback)(uint32_t)) {
    _elapsedCallback = callback;
  }

  /**
   * @brief Check if a custom interval has elapsed without affecting the main timer.
   * @param interval The interval to check in milliseconds.
   * @return True if the interval has elapsed, false otherwise.
   */
  inline bool elapsedInterval(uint32_t interval) const {
    return (millis() - pMillis >= interval);
  }

  /**
   * @brief Get the remaining number of repetitions.
   * @return Remaining repetitions, or 0 if infinite.
   */
  inline uint32_t getRemainingCount() const {
    return (_repeatCount > 0 && _count < _repeatCount) ? (_repeatCount - _count) : 0;
  }

  /**
   * @brief Format the current elapsed time into a provided buffer as HH:MM:SS.
   * 
   * This function calculates the elapsed time since the last reset or event, converts 
   * it into hours, minutes, and seconds, and formats it into a user-provided buffer 
   * in the format "HH:MM:SS". The buffer must be large enough to hold the formatted 
   * string (minimum 9 bytes including null terminator).
   * 
   * @param buffer Pointer to the character array where the formatted time will be stored.
   * @param bufferSize Size of the buffer in bytes, to prevent overflow.
   */
  void formatElapsedTime(char* buffer, uint8_t bufferSize) const;

  /**
   * @brief Set the priority of callback execution.
   * @param useElapsedFirst True to execute elapsedCallback first, false for simple callback first.
   */
  inline void setPriorityCallback(bool useElapsedFirst) {
    _useElapsedFirst = useElapsedFirst;
  }

  /**
   * @brief Pause all instances of timecontrol.
   */
  static void pauseAll();

  /**
   * @brief Resume the timer from an interrupt handler.
   */
  inline void resumeFromInterrupt() {
    if (!_state) resume();
  }

  /**
   * @brief Check if total elapsed time exceeds a timeout.
   * @param timeout Timeout in milliseconds.
   * @return True if time is up, false otherwise.
   */
  inline bool isTimeUp(uint32_t timeout) const {
    return getTotalElapsedTime() >= timeout;
  }

  /**
   * @brief Get the average elapsed time over a number of samples.
   * @param samples Number of samples to average (max 10).
   * @return Average elapsed time in milliseconds.
   */
  uint32_t getAverageElapsedTime(uint8_t samples);

   /**
   * @brief Start or check a countdown with an optional callback when it reaches zero.
   * @param duration The countdown duration in milliseconds.
   * @param callback Optional function to execute when countdown reaches zero.
   * @return The remaining time in milliseconds, or 0 if countdown has finished.
   */
uint32_t countdown(uint32_t duration, void (*callback)(void) = nullptr);

private:
  uint32_t _timelapse;                /**< The timelapse value in milliseconds. */
  uint32_t pMillis;                   /**< Previous millis value for time control. */
  uint32_t _count;                    /**< Counter for elapsed events. */
  bool _state;                        /**< The state of timecontrol (true for active, false for inactive). */
  uint32_t _startTime;                /**< Time of creation or full reset for total elapsed time. */
  uint32_t _repeatCount;              /**< Number of repetitions (0 for infinite). */
  uint32_t _pMicros;                  /**< Previous micros value for microsecond timing. */
  uint32_t _lastElapsedTime;          /**< Time of the last elapsed event in milliseconds. */
  void (*_elapsedCallback)(uint32_t); /**< Callback with elapsed time parameter. */
  bool _useElapsedFirst;              /**< Priority of elapsed callback execution. */
  uint32_t _elapsedTimes[10];         /**< Buffer for storing last elapsed times (max 10 samples). */
  uint8_t _elapsedIndex;              /**< Index for elapsed times buffer. */
  void (*_callback)(void);            /**< Callback function for elapsed events. */
  static char _buffer[16];            /**< Static buffer for storing formatted time strings (optimized size for HH:MM:SS). */

  static void interruptHandler();
  static timecontrol* _instance;
};

#endif  // TIMECONTROL_H
