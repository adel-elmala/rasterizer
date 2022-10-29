#ifndef _RGBCOLOR_
#define _RGBCOLOR_
class RGBColor
{
public:
    float r, g, b, a;

    RGBColor(void);
    RGBColor(float rChannel, float gChannel, float bChannel);
    RGBColor(const RGBColor &color);
    RGBColor &operator=(const RGBColor &rhs);
    ~RGBColor();
    RGBColor &operator+(const RGBColor &rhs);
    RGBColor &operator-(const RGBColor &rhs);
    RGBColor &operator*(float rhs);
    void CorrectRange(void);

};

#endif