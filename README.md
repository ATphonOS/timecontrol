# timecontrol Library

## Overview

The library provides ***non-blocking timing control for multitasking use***, except for the `wait()` function, which is a specific and optional utility. Its design based on time comparisons (millis() and micros()) ensures that timing operations are fast and do not interfere with other tasks, making it ideal for applications where continuous flow is critical.

## Installation

1. [Download](https://github.com/ATphonOS/timecontrol/archive/refs/heads/main.zip) this repository.
2. **Install**: Add the library to the Arduino IDE via the ZIP file.
   - Open the Arduino IDE.  
   - Go to `Sketch > Include Library > Add .ZIP Library...`.  
   - Navigate to the location where you saved `timecontrol-main.zip`, select it, and click "Open".  
   - The IDE will install the library automatically into your `libraries` directory.
3. **Include**: Add the library to your sketch with `#include "timecontrol.h"` or  `#include <timecontrol.h>`.
4. **Verify**:  Restart the Arduino IDE if it was open during installation to ensure the library is recognized.  
   - After restarting, you can confirm the library is available under `Sketch > Include Library`, where "timecontrol" should appear in the list.


## Requirements

- **Software**: Arduino IDE (version 1.8.x or later) or any compatible development environment.
- **Hardware**: Arduino-compatible boards supporting `millis()`, `micros()`, and hardware interrupts.
- **Dependencies**: None beyond the standard Arduino core library (`Arduino.h`).

## Features

- Accurate timing in milliseconds and microseconds.
- Flexible time unit conversion (milliseconds ↔ seconds).
- Callback execution for elapsed events, with optional elapsed time data.
- External interrupt support for event-driven triggers.
- Countdown mode with optional completion callback.
- Time formatting in HH:MM:SS / DAYS HH:MM:SS for easy readability.
- Configurable repetitions and runtime timelapse adjustments.
- Event tracking with elapsed count and average time calculation.
- Lightweight and optimized for embedded systems.

## Public Methods

Below is a table of all public methods in the `timecontrol` class, including both inline and non-inline methods.

### Constructors and Destructor


| Constructors | Description |
| --- | --- |
| `timecontrol()` | Default constructor, initializes with a timelapse of 0 and active state |
| `timecontrol(uint32_t timelapse)` | Constructor with specified timelapse in milliseconds |
| `timecontrol(uint32_t timelapse, bool state, uint32_t previousMillis)` | Constructor with timelapse, initial state, and custom previous millisecond reference |

&nbsp;<br>&nbsp;

| Destructor | Description |
| --- | --- |
| `~timecontrol()` | Empty destructor (no dynamic memory to free) |

### Core Timing Methods

| Method | Description |
| --- | --- |
| `bool elapsed()` |Checks if the timelapse has elapsed, updates state, and triggers callbacks if set. Returns `true` on elapse.  |
| `bool elapsedSeconds()` |Similar to `elapsed()`, but operates in seconds instead of milliseconds. |
| `bool elapsedMicros()` |Checks elapsed time in microseconds, using `_timelapse` as microseconds. |
| `uint32_t countdown(uint32_t duration, void (*callback)(void) = nullptr)` |Starts or checks a countdown, returning remaining time in milliseconds. Executes the optional callback when it reaches zero. |
| `uint32_t remainingTime()` |Returns the time in milliseconds until the next `elapsed()` event, or 0 if stopped or elapsed.  |

### State Management (Inline Methods)


| Method | Description |
| --- | --- |
| `inline void setState(bool state)` |Sets the timer state (`true` for active, `false` for inactive). |
| `inline bool getState() const` |Returns the current state (`true` if running).  |
| `inline void stop()` |Stops the timer by setting `_state` to `false`.  |
| `inline void resume()` |Resumes the timer by setting `_state` to `true`. |
| `inline bool isRunning() const` |Alias for `getState()`, returns `true` if active. |
| `inline void toggleState()` |Toggles the timer state between running and stopped.|
| `inline void restart()` |Resets and resumes the timer.|
| `inline void runOnce()` |Configures the timer to run once and stop after one elapse.  |
| `inline void fullReset()` |Fully resets the timer, including `_startTime`, to initial state. |


### Timelapse and Count Management (Inline)

| Method | Description |
| --- | --- |
| `inline uint32_t getTimelapse() const` |Returns the current timelapse value in milliseconds.  |
| `inline void setTimelapse(uint32_t timelapse)` |Updates the timelapse value.  |
| `inline void adjustTimelapse(int32_t adjustment)` |Adjusts `_timelapse` by adding or subtracting, ensuring it stays non-negative.  |
| `inline uint32_t elapsedCount() const` |Returns the number of elapsed events since last reset.  |
| `inline void setRepeatCount(uint32_t count)` |Sets the number of repetitions (0 for infinite).  |
| `inline uint32_t getRepeatCount() const` |Returns the configured repeat count.  |
| `inline void toggleRepeat()` |Switches between infinite (0) and single (1) repeat modes.  |
| `inline uint32_t getRemainingCount() const` | Returns remaining repetitions, or 0 if infinite.  |

### Time Queries (Inline Methods)

| Method | Description |
| --- | --- |
| `inline uint32_t getElapsedTime() const` |Returns elapsed time since last reset if running, 0 if stopped.  |
| `inline uint32_t getTotalElapsedTime() const` |Returns total time since creation or full reset. Inline for `millis() - _startTime`.|
| `inline uint32_t getLastElapsedTime() const` |Returns the duration of the last elapsed event. |
| `inline uint32_t millisToSeconds() const` |Converts current `millis()` to seconds. Inline for simple division.  |
| `inline uint32_t convertTime(uint32_t time, TimeDirection direction)` |Converts between milliseconds and seconds based on `direction`. |
| `inline bool elapsedSince(uint32_t referenceTime)` |Checks if `_timelapse` has elapsed since a given reference time.  |
| `inline bool elapsedInterval(uint32_t interval) const` |Checks if a custom interval has elapsed since last reset.  |
| `inline bool isOverdue() const` |Returns `true` if elapsed time exceeds `_timelapse * 2`. Inline for fast overrun detection.  |
| `inline bool isTimeUp(uint32_t timeout) const` |Checks if total elapsed time exceeds a timeout.|


### Callback and Interrupt Management (Methods and Inline Methods)


| Method | Description |
| --- | --- |
| `void elapsedExec(void (*function)(void))` |Executes a function if `elapsed()` is true.  |
| `inline void setCallback(void (*callback)(void))` |Sets a permanent callback for `elapsed()` events. |
| `inline void setElapsedCallback(void (*callback)(uint32_t))` |Sets a callback with elapsed time parameter. |
| `inline void setPriorityCallback(bool useElapsedFirst)` |Sets callback execution order (`true` for elapsed callback first). |
| `void attachInterrupt(uint8_t pin, uint8_t mode)` |Links an interrupt to a pin, triggering `interruptHandler()` on events.  |
| `inline void resumeFromInterrupt()` |Resumes the timer if stopped, used in interrupt contexts. |


### Time Formatting and Utilities

| Method | Description |
| --- | --- |
| `char* secToTime(uint32_t sec) const` |Converts seconds to HH:MM:SS or D:HH:MM:SS format, stored in a static buffer.  |
| `char* printRunTime() const` |Returns current runtime in HH:MM:SS format via `secToTime()`.  |
| `char* printTime(uint32_t sec) const` |Formats a given time in seconds as HH:MM:SS.  |
| `void formatElapsedTime(char* buffer, uint8_t bufferSize) const` |Formats current elapsed time into a user-provided buffer as HH:MM:SS.|
| `void wait(uint32_t duration)` |Blocks execution for a specified duration in milliseconds.  |


### Reset and Sync (Inline Methods and Methods)
| Method | Description |
| --- | --- |
| `inline void reset()` |Resets reference times and counters without altering state or timelapse. Inline for quick reset.  |
| `inline uint32_t pauseAndGetElapsed()` |Pauses the timer and returns elapsed time. Inline for combined stop and query. |
| `void pauseAndResumeLater(uint32_t& elapsedOut)` |Pauses and stores elapsed time, or resumes with stored value.  |
| `inline void setStartTime(uint32_t startTime)` |Sets the start time for total elapsed tracking. Inline for `_startTime` update.  |

### Static and Advanced Functions
| Method | Description |
| --- | --- |
| `static void pauseAll()` |Pauses all instances (currently limited to one due to single `_instance` design).  |
| `uint32_t getAverageElapsedTime(uint8_t samples)` | Returns the average elapsed time over a specified number of samples (max 10)  |

## Inline Methods Explained

Many functions in `timecontrol` are marked as `inline` to optimize performance by avoiding function call overhead. These are typically simple operations, categorized as follows:

| Category              | Method                         | Description                                                  |
|-----------------------|-----------------------------------|--------------------------------------------------------------|
| **Direct Member Access** | `getTimelapse()`                 | Returns the current `_timelapse` value directly.             |
|                       | `getState()`                     | Returns the current `_state` value directly.                 |
|                       | `isRunning()`                    | Alias for `getState()`, returns `_state`.                    |
|                       | `elapsedCount()`                 | Returns the `_count` of elapsed events directly.             |
|                       | `getRepeatCount()`               | Returns the configured `_repeatCount` directly.              |
|                       | `getLastElapsedTime()`           | Returns the `_lastElapsedTime` of the last event directly.   |
|                       | `getTotalElapsedTime()`          | Returns total time since `_startTime` (`millis() - _startTime`). |
|                       | `getRemainingCount()`            | Returns remaining repetitions based on `_repeatCount` and `_count`. |
| **Simple Assignments**   | `setState()`                     | Assigns a new value to `_state` with minimal logic.          |
|                       | `setTimelapse()`                 | Assigns a new value to `_timelapse` directly.                |
|                       | `setCallback()`                  | Assigns a callback function to `_callback`.                  |
|                       | `setElapsedCallback()`           | Assigns a callback with elapsed time to `_elapsedCallback`.  |
|                       | `setPriorityCallback()`          | Assigns a value to `_useElapsedFirst` for callback order.    |
|                       | `setRepeatCount()`               | Assigns a new value to `_repeatCount` directly.              |
|                       | `setStartTime()`                 | Assigns a new value to `_startTime` directly.                |
| **Basic Arithmetic**     | `getElapsedTime()`               | Calculates `millis() - pMillis` if running, else 0.          |
|                       | `convertTime()`                  | Converts time between milliseconds and seconds.              |
|                       | `adjustTimelapse()`              | Adjusts `_timelapse` by adding/subtracting, ensures non-negative. |
|                       | `millisToSeconds()`              | Divides `millis()` by 1000 to get seconds.                   |
|                       | `elapsedSince()`                 | Checks if `_timelapse` has passed since `referenceTime`.     |
|                       | `elapsedInterval()`              | Checks if a custom interval has passed since `pMillis`.      |
|                       | `isOverdue()`                    | Returns true if `millis() - pMillis > _timelapse * 2`.       |
|                       | `isTimeUp()`                     | Checks if `getTotalElapsedTime()` exceeds a timeout.         |
| **Convenience Wrappers** | `stop()`                         | Sets `_state` to `false` in a single call.                   |
|                       | `resume()`                       | Sets `_state` to `true` in a single call.                    |
|                       | `toggleState()`                  | Flips `_state` between true and false.                       |
|                       | `restart()`                      | Calls `reset()` and `resume()` together.                     |
|                       | `runOnce()`                      | Sets `_repeatCount` to 1 and calls `resume()`.               |
|                       | `fullReset()`                    | Resets `_startTime`, `pMillis`, `_pMicros`, `_count`, and sets `_state` to true. |
|                       | `pauseAndGetElapsed()`           | Returns `getElapsedTime()` and calls `stop()`.               |
|                       | `toggleRepeat()`                 | Switches `_repeatCount` between 0 (infinite) and 1 (single). |
|                       | `resumeFromInterrupt()`          | Calls `resume()` if `_state` is false.                       |

Inlining is beneficial for frequent calls in time-critical loops, reducing execution time at the cost of slightly larger binary size. These functions are defined in the header file (`timecontrol.h`) to allow the compiler to inline them at compile time.

## Usage Notes

- **Precision**: Relies on Arduino’s `millis()` and `micros()`, which may drift over long periods or reset after ~49 days (millis) or ~70 minutes (micros).
- **Interrupts**: `attachInterrupt()` and `pauseAll()` currently support only one instance due to the static `_instance` design.
- **Buffer Size**: For `formatElapsedTime()`, provide a buffer of at least 9 bytes (`"HH:MM:SS\0"`) to avoid truncation.
- **Thread Safety**: Not designed for multi-threaded environments; use with caution in interrupt-heavy applications.

## Limitations

- **Single Instance Interrupt**: Only one `timecontrol` object can handle interrupts due to the static `_instance` pointer.
- **No Overflow Handling**: Does not explicitly handle `millis()` or `micros()` overflow, though Arduino’s subtraction method mitigates this for differences.
- **Static Buffer**: `secToTime()` and related functions use a shared static buffer, which may overwrite data in concurrent calls.
