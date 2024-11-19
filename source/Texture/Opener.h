#ifndef OPENER_H
#define OPENER_H

#include <QImage>

template <typename T>
class Opener {
public:
    static bool isOpen; 
    static QImage image;

    Opener(const T& obj)
    {
        if (!isOpen)
        {
            QString url = QString::fromStdString(obj.GetUrl());
            image = QImage(url);  
            isOpen = true;  
        }
    }

    QImage GetImage() {return image;};
};

template <typename T>
bool Opener<T>::isOpen = false;  // Изначально картинка не загружена

template <typename T>
QImage Opener<T>::image = QImage();

#endif