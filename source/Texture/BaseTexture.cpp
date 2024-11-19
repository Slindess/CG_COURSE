//
// Created by Михаил Слиняков on 19.11.2024.
//

#include "BaseTexture.h"

#include <QImage>
#include <QColor>
#include <vector>
#include <iostream>
#include "Opener.h"
/*
std::vector<int> BaseTexture::GetPixelColor(int x, int y)
{
    //QImage image = QImage(QString::fromStdString(_url));
    Opener o = new Opener(GetType());
    QImage image = o.GetImage();
    std::vector<int> colorVec;
    //std::cout << _url << "\n";
    //std::cout << image.width() << "\n";
    if (x >= 0 && x < image.width() && y >= 0 && y < image.height())
    {
        QColor color = image.pixelColor(x, y);
        colorVec.push_back(color.red());
        colorVec.push_back(color.green());
        colorVec.push_back(color.blue());
    }

    return colorVec;
}*/

BaseTexture::~BaseTexture()
{

}