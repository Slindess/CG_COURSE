//
// Created by Михаил Слиняков on 19.11.2024.
//

#include "NoTexture.h"
#include <QImage>
#include <QColor>
#include "../Opener.h"
std::vector<int> NoTexture::GetPixelColor(int x, int y) {
        Opener<NoTexture> opener(*this);
        QImage image = opener.GetImage();
        std::vector<int> colorVec;
        if (image.width() == 0) return colorVec;

        if (x >= 0 && x < image.width() && y >= 0 && y < image.height()) {
            QColor color = image.pixelColor(x, y);
            colorVec.push_back(color.red());
            colorVec.push_back(color.green());
            colorVec.push_back(color.blue());
        }

        return colorVec;
}