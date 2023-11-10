#include "../include/Texture.h"

#include <cstddef>
#include <cstdlib>
#include <fmt/core.h>

#include <csetjmp>
#include <fstream>
#include <png.h>

Texture::Texture(const char* filename)
{
    Texture::load(filename);
}

//Texture::Texture(std::string filename)
//{
    //Texture::load(filename);
//}

//Texture::load(std::string filename)
//{
    //Texture::load(file)
//}

void Texture::load(const char* filename)
{
    FILE* image = fopen(filename, "rb");
    if(!image)
    {
    	fmt::print(stderr, "ERR_PNG_0: Невозможно открыть файл текстуры {}!\n", filename);
    	return;
    }
    
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(!png)
    {
    	fmt::print(stderr, "ERR_PNG_1: {}\n", filename);
    	fclose(image);
    	
    	return;
    }
    
    png_infop info = png_create_info_struct(png);
    if(!png)
    {
    	fmt::print(stderr, "ERR_PNG_2: {}\n", filename);
    	png_destroy_read_struct(&png, nullptr, nullptr);
    	fclose(image);

    	return;
    }
    
    //png_infop end_info = png_create_
    //if(!png)
    //{
    	//fmt::print(stderr, "ERR_PNG_2: {}\n", filename);
    	//png_destroy_read_struct(&png, nullptr, nullptr);
    	//fclose(image);
//
    	//return;
    //}
    
    if(setjmp(png_jmpbuf(png)))
    {
    	png_destroy_read_struct(&png, &info, nullptr);
    	fclose(image);

    	return;
    }
    
    png_init_io(png, image);
    png_read_info(png, info);
    
    m_width = png_get_image_width(png, info);
    m_height = png_get_image_height(png, info);
    m_png_info.color_type = png_get_color_type(png, info);
    m_png_info.bit_depth = png_get_bit_depth(png, info);
    
    if(m_png_info.bit_depth == 16)
    {
    	png_set_strip_16(png);
    }
    
    if(m_png_info.color_type == PNG_COLOR_TYPE_PALETTE)
    {
    	png_set_palette_to_rgb(png);
    }
    
    if(m_png_info.color_type == PNG_COLOR_TYPE_GRAY &&
       m_png_info.bit_depth < 8)
    {
    	png_set_expand_gray_1_2_4_to_8(png);
    }
    
    //if(png_get_valid(png, info, PNG_INFO_tRNS))
    //{
    	//png_set_tRNS_to_alpha(png);
    //}
    
    if(m_png_info.color_type == PNG_COLOR_TYPE_RGB ||
       m_png_info.color_type == PNG_COLOR_TYPE_GRAY ||
       m_png_info.color_type == PNG_COLOR_TYPE_PALETTE)
    {
//    	png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
//    	png_set_strip_alpha(png);
    }
    
    png_set_strip_alpha(png);
    
    if(m_png_info.color_type == PNG_COLOR_TYPE_GRAY ||
       m_png_info.color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
       	png_set_gray_to_rgb(png);
    }
    
    png_read_update_info(png, info);
    
    if(m_png_info.row_pointers)
    {
    	png_destroy_read_struct(&png, &info, nullptr);
    	fmt::print(stderr, "ERR_PNG_3: {}\n", filename);

		return;
    }
    
    m_png_info.row_pointers = reinterpret_cast<png_bytep*>(malloc(sizeof(png_bytep) * m_height));
    for(size_t y = 0; y < m_height; y++)
    {
    	m_png_info.row_pointers[y] = reinterpret_cast<png_byte*>(malloc(png_get_rowbytes(png, info)));
    }
    
    png_read_image(png, m_png_info.row_pointers);
    
    fclose(image);
    
    png_destroy_read_struct(&png, &info, nullptr);
}

size_t Texture::get_width()
{
    return m_width;
}

size_t Texture::get_height()
{
    return m_height;
}

unsigned char* Texture::get_data()
{
    //return reinterpret_cast<const unsigned char*>(&m_png_info.row_pointers[0][0]);
    unsigned char* p = new unsigned char[m_width * m_height * 3]; // p - динамический одномерный массив в n*m элементов
    for(size_t i = 0; i < m_height; i++)
    {
        for(size_t j = 0; j < m_width * 3; j++)
        {
            p[m_width * 3 * i + j] = m_png_info.row_pointers[i][j];
        }
    }
    
    return p;
}