#ifndef CG_SAND_H
#define CG_SAND_H

#include "../BaseTexture.h"

class SandTexture: public BaseTexture
{
public:
    SandTexture() : BaseTexture("../Texture/Images/mount-3.jpg") {}
    virtual const std::type_info& GetType() {return typeid(*this);}
    virtual std::vector<int> GetPixelColor(int x, int y) override;
};


#endif //CG_SIMPLEMOUNTAIN_H
