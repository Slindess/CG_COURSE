//
// Created by Михаил Слиняков on 18.11.2024.
//

#ifndef CG_BASETEXTURE_H
#define CG_BASETEXTURE_H

#include <string>
#include <vector>
#include <QImage>

class BaseTexture
{
public:
    BaseTexture();
    BaseTexture(std::string url) : _url(url) {_image = QImage(QString::fromStdString(_url));}
    std::string GetUrl(){return _url;};
    std::vector<int> GetPixelColor(int x, int y);  // -> [r, g, b]
    virtual ~BaseTexture() = 0;
protected:
    std::string _url;
    QImage _image = QImage("NoTexture.jpg");
};

#endif //CG_BASETEXTURE_H
