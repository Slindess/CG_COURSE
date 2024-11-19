
#ifndef CG_SNOW_H
#define CG_SNOW_H

#include "../BaseTexture.h"

class SnowTexture: public BaseTexture
{
public:
    SnowTexture() : BaseTexture("/home/slindess/Рабочий\ стол/CG_COURSE/source/Texture/Images/mount-3.jpg") {}
    virtual const std::type_info& GetType() {return typeid(*this);}
    virtual std::vector<int> GetPixelColor(int x, int y) override;
};


#endif //CG_SIMPLEMOUNTAIN_H
