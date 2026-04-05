// Defines a basic Vector3 and Vector3Int.

#pragma once

class Vector3 {
public:
    // Members
    float x, y, z;

    // Constructors
    Vector3(
        float x = 0.0f,
        float y = 0.0f,
        float z = 0.0f)
        : x(x), y(y), z(z) {
    }
};

class Vector3Int {
public:
    // Members
    int x, y, z;

    // Constructors
    Vector3Int(
        int x = 0,
        int y = 0,
        int z = 0)
        : x(x), y(y), z(z) {
    }
};