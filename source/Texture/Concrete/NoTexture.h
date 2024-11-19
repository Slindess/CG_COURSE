//
// Created by Михаил Слиняков on 19.11.2024.
//

#ifndef CG_NOTEXTURE_H
#define CG_NOTEXTURE_H

#include "../BaseTexture.h"

/* Заглушка для отсутствия текстуры
Метод получения пикселя вернет пустой вектор */
class NoTexture : public BaseTexture
{
public:
    NoTexture() : BaseTexture("../Images/no.jpg") {}
};


#endif //CG_NOTEXTURE_H
