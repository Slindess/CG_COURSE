//
// Created by Михаил Слиняков on 18.11.2024.
//

#ifndef CG_BASETEXTURE_H
#define CG_BASETEXTURE_H

#include <string>
#include <vector>

class BaseTexture
{
public:
    BaseTexture();
    BaseTexture(std::string url) : _url(url) {}
    std::vector<int> GetPixelColor(int x, int y);  // -> [r, g, b]
    virtual ~BaseTexture() = 0;
protected:
    std::string _url;
};

#endif //CG_BASETEXTURE_H
