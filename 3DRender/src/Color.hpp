#pragma once

class Color
{
public:
	Color() : Color(0.0f, 0.0f, 0.0f, 1.0f) {}
	Color(float r, float g, float b, float a = 1.0f) : m_red(r), m_green(g), m_blue(b), m_alpha(a){}

    inline float GetRed() const { return m_red; };
    inline void SetRed(float red) { m_red = red; };

    inline float GetGreen() const { return m_green; };
    inline void SetGreen(float green) { m_green = green; };

    inline float GetBlue() const { return m_blue; };
    inline void SetBlue(float blue) { m_blue = blue; };

    inline float GetAlpha() const { return m_alpha; };
    inline void SetAlpha(float alpha) { m_alpha = alpha; };


private:
	float m_red, m_green, m_blue, m_alpha;
};