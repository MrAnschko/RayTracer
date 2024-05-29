// taken from https://youtu.be/vqT5j38bWGg?si=OqEuJI-hCPmNBIYC
// mostly
#ifndef IMAGE_H
#define IMAGE_H
#include <vector>

struct Color {
    float r,g, b;

    Color Lerp(Color col_1, Color col_2, float t) const; // this is just a normal lerp. A color lerp would be bette but I didn't want to do hsv & rgb conversion
    Color();
    Color(float r, float g, float b);
    
    ~Color();

};

class Image
{
public: 
    Image();
    Image(int width, int height);
    ~Image();
    Color GetColor(int x, int y) const;
    static Color RandomColor();
    void SetColor(const Color& col, int x, int y);
    void Export(const char* path) const;
private:
    int m_width;
    int m_height;
    std:: vector<Color> m_colors;

};
#endif