// Defines a basic Vector4 and Vector4Int.

#pragma once

class Vector4 {
public:
    // Members
    float x, y, z, w;

    // Constructors
    Vector4(
        float x = 0.0f,
        float y = 0.0f,
        float z = 0.0f,
        float w = 0.0f)
        : x(x), y(y), z(z), w(w) {
    }
};

class Vector4Int {
public:
    // Members
    int x, y, z, w;

    // Constructors
    Vector4Int(
        int x = 0,
        int y = 0,
        int z = 0,
        int w = 0)
        : x(x), y(y), z(z), w(w) {
    }
};