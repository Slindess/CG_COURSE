//
// Created by Михаил Слиняков on 19.11.2024.
//

#include "BaseTexture.h"

#include <QImage>
#include <QColor>
#include <vector>

std::vector<int> BaseTexture::GetPixelColor(int x, int y)
{
    QString imagePath = QString::fromStdString(_url);;
    QImage image(imagePath);
    std::vector<int> colorVec;

    if (x >= 0 && x < image.width() && y >= 0 && y < image.height())
    {
        QColor color = image.pixelColor(x, y);
        colorVec.push_back(color.red());
        colorVec.push_back(color.green());
        colorVec.push_back(color.blue());
    }

    return colorVec;
}

BaseTexture::~BaseTexture()
{

}