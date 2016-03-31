#include "resourcemanager.h"

#include <string>



ResourceManager::ResourceManager(Platform* platform)
{
    this->platform = platform;
}

ResourceManager::~ResourceManager()
{
}

TextureInfo ResourceManager::GetTexture(std::string fileName)
{
    auto it = textureTree.find(fileName);
    TextureInfo result = TextureInfo();
    if (it == textureTree.end())
    {
        result = LoadTexture(fileName);
        textureTree.insert(make_pair(fileName, result));
    }
    else
    {
        result = it->second;
    }
    return result;
}


TextureInfo ResourceManager::LoadTexture(std::string fileName)
{
    File imageFile = platform->ReadEntireFile((char*)fileName.c_str());
    TextureInfo texture;
    BmpHeader* header = (BmpHeader*)imageFile.contents;


    texture.header = header;
    texture.imageData = (ui32*)((ui8*)imageFile.contents + header->bitmapOffset);
    texture.width = header->width;
    texture.height = header->height;
    return texture;

}