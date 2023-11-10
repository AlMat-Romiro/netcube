#pragma once

#include <string>

#include <png.h>

class Texture
{
    public:
        Texture(const char* filename);
//        Texture(std::string filename);
        
        void load(const char* filename);
        //void load(std::string filename);
        
        size_t get_width();
        size_t get_height();
        unsigned char* get_data();
        
    private:
        size_t m_width, m_height;   
        struct s_png_info
        {
            png_byte color_type;
            png_byte bit_depth;
            png_bytep* row_pointers = nullptr;
        } m_png_info;            
};
