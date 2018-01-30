//
//  TGAFile.cpp
//  Renderer
//
//  Created by Trevor Smith on 8/1/13.
//  Copyright (c) 2013 Trevor Smith. All rights reserved.
//

#include "TGAFile.h"
#include <fstream>
#include <cmath>

int TGAFile::SaveTGA(void *data, int size, TGAHeader *header, std::string filename)
{
    std::fstream file;
    file.open(filename, std::ios::out|std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "saveTGA: failed to open file " << filename << std::endl;
        return -1;
    }
    
    file.write((char *)header, sizeof(TGAHeader));
    file.write((char *)data, size);
    file.close();
    
    return 0;
}

int TGAFile::SaveTGA(Pixel8 *data, uint16_t width, uint16_t height, std::string filename, uint8_t flags)
{
    TGAHeader header;
    
    // set file attributes in header
    memset(&header, 0, sizeof(header));
    header.imageSpec.imageWidth = width;
    header.imageSpec.imageHeight = height;
    header.imageSpec.pixelDepth = 8; // 8 bits/pixel
    header.imageSpec.descriptor = flags;
    header.imageType = 3; // greyscale
    
    return SaveTGA(data, width*height, &header, filename);
}

int TGAFile::SaveTGA(Pixel16 *data, uint16_t width, uint16_t height, std::string filename, uint8_t flags)
{
    TGAHeader header;
    
    // set file attributes in header
    memset(&header, 0, sizeof(header));
    header.imageSpec.imageWidth = width;
    header.imageSpec.imageHeight = height;
    header.imageSpec.pixelDepth = 16; // 16 bits/pixel
    header.imageSpec.descriptor = flags;
    header.imageType = 2; // color
    
    return SaveTGA(data, width*height*2, &header, filename);
}

int TGAFile::SaveTGA(Pixel24 *data, uint16_t width, uint16_t height, std::string filename, uint8_t flags)
{
    TGAHeader header;
    
    // set file attributes in header
    memset(&header, 0, sizeof(header));
    header.imageSpec.imageWidth = width;
    header.imageSpec.imageHeight = height;
    header.imageSpec.pixelDepth = 24; // 24 bits/pixel
    header.imageSpec.descriptor = flags;
    header.imageType = 2; // color
    
    return SaveTGA(data, width*height*3, &header, filename);
}

void TGAFile::createImage(uint16_t width, uint16_t height, PixelType type)
{
    if (m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    m_width = width;
    m_height = height;
    m_type = type;
    int size;
    switch (type)
    {
        case Gray8:
            size = width*height;
            break;
        case Color16:
            size = width*height*2;
            break;
        case Color24:
            size = width*height*3;
            break;
        default:
            std::cerr << "createImage: invalid image type\n";
            return;
    }
    
    m_data = new char [size];
    memset(m_data, 255, size);
}

void TGAFile::openFile(std::string filename)
{
    int oldSize = (m_data != nullptr) ? m_width * m_height * m_type : 0;

    std::fstream file;
    file.open(filename, std::ios::in|std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "openFile: failed to open file " << filename << std::endl;
        return;
    }
    
    TGAHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(TGAHeader));
    m_width = header.imageSpec.imageWidth;
    m_height = header.imageSpec.imageHeight;
    int size = 0;
    
    if (header.imageSpec.pixelDepth == 8 && header.imageType == 3)
        m_type = Gray8;
    else if (header.imageSpec.pixelDepth == 16 && header.imageType == 2)
        m_type = Color16;
    else if (header.imageSpec.pixelDepth == 24 && header.imageType == 2)
        m_type = Color24;
    else
    {
        std::cerr << "openFile: unsupported image type\n";
        return;
    }

    size = m_width * m_height * m_type;

    if (m_data != nullptr && oldSize != size)
    {
        delete[] m_data;
        m_data = new char [size];
    }
    else
        m_data = new char [size];

    file.read(m_data, size);
    file.close();
}

void TGAFile::saveFile(std::string filename)
{
    if (m_data == nullptr)
        return;
    
    switch (m_type)
    {
        case Gray8:
            SaveTGA(reinterpret_cast<Pixel8*>(m_data), m_width, m_height, filename);
            break;
        case Color16:
            SaveTGA(reinterpret_cast<Pixel16*>(m_data), m_width, m_height, filename);
            break;
        case Color24:
            SaveTGA(reinterpret_cast<Pixel24*>(m_data), m_width, m_height, filename);
            break;
        default:
            break;
    }
}

void TGAFile::createTexture()
{
    if (m_data == nullptr)
        return;

    if (m_texture == 0)
        glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    bufferTexture(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

#if 1
    // Use mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#else
    // Use nearest filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif
}

void TGAFile::bufferTexture(GLenum target)
{
    GLenum channels, order, format;

    switch (m_type)
    {
    case Gray8:
        channels = GL_RED;
        order = GL_RED;
        format = GL_UNSIGNED_BYTE;
        break;
    case Color16:
        channels = GL_RGB;
        order = GL_BGRA;
        format = GL_UNSIGNED_SHORT_1_5_5_5_REV;
        break;
    case Color24:
        channels = GL_RGB;
        order = GL_BGR;
        format = GL_UNSIGNED_BYTE;
        break;
    default:
        return;
    }

    glTexImage2D(target, 0, channels, m_width, m_height, 0, order, format, m_data);
}

void TGAFile::deleteTexture()
{
    // OpenGL does nothing if texture ID is 0
    glDeleteTextures(1, &m_texture);
    m_texture = 0;
}

void TGAFile::setGray(uint16_t x, uint16_t y, Pixel8 value)
{
    if (m_data != nullptr && m_type == Gray8)
        reinterpret_cast<Pixel8*>(m_data)[y*m_width+x] = value;
}

void TGAFile::setRed(uint16_t x, uint16_t y, Pixel8 value)
{
    if (m_data == nullptr)
        return;
    
    if (m_type == Color16)
        reinterpret_cast<Pixel16*>(m_data)[y*m_width+x].red = value;
    else if (m_type == Color24)
        reinterpret_cast<Pixel24*>(m_data)[y*m_width+x].red = value;
    else
        std::cerr << "setting red in non-color image\n";
}

void TGAFile::setGreen(uint16_t x, uint16_t y, Pixel8 value)
{
    if (m_data == nullptr)
        return;
    
    if (m_type == Color16)
        reinterpret_cast<Pixel16*>(m_data)[y*m_width+x].green = value;
    else if (m_type == Color24)
        reinterpret_cast<Pixel24*>(m_data)[y*m_width+x].green = value;
}

void TGAFile::setBlue(uint16_t x, uint16_t y, Pixel8 value)
{
    if (m_data == nullptr)
        return;
    
    if (m_type == Color16)
        reinterpret_cast<Pixel16*>(m_data)[y*m_width+x].blue = value;
    else if (m_type == Color24)
        reinterpret_cast<Pixel24*>(m_data)[y*m_width+x].blue = value;
}

TGAFile::Pixel8 TGAFile::getGray(uint16_t x, uint16_t y)
{
    if (m_data != nullptr && m_type == Gray8)
        return reinterpret_cast<Pixel8*>(m_data)[y*m_width+x];
    
    return 0;
}

TGAFile::Pixel8 TGAFile::getRed(uint16_t x, uint16_t y)
{
    if (m_data == nullptr)
        return 0;
    
    if (m_type == Color16)
        return reinterpret_cast<Pixel16*>(m_data)[y*m_width+x].red;
    else if (m_type == Color24)
        return reinterpret_cast<Pixel24*>(m_data)[y*m_width+x].red;
    
    std::cerr << "getting red in non-color image\n";
    
    return 0;
}

TGAFile::Pixel8 TGAFile::getGreen(uint16_t x, uint16_t y)
{
    if (m_data == nullptr)
        return 0;
    
    if (m_type == Color16)
        return reinterpret_cast<Pixel16*>(m_data)[y*m_width+x].green;
    else if (m_type == Color24)
        return reinterpret_cast<Pixel24*>(m_data)[y*m_width+x].green;
    
    return 0;
}

TGAFile::Pixel8 TGAFile::getBlue(uint16_t x, uint16_t y)
{
    if (m_data == nullptr)
        return 0;
    
    if (m_type == Color16)
        return reinterpret_cast<Pixel16*>(m_data)[y*m_width+x].blue;
    else if (m_type == Color24)
        return reinterpret_cast<Pixel24*>(m_data)[y*m_width+x].blue;
    
    return 0;
}

TGAFile::Pixel8 TGAFile::getGray(float x, float y)
{
    // Coerce into [0, 1)
    x -= floor(x);
    y -= floor(y);
    return getGray(x*m_width, y*m_height);
}

TGAFile::Pixel8 TGAFile::getRed(float x, float y)
{
    // Coerce into [0, 1)
    x = (x-floor(x))*m_width;
    y = (y-floor(y))*m_height;
    
    // Think about doing biliear sample here.
    return getRed(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
}

TGAFile::Pixel8 TGAFile::getGreen(float x, float y)
{
    // Coerce into [0, 1)
    x -= floor(x);
    y -= floor(y);
    return getGreen(static_cast<uint16_t>(x*m_width), static_cast<uint16_t>(y*m_height));
}

TGAFile::Pixel8 TGAFile::getBlue(float x, float y)
{
    // Coerce into [0, 1)
    x -= floor(x);
    y -= floor(y);
    uint16_t xi = x*m_width;
    uint16_t yi = y*m_height;
    return getBlue(xi, yi);
}
