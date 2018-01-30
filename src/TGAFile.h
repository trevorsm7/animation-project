//
//  TGAFile.h
//  Renderer
//
//  Created by Trevor Smith on 8/1/13.
//  Copyright (c) 2013 Trevor Smith. All rights reserved.
//

#ifndef __Renderer__TGAFile__
#define __Renderer__TGAFile__

#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#include <iostream>
#include <stdint.h>
#include <string>

class TGAFile
{
protected:
    struct TGAColorMapSpec
    {
        uint16_t offsetBytes;
        uint16_t entryCount;
        uint8_t entrySize;
    }
    __attribute__((__packed__));
    
    struct TGAImageSpec
    {
        uint16_t xOrigin; // 0?
        uint16_t yOrigin; // 0?
        uint16_t imageWidth;
        uint16_t imageHeight;
        uint8_t pixelDepth; // 8, 16, 24, etc
        uint8_t descriptor; // [..vhaaaa] vertical/horizontal flip, alpha bits
    }
    __attribute__((__packed__));
    
    struct TGAHeader
    {
        uint8_t idLength; // 0
        uint8_t colorMapType; // 0
        uint8_t imageType; // 2 for color, 3 for grey
        TGAColorMapSpec colorMapSpec; // 0, 0, 0
        TGAImageSpec imageSpec;
        
    }
    __attribute__((__packed__));
    
    static int SaveTGA(void *data, int size, TGAHeader *header, std::string filename);
    
public:
    struct Pixel24
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    }
    __attribute__((__packed__));
    
    struct Pixel16
    {
        uint16_t blue:5;
        uint16_t green:5;
        uint16_t red:5;
        uint16_t alpha:1; // specify in image descriptor?
    }
    __attribute__((__packed__));
    
    typedef uint8_t Pixel8;
    
    enum PixelType {Gray8=1, Color16=2, Color24=3};
    
    static const uint8_t HorizontalFlip = 0x10;
    static const uint8_t VerticalFlip = 0x20;
    
    static int SaveTGA(Pixel8 *data, uint16_t width, uint16_t height, std::string filename, uint8_t flags=0);
    static int SaveTGA(Pixel16 *data, uint16_t width, uint16_t height, std::string filename, uint8_t flags=0);
    static int SaveTGA(Pixel24 *data, uint16_t width, uint16_t height, std::string filename, uint8_t flags=0);
    
protected:
    char *m_data;
    uint16_t m_width, m_height;
    PixelType m_type;
    GLuint m_texture;
    
public:
    TGAFile(): m_data(nullptr), m_texture(0) {}
    ~TGAFile() {deleteTexture(); if (m_data != nullptr) delete[] m_data;}
    
    void createImage(uint16_t width, uint16_t height, PixelType type);
    void openFile(std::string filename);
    void saveFile(std::string filename);
    
    // OpenGL functions
    void createTexture();
    void bufferTexture(GLenum target);
    void deleteTexture();
    void bindTexture() {glBindTexture(GL_TEXTURE_2D, m_texture);}
    
    uint16_t getWidth() {return (m_data == nullptr) ? 0 : m_width;}
    uint16_t getHeight() {return (m_data == nullptr) ? 0 : m_height;}
    PixelType getType() {return m_type;}
    
    void setGray(uint16_t x, uint16_t y, Pixel8 value);
    void setRed(uint16_t x, uint16_t y, Pixel8 value);
    void setGreen(uint16_t x, uint16_t y, Pixel8 value);
    void setBlue(uint16_t x, uint16_t y, Pixel8 value);
    
    Pixel8 getGray(uint16_t x, uint16_t y);
    Pixel8 getRed(uint16_t x, uint16_t y);
    Pixel8 getGreen(uint16_t x, uint16_t y);
    Pixel8 getBlue(uint16_t x, uint16_t y);
    
    Pixel8 getGray(float x, float y);
    Pixel8 getRed(float x, float y);
    Pixel8 getGreen(float x, float y);
    Pixel8 getBlue(float x, float y);
};

#endif /* defined(__Renderer__TGAFile__) */
