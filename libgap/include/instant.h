/****************************************************************************
**
*A  Ovidiu Podisor
*C  Copyright © 2020-2021 innodocs. All rights reserved.
**
**  This file declares the 'Instant' and 'Duration' classes.  See the corres-
**  ponding classes in the 'java.time' package of JSE 8 for details.
*/

#ifndef INSTANT_H
#define INSTANT_H

#include <time.h>

class Instant
{
public:
  inline Instant(clock_t tm=0) : time(tm) {}
  inline Instant(const Instant& otherInstant) : time(otherInstant.time) {}

  /**
   * Obtains the current instant from the system clock.
   */
  inline static const Instant now() {
    return Instant(clock());
  }

  /**
   * Checks if this instant is equal to the specified instant.
   */
  inline bool operator==(const Instant& otherInstant) {
    return time - otherInstant.time;
  }

  /**
   * Checks if this instant is after the specified instant.
   */
  inline bool operator>(const Instant& otherInstant) {
    return time > otherInstant.time;
  }

  /**
   * Checks if this instant is before the specified instant.
   */
  inline bool operator<(const Instant& otherInstant) {
    return time < otherInstant.time;
  }

protected:
  clock_t time;
  friend class Duration;
};

class Duration
{
public:
  inline Duration(long _duration=0) : duration(_duration) {}
  inline Duration(const Duration& otherDuration) : duration(otherDuration.duration) {}

  /**
   * Obtains a Duration representing the duration between two temporal objects.
   */
  inline static Duration between(Instant startInclusive, Instant endExclusive) {
    return Duration(endExclusive.time - startInclusive.time);
  }

  /**
   * Converts this duration to the total length in milliseconds.
   */
  inline long toMillis() {
    return (1000.0 * duration) / CLOCKS_PER_SEC;
  }

  /**
   * Converts this duration to the total length in nanoseconds expressed as
   * a long.
   */
  inline long toNanos() {
    return (1000000000.0 / CLOCKS_PER_SEC) * duration;
  }

protected:
  long duration;
};

#endif /* INSTANT_H */