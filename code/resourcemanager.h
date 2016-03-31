#pragma once
#include "typedefs.h"
#include "platform.h"
#include "shaders.h"

#include <map>

#pragma pack(push, 1)
struct BmpHeader
{

    ui16   fileType;     /* File type, always 0x4D42 ("BM") */
    ui32  fileSize;     /* Size of the file in bytes */
    ui16   reserved1;    /* Always 0 */
    ui16   reserved2;    /* Always 0 */
    ui32  bitmapOffset; /* Starting position of image data in bytes */
    ui32 sizeHeader;            /* Size of this header in bytes */
    i32  width;           /* Image width in pixels */
    i32  height;          /* Image height in pixels */
    ui16  planes;          /* Number of color planes */
    ui16  bitsPerPixel;    /* Number of bits per pixel */
    ui32 compression;     /* Compression methods used */
    ui32 sizeOfBitmap;    /* Size of bitmap in bytes */
    i32  horzResolution;  /* Horizontal resolution in pixels per meter */
    i32  vertResolution;  /* Vertical resolution in pixels per meter */
    ui32 colorsUsed;      /* Number of colors in the image */
    ui32 colorsImportant; /* Minimum number of important colors */
    ui32 redMask;       /* Mask identifying bits of red component */
    ui32 greenMask;     /* Mask identifying bits of green component */
    ui32 blueMask;      /* Mask identifying bits of blue component */
    ui32 alphaMask;     /* Mask identifying bits of alpha component */
};
#pragma pack(pop)


struct TextureInfo
{
    BmpHeader* header;
    ui32* imageData;
    ui32 textureID;
    i32 width;
    i32 height;


};


class ResourceManager
{
public:
    ResourceManager(Platform* platform);
    ~ResourceManager();


    TextureInfo GetTexture(std::string fileName);
private:
    TextureInfo LoadTexture(std::string fileName);
    std::map<std::string, TextureInfo> textureTree;
    //std::map<std::string, ShaderInfo> shadertree;
    Platform* platform;

};