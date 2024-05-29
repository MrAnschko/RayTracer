#include "Image.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

Color Color::Lerp(Color col_1, Color col_2, float t) const
{
    float r_ = (1.0f - t)*col_1.r + col_2.r * t;
    float g_ = (1.0f - t)*col_1.g + col_2.g * t;
    float b_ = (1.0f - t)*col_1.b + col_2.b * t;
    return Color(r_,g_,b_);
}

Color::Color()
    : r(0), g(0), b(0)
{
}

Color::Color(float r, float g, float b)
 : r(r),g(g),b(b)
{
}

Color::~Color()
{
}
Image::Image(){
    m_width = 0;
    m_height = 0;
    m_colors = std::vector<Color>(0);
}

Image::Image(int width, int height)
: m_width(width),m_height(height),m_colors(std::vector<Color>(width*height))
{
}

Image::~Image()
{
}

Color Image::GetColor(int x, int y) const
{
    return m_colors[y*m_width+x];
}

//makes a random color
Color Image::RandomColor()
{
    //std::srand(time(nullptr)); // use current time as seed for random generator
    float r = static_cast< float >(std::rand()) / static_cast< float >(RAND_MAX);
    float g = static_cast< float >(std::rand()) / static_cast< float >(RAND_MAX);
    float b = static_cast< float >(std::rand()) / static_cast< float >(RAND_MAX);

    return Color(r,g,b);
}

void Image::SetColor(const Color& col, int x, int y)
{
    m_colors[y*m_width+x].r = col.r;
    m_colors[y*m_width+x].g = col.g;
    m_colors[y*m_width+x].b = col.b;
    
}


void Image::Export(const char *path) const
{
    std::ofstream f;
    f.open(path, std::ios::out|std::ios::binary);

    if (!f.is_open())
    {
        std::cout << "File could not be opened!\n";
        return;
    }

    unsigned char bmpPad[3] = {0,0,0};
    const int paddingAmount = ((4-(m_width*3)%4)%4);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize * informationHeaderSize + m_width * m_height*3 + paddingAmount * m_height;

    unsigned char fileHeader[fileHeaderSize];

    //File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize>>8;
    fileHeader[4] = fileSize>>16;
    fileHeader[5] = fileSize>>24;
    // Resereved (Not Used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // Pixel Data offset
    fileHeader[10]= fileHeaderSize+informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];
    // Header Size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    //
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;
    //
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;
    //
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    //Bits per Pixel (rgb)
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    //
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    //
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    //
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    //
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    //
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    //
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader),fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader),informationHeaderSize);

    for(int y = m_height-1; y>= 0 ; y--)
    {
        for(int x = 0; x <m_width;x++)
        {
            float r_s = GetColor(x,y).r * 255.0f;
            r_s = r_s > 255? 255 : r_s;  
            float g_s = GetColor(x,y).g * 255.0f;
            g_s = g_s > 255? 255 : g_s;  
            float b_s = GetColor(x,y).b * 255.0f;
            b_s = b_s > 255? 255 : b_s;
            unsigned char r = static_cast<unsigned char>( r_s);
            unsigned char g = static_cast<unsigned char>( g_s);
            unsigned char b = static_cast<unsigned char>( b_s);
            unsigned char color[] = {b,g,r};
            f.write(reinterpret_cast<char*>(color),3); 
        }
        f.write(reinterpret_cast<char*>(bmpPad),paddingAmount);
    }

    f.close();

    std::cout << "File Created\n";



}
