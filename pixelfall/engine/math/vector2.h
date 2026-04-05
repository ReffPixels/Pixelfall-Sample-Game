// Defines a basic Vector2 and Vector2Int.

#pragma once
// Standard Library
#include <cmath>
#include <algorithm>
// Math
#include "pixelfall/engine/math/numbers.h"

class Vector2 {
public:
    // Members
    float x, y;

    // Constructors
    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}

    // Static direction vectors
    static const Vector2 Zero;
    static const Vector2 Up;
    static const Vector2 Down;
    static const Vector2 Left;
    static const Vector2 Right;
    static const Vector2 UpRight;
    static const Vector2 DownRight;
    static const Vector2 UpLeft;
    static const Vector2 DownLeft;

    // Returns the magnitude of this vector
    float getMagnitude() const {
        return sqrt((x * x) + (y * y));
    }

    float getMaxValue() {
        return std::max(x, y);
    }

    float getMinValue() {
        return std::min(x, y);
    }

    // Normalizes this vector to have a magnitude of 1
    void normalize() {
        float magnitude = getMagnitude();

        if (magnitude != 0.0f) {
            x /= magnitude;
            y /= magnitude;
        }
    }

    // Returns a new Vector2 with a magnitude of 1
    Vector2 normalized() {
        float magnitude = getMagnitude();

        if (magnitude != 0.0f) {
            return Vector2(x / magnitude, y / magnitude);
        }
        return Vector2(0.0f, 0.0f);
    }

    // Flips this vector horizontally (Inverts value of X)
    void flipHorizontally() {
        x = -x;
    }

    // Flips this vector vertically (Inverts value of Y)
    void flipVertically() {
        y = -y;
    }

    // Unary Arithmetic Operators
    Vector2 operator+() const { return {x, y}; }
    Vector2 operator-() const { return {-x, -y}; }

    // Float Arithmetic Operators
    Vector2 operator+(float const &otherValue) const { return {x + otherValue, y + otherValue}; }
    Vector2 operator-(float const &otherValue) const { return {x - otherValue, y - otherValue}; }
    Vector2 operator*(float const &otherValue) const { return {x * otherValue, y * otherValue}; }
    Vector2 operator/(float const &otherValue) const { return {x / otherValue, y / otherValue}; }

    // Vector2 Arithmetic Operators
    Vector2 operator+(const Vector2 &otherVector) const { return {x + otherVector.x, y + otherVector.y}; }
    Vector2 operator-(const Vector2 &otherVector) const { return {x - otherVector.x, y - otherVector.y}; }
    Vector2 operator*(const Vector2 &otherVector) const { return {x * otherVector.x, y * otherVector.y}; }
    Vector2 operator/(const Vector2 &otherVector) const { return {x / otherVector.x, y / otherVector.y}; }

    // Comparison Operators
    bool operator==(const Vector2 &otherVector) const { return x == otherVector.x && y == otherVector.y; }
    bool operator!=(const Vector2 &otherVector) const { return x != otherVector.x || y != otherVector.y; }
    bool operator>(const Vector2 &otherVector) const { return getMagnitude() > otherVector.getMagnitude(); }
    bool operator<(const Vector2 &otherVector) const { return getMagnitude() < otherVector.getMagnitude(); }
    bool operator>=(const Vector2 &otherVector) const { return getMagnitude() >= otherVector.getMagnitude(); }
    bool operator<=(const Vector2 &otherVector) const { return getMagnitude() <= otherVector.getMagnitude(); }

    // Compound Assignment Operators
    Vector2 &operator+=(float const &otherValue) {
        x += otherValue;
        y += otherValue;
        return *this;
    }
    Vector2 &operator-=(float const &otherValue) {
        x -= otherValue;
        y -= otherValue;
        return *this;
    }
    Vector2 &operator*=(float const &otherValue) {
        x *= otherValue;
        y *= otherValue;
        return *this;
    }
    Vector2 &operator/=(float const &otherValue) {
        x /= otherValue;
        y /= otherValue;
        return *this;
    }
    Vector2 &operator+=(const Vector2 &otherVector) {
        x += otherVector.x;
        y += otherVector.y;
        return *this;
    }
    Vector2 &operator-=(const Vector2 &otherVector) {
        x -= otherVector.x;
        y -= otherVector.y;
        return *this;
    }
    Vector2 &operator*=(const Vector2 &otherVector) {
        x *= otherVector.x;
        y *= otherVector.y;
        return *this;
    }
    Vector2 &operator/=(const Vector2 &otherVector) {
        x /= otherVector.x;
        y /= otherVector.y;
        return *this;
    }
};
// Define static direction vectors
const inline Vector2 Vector2::Zero{0.0f, 0.0f};
const inline Vector2 Vector2::Up{0.0f, 1.0f};
const inline Vector2 Vector2::Down{0.0f, -1.0f};
const inline Vector2 Vector2::Left{-1.0f, 0.0f};
const inline Vector2 Vector2::Right{1.0f, 0.0f};
const inline Vector2 Vector2::UpRight{1.0f, 1.0f};
const inline Vector2 Vector2::DownRight{1.0f, -1.0f};
const inline Vector2 Vector2::UpLeft{-1.0f, 1.0f};
const inline Vector2 Vector2::DownLeft{-1.0f, -1.0f};

// Returns a Unit Vector2 direction from an angle in radians
inline Vector2 directionFromRadians(float radians) {
    return Vector2(cos(radians), sin(radians));
}

// Return a Unit Vector2 direction from an angle in degrees (0.0f to 360.0f)
inline Vector2 directionFromDegrees(float degrees) {
    float radians = degrees * math::numbers::pi / 180.0f;
    return Vector2(cos(radians), sin(radians));
}

// Basic Vector2 defined by int x and int y
class Vector2Int {
public:
    // Members
    int x, y;

    // Constructors
    Vector2Int() = default;
    Vector2Int(int x, int y) : x(x), y(y) {}

    // Static direction vectors
    static const Vector2Int Zero;
    static const Vector2Int Up;
    static const Vector2Int Down;
    static const Vector2Int Left;
    static const Vector2Int Right;
    static const Vector2Int UpRight;
    static const Vector2Int DownRight;
    static const Vector2Int UpLeft;
    static const Vector2Int DownLeft;

    // Returns the magnitude of this vector (As float)
    float getMagnitude() const {
        return sqrt(pow(static_cast<float>(x), 2) + pow(static_cast<float>(y), 2));
    }

    int getMaxValue() {
        return std::max(x, y);
    }

    int getMinValue() {
        return std::min(x, y);
    }

    // Flips this vector horizontally (Inverts value of X)
    void flipHorizontally() {
        x = -x;
    }

    // Flips this vector vertically (Inverts value of Y)
    void flipVertically() {
        y = -y;
    }

    // Unary Arithmetic Operators
    Vector2Int operator+() const { return {x, y}; }
    Vector2Int operator-() const { return {-x, -y}; }

    // Int Arithmetic Operators
    Vector2Int operator+(int const &otherValue) const { return {x + otherValue, y + otherValue}; }
    Vector2Int operator-(int const &otherValue) const { return {x - otherValue, y - otherValue}; }
    Vector2Int operator*(int const &otherValue) const { return {x * otherValue, y * otherValue}; }
    Vector2Int operator/(int const &otherValue) const { return {x / otherValue, y / otherValue}; }

    // Vector2Int Arithmetic Operators
    Vector2Int operator+(const Vector2Int &otherVector) const { return {x + otherVector.x, y + otherVector.y}; }
    Vector2Int operator-(const Vector2Int &otherVector) const { return {x - otherVector.x, y - otherVector.y}; }
    Vector2Int operator*(const Vector2Int &otherVector) const { return {x * otherVector.x, y * otherVector.y}; }
    Vector2Int operator/(const Vector2Int &otherVector) const { return {x / otherVector.x, y / otherVector.y}; }

    // Comparison Operators
    bool operator==(const Vector2Int &otherVector) const { return x == otherVector.x && y == otherVector.y; }
    bool operator!=(const Vector2Int &otherVector) const { return x != otherVector.x || y != otherVector.y; }
    bool operator>(const Vector2Int &otherVector) const { return getMagnitude() > otherVector.getMagnitude(); }
    bool operator<(const Vector2Int &otherVector) const { return getMagnitude() < otherVector.getMagnitude(); }
    bool operator>=(const Vector2Int &otherVector) const { return getMagnitude() >= otherVector.getMagnitude(); }
    bool operator<=(const Vector2Int &otherVector) const { return getMagnitude() <= otherVector.getMagnitude(); }

    // Compound Assignment Operators
    Vector2Int &operator+=(int const &otherValue) {
        x += otherValue;
        y += otherValue;
        return *this;
    }
    Vector2Int &operator-=(int const &otherValue) {
        x -= otherValue;
        y -= otherValue;
        return *this;
    }
    Vector2Int &operator*=(int const &otherValue) {
        x *= otherValue;
        y *= otherValue;
        return *this;
    }
    Vector2Int &operator/=(int const &otherValue) {
        x /= otherValue;
        y /= otherValue;
        return *this;
    }
    Vector2Int &operator+=(const Vector2Int &otherVector) {
        x += otherVector.x;
        y += otherVector.y;
        return *this;
    }
    Vector2Int &operator-=(const Vector2Int &otherVector) {
        x -= otherVector.x;
        y -= otherVector.y;
        return *this;
    }
    Vector2Int &operator*=(const Vector2Int &otherVector) {
        x *= otherVector.x;
        y *= otherVector.y;
        return *this;
    }
    Vector2Int &operator/=(const Vector2Int &otherVector) {
        x /= otherVector.x;
        y /= otherVector.y;
        return *this;
    }
};
// Define static direction vectors
const inline Vector2Int Vector2Int::Zero{0, 0};
const inline Vector2Int Vector2Int::Up{0, 1};
const inline Vector2Int Vector2Int::Down{0, -1};
const inline Vector2Int Vector2Int::Left{-1, 0};
const inline Vector2Int Vector2Int::Right{1, 0};
const inline Vector2Int Vector2Int::UpRight{1, 1};
const inline Vector2Int Vector2Int::DownRight{1, -1};
const inline Vector2Int Vector2Int::UpLeft{-1, 1};
const inline Vector2Int Vector2Int::DownLeft{-1, -1};