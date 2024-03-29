#pragma once

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix2x2.h"
#include "math/matrix3x3.h"
#include "math/matrix4x4.h"


namespace kl::math {
    inline float line_x(const float2& a, const float2& b, float y)
    {
        return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x;
    }

    inline float line_y(const float2& a, const float2& b, float x)
    {
        return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y;
    }

    template <typename T = float, typename T0, typename T1, typename T2>
    T normalize(const T0& val, const T1& lower, const T2& upper)
    {
        return (T(val) - T(lower)) / (T(upper) - T(lower));
    }

    template <typename T, typename T0, typename T1>
    T minmax(const T& val, const T0& min_value, const T1& max_value)
    {
        return std::min(std::max(val, static_cast<T>(min_value)), static_cast<T>(max_value));
    }

    template <typename T>
    vector4<T> to_quaternion(const vector3<T>& eul)
    {
        const T cr = T(std::cos(math::to_radians(eul.x) * 0.5f));
        const T sr = T(std::sin(math::to_radians(eul.x) * 0.5f));
        const T cp = T(std::cos(math::to_radians(eul.y) * 0.5f));
        const T sp = T(std::sin(math::to_radians(eul.y) * 0.5f));
        const T cy = T(std::cos(math::to_radians(eul.z) * 0.5f));
        const T sy = T(std::sin(math::to_radians(eul.z) * 0.5f));

        return {
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        };
    }

    template <typename T>
    vector3<T> to_euler(const vector4<T>& quaternion)
    {
        const T sin_p = T(2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x));
        const T sin_r_cos_p = T(2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z));
        const T cos_r_cos_p = T(1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y));
        const T sin_y_cos_p = T(2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y));
        const T cos_y_cos_p = T(1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z));

        return {
            math::to_degrees<T>(std::atan2(sin_r_cos_p, cos_r_cos_p)),
            math::to_degrees<T>((std::abs(sin_p) >= 1.0f) ? std::copysign(pi * 0.5f, sin_p) : std::asin(sin_p)),
            math::to_degrees<T>(std::atan2(sin_y_cos_p, cos_y_cos_p))
        };
    };
}
