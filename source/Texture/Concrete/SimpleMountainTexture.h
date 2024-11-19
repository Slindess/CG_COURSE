//
// Created by Михаил Слиняков on 18.11.2024.
//

#ifndef CG_SIMPLEMOUNTAIN_H
#define CG_SIMPLEMOUNTAIN_H

#include "../BaseTexture.h"

class SimpleMountainTexture: public BaseTexture
{
public:
    SimpleMountainTexture() : BaseTexture("../Texture/Images/mount-3.jpg") {}
    virtual const std::type_info& GetType() {return typeid(*this);}
    virtual std::vector<int> GetPixelColor(int x, int y) override;
};


#endif //CG_SIMPLEMOUNTAIN_H
