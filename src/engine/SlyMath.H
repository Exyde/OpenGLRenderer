#pragma once

namespace SlyMath {

template <typename T>
static T Lerp(const T& a, const T& b, float t) {
    return a + (b - a) * t;
}

template <typename T>
static T Clamp(const T& value, const T& min, const T& max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

}