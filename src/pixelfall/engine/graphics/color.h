// Defines a color type (rgba)
// Default is always opaque white

#pragma once
#include <string>
#include "pixelfall/engine/math/vectors.h"

class Color {
public:
    // RGBA (0.0f to 1.0f) is the internal representation of colors, everything else is translated to it. 
    float r, g, b, a;

    // Constructor (RGBA)
    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {
    };

    // Static Factory Methods
    static Color fromRGBA255(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);
    static Color fromHSLA(float h = 0.0f, float s = 0.0f, float l = 1.0f, float a = 1.0f);
    static Color fromHSVA(float h = 0.0f, float s = 0.0f, float v = 1.0f, float a = 1.0f);
    static Color fromHexRGBA(uint32_t hex);
    static Color fromHexcode(const std::string& hexcode);

    // Getters
    Vector3 getRGB255() const;
    Vector4 getRGBA255() const;
    Vector3 getHSL() const;
    Vector4 getHSLA() const;
    Vector3 getHSV() const;
    Vector4 getHSVA() const;
    uint32_t getHexRGB() const;
    uint32_t getHexRGBA() const;
    std::string getHexcode() const;

    // Setters
    void setRGB255(Vector3 color);
    void setRGBA255(Vector4 color);
    void setHSL(Vector3 color);
    void setHSLA(Vector4 color);
    void setHSV(Vector3 color);
    void setHSVA(Vector4 color);
    void setHexRGB(uint32_t color);
    void setHexRGBA(uint32_t color);
    void setHexcode(const std::string& color);

    // Methods
    static bool isValidHexcode(const std::string& hexcode);
};