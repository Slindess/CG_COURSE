#ifndef CG_JUNGLE_H
#define CG_JUNGLE_H

#include "../BaseTexture.h"

class JungleTexture: public BaseTexture
{
public:
    JungleTexture() : BaseTexture("../Texture/Images/mount-5.jpg") {}
    virtual const std::type_info& GetType() {return typeid(*this);}
    virtual std::vector<int> GetPixelColor(int x, int y) override;
};


#endif //CG_SIMPLEMOUNTAIN_H
