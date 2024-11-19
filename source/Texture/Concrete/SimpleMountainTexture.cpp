//
// Created by Михаил Слиняков on 18.11.2024.
//

#include "SimpleMountainTexture.h"
#include <QImage>
#include <QColor>
#include "../Opener.h"

std::vector<int> SimpleMountainTexture::GetPixelColor(int x, int y){
        Opener<SimpleMountainTexture> opener(*this);
        QImage image = opener.GetImage();
        std::vector<int> colorVec;
        if (x > image.width()) x = image.width() -1 ;
        if (y > image.height()) x = image.height() -1 ;
        if (x >= 0 && x < image.width() && y >= 0 && y < image.height()) {
            QColor color = image.pixelColor(x, y);
            colorVec.push_back(color.red());
            colorVec.push_back(color.green());
            colorVec.push_back(color.blue());
        }

        return colorVec;
}