#ifndef INTERVAL_H
#define INTERVAL_H

#include <cmath>
class interval {
  public:
    double min, max;

    interval() : min(+INFINITY), max(-INFINITY) {} 

    interval(double min, double max) : min(min), max(max) {}

    constexpr double size() const noexcept {
        return max - min;
    }

    constexpr bool contains(double x) const noexcept  {
        return min <= x && x <= max;
    }

    constexpr bool surrounds(double x) const noexcept {
        return min < x && x < max;
    }

    constexpr double clamp(double x) const noexcept {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

inline const interval interval::empty    = interval(+INFINITY, -INFINITY);
inline const interval interval::universe = interval(-INFINITY, +INFINITY);

#endif