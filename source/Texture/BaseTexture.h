//
// Created by Михаил Слиняков on 18.11.2024.
//

#ifndef CG_BASETEXTURE_H
#define CG_BASETEXTURE_H

#include <string>
#include <vector>
#include <QImage>
#include <typeinfo>

class BaseTexture
{
public:
    BaseTexture();
    BaseTexture(std::string url) : _url(url) {}
    std::string GetUrl() const {return _url;};
    virtual std::vector<int> GetPixelColor(int x, int y) = 0;  // -> [r, g, b]
    virtual const std::type_info& GetType() = 0;
    virtual ~BaseTexture() = 0;
protected:
    std::string _url;
};

#endif //CG_BASETEXTURE_H
