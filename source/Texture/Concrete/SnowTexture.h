
#ifndef CG_SNOW_H
#define CG_SNOW_H

#include "../BaseTexture.h"

class SnowTexture: public BaseTexture
{
public:
    SnowTexture() : BaseTexture("../Texture/Images/mount-2.png") {}
    virtual const std::type_info& GetType() {return typeid(*this);}
    virtual std::vector<int> GetPixelColor(int x, int y) override;
};


#endif //CG_SIMPLEMOUNTAIN_H
