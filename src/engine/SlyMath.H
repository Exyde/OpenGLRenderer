#pragma once

namespace SlyMath {

    // Linearly interpolates between a and b by t (t in [0, 1])
    template<typename T>
    static T Lerp(const T& a, const T& b, float t) {
        return a + (b - a) * t;
    }

    // Clamps value between min and max
    template<typename T>
    static T Clamp(const T& value, const T& min, const T& max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

}