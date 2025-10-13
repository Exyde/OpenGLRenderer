#pragma once

#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(float x) : x(x), y(x), z(x) {}

    // Operator overloads
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

    Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vector3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3& operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    // Vector operations
    float Magnitude() const { return std::sqrt(x * x + y * y + z * z); }

    glm::vec3 GLM() const { return glm::vec3(x, y, z);}

    Vector3 Normalized() const {
        float mag = Magnitude();
        return mag > 0 ? *this / mag : Vector3();
    }
    float Dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vector3 Cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    // Static vectors
    static Vector3 Zero() { return Vector3(0, 0, 0); }
    static Vector3 One() { return Vector3(1, 1, 1); }
    static Vector3 Up() { return Vector3(0, 1, 0); }
    static Vector3 Down() { return Vector3(0, -1, 0); }
    static Vector3 Left() { return Vector3(-1, 0, 0); }
    static Vector3 Right() { return Vector3(1, 0, 0); }
    static Vector3 Forward() { return Vector3(0, 0, 1); }
    static Vector3 Back() { return Vector3(0, 0, -1); }

    // Output
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};