#include "RGBColor.h"

RGBColor::RGBColor(void) : r(0.0f), g(0.0f), b(0.0f), a{0.0f} {}

RGBColor::RGBColor(float rChannel, float gChannel, float bChannel) : r(rChannel), g(gChannel), b(bChannel), a(0.0f) {}
RGBColor::RGBColor(const RGBColor &color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
RGBColor &RGBColor::operator=(const RGBColor &rhs)
{
    if (this != &rhs)
    {
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        a = rhs.a;
    }
    CorrectRange();

    return *this;
}

RGBColor::~RGBColor() {}

RGBColor &RGBColor::operator+(const RGBColor &rhs)
{
    if (this != &rhs)
    {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        a += rhs.a;
        CorrectRange();
    }
    return *this;
}

RGBColor &RGBColor::operator-(const RGBColor &rhs)
{
    if (this != &rhs)
    {
        r -= rhs.r;
        g -= rhs.g;
        b -= rhs.b;
        a -= rhs.a;
        CorrectRange();
    }
    return *this;
}
RGBColor &RGBColor::operator*(float rhs)
{
    r *= rhs;
    g *= rhs;
    b *= rhs;
    a *= rhs;
    CorrectRange();
    return *this;
}

void RGBColor::CorrectRange(void)
{
    r = r > 1.0f ? 1.0f : r;
    r = r < 0.0f ? 0.0f : r;

    g = g > 1.0f ? 1.0f : g;
    g = g < 0.0f ? 0.0f : g;

    b = b > 1.0f ? 1.0f : b;
    b = b < 0.0f ? 0.0f : b;

    a = a > 1.0f ? 1.0f : a;
    a = a < 0.0f ? 0.0f : a;
}