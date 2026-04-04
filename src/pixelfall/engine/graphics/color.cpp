// Implementation for color.h

#include "pixelfall/engine/graphics/color.h"
// Standard Library
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

// Static Factory Methods
Color Color::fromRGBA255(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

Color Color::fromHSLA(float h, float s, float l, float a) {
    // Achromatic case
    if (s == 0.0f)
        return Color(l, l, l, a);

    auto hue2rgb = [](float p, float q, float t) -> float {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
    };

    float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
    float p = 2.0f * l - q;
    float r = hue2rgb(p, q, h + 1.0f / 3.0f);
    float g = hue2rgb(p, q, h);
    float b = hue2rgb(p, q, h - 1.0f / 3.0f);
    return Color(r, g, b, a);
}

Color Color::fromHSVA(float h, float s, float v, float a) {
    if (s == 0.0f)
        return Color(v, v, v, a);

    int i = (int)(h * 6.0f);
    float f = h * 6.0f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);

    switch (i % 6) {
        case 0: return Color(v, t, p, a);
        case 1: return Color(q, v, p, a);
        case 2: return Color(p, v, t, a);
        case 3: return Color(p, q, v, a);
        case 4: return Color(t, p, v, a);
        case 5: return Color(v, p, q, a);
    }
    return Color(v, v, v, a);
}

Color Color::fromHexRGBA(uint32_t hex) {
    float r = ((hex >> 24) & 0xFF) / 255.0f;
    float g = ((hex >> 16) & 0xFF) / 255.0f;
    float b = ((hex >> 8)  & 0xFF) / 255.0f;
    float a = ((hex >> 0)  & 0xFF) / 255.0f;
    return Color(r, g, b, a);
}

Color Color::fromHexcode(const std::string& hexcode) {
    if (isValidHexcode(hexcode)) {
        std::string cleanHexcode = hexcode;
        if (hexcode[0] == '#')
            cleanHexcode = hexcode.substr(1);

        // Transform the characters from base 16 into a number in groups of two (Red, Green, and Blue Channels)
        float r = (uint8_t)std::stoul(cleanHexcode.substr(0, 2), nullptr, 16) / 255.0f;
        float g = (uint8_t)std::stoul(cleanHexcode.substr(2, 2), nullptr, 16) / 255.0f;
        float b = (uint8_t)std::stoul(cleanHexcode.substr(4, 2), nullptr, 16) / 255.0f;
        float a = 1.0f; // Opaque

        // Check if this colour has alpha information
        if (cleanHexcode.size() == 8)
            a = (uint8_t)std::stoul(cleanHexcode.substr(6, 2), nullptr, 16) / 255.0f;

        // Create RGBA Colour from values
        return Color(r, g, b, a);
    }
    std::cout << "Error: Invalid hexcode, returning white: " << hexcode << std::endl;
    return Color();
}

// Getters
Vector3 Color::getRGB255() const {
    return Vector3(r * 255, g * 255, b * 255);
}
Vector4 Color::getRGBA255() const {
    return Vector4(r * 255, g * 255, b * 255, a * 255);
}
Vector3 Color::getHSL() const {
    float max = std::max({ r, g, b });
    float min = std::min({ r, g, b });
    float l = (max + min) / 2.0f;

    if (max == min)
        return Vector3(0.0f, 0.0f, l); // achromatic

    float d = max - min;
    float s = l > 0.5f ? d / (2.0f - max - min) : d / (max + min);
    float h;
    if      (max == r) h = (g - b) / d + (g < b ? 6.0f : 0.0f);
    else if (max == g) h = (b - r) / d + 2.0f;
    else               h = (r - g) / d + 4.0f;
    h /= 6.0f;

    return Vector3(h, s, l);
}

Vector4 Color::getHSLA() const {
    Vector3 hsl = getHSL();
    return Vector4(hsl.x, hsl.y, hsl.z, a);
}

Vector3 Color::getHSV() const {
    float max = std::max({ r, g, b });
    float min = std::min({ r, g, b });
    float d = max - min;
    float v = max;
    float s = max == 0.0f ? 0.0f : d / max;
    float h = 0.0f;

    if (d != 0.0f) {
        if      (max == r) h = (g - b) / d + (g < b ? 6.0f : 0.0f);
        else if (max == g) h = (b - r) / d + 2.0f;
        else               h = (r - g) / d + 4.0f;
        h /= 6.0f;
    }

    return Vector3(h, s, v);
}

Vector4 Color::getHSVA() const {
    Vector3 hsv = getHSV();
    return Vector4(hsv.x, hsv.y, hsv.z, a);
}

uint32_t Color::getHexRGB() const {
    return ((uint32_t)(r * 255) << 16) |
           ((uint32_t)(g * 255) << 8)  |
           ((uint32_t)(b * 255));
}

uint32_t Color::getHexRGBA() const {
    return ((uint32_t)(r * 255) << 24) |
           ((uint32_t)(g * 255) << 16) |
           ((uint32_t)(b * 255) << 8)  |
           ((uint32_t)(a * 255));
}

std::string Color::getHexcode() const {
    std::ostringstream ss;
    ss << "#"
       << std::hex << std::uppercase << std::setfill('0')
       << std::setw(2) << (int)(r * 255)
       << std::setw(2) << (int)(g * 255)
       << std::setw(2) << (int)(b * 255)
       << std::setw(2) << (int)(a * 255);
    return ss.str();
}

// Setters
void Color::setRGB255(Vector3 color) {
    *this = fromRGBA255(color.x, color.y, color.z);
}
void Color::setRGBA255(Vector4 color) {
    *this = fromRGBA255(color.x, color.y, color.z, color.w);
}
void Color::setHSL(Vector3 color) {
    *this = fromHSLA(color.x, color.y, color.z);
}
void Color::setHSLA(Vector4 color) {
    *this = fromHSLA(color.x, color.y, color.z, color.w);
}
void Color::setHSV(Vector3 color) {
    *this = fromHSVA(color.x, color.y, color.z);
}
void Color::setHSVA(Vector4 color) {
    *this = fromHSVA(color.x, color.y, color.z, color.w);
}
void Color::setHexRGB(uint32_t color) {
    *this = fromHexRGBA(color << 8 | 0xFF);
}
void Color::setHexRGBA(uint32_t color) {
    *this = fromHexRGBA(color);
}
void Color::setHexcode(const std::string& color) {
    *this = fromHexcode(color);
}

// Used to check if a hexcode is valid (Format is either #FFFFFFF, FFFFFF, #FFFFFF00 or FFFFFF00)
bool Color::isValidHexcode(const std::string& hexcode) {
    // Make sure the string is not empty
    if (hexcode.empty()) return false;

    // Remove optional # character
    std::string cleanHexcode = hexcode;
    if (hexcode[0] == '#')
        cleanHexcode = hexcode.substr(1);

    // Check that the hexcode is the correct size (6 for opaque, 8 for alpha)
    if (cleanHexcode.size() == 6 || cleanHexcode.size() == 8) {
        // Check if all characters are valid hex characters
        return std::all_of(cleanHexcode.begin(), cleanHexcode.end(), [](char c) {
            return std::isxdigit(c); // true for 0-9, a-f, A-F
            });
    }
    return false;
}