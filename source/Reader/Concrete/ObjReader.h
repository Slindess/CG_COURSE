#ifndef __OBJREADER__
#define __OBJREADER__

#include <memory>
#include <string>

#include "../BaseReader.h"
#include "../../Objects/Concrete/PolygonObject.h"
#include "../../Components/BaseComponent.h"
#include "../../Components/Concrete/Polygon.h"
#include "../../Texture/BaseTexture.h"
#include "../../Texture/Concrete/SimpleMountainTexture.h"
#include "../../Texture/Concrete/SnowTexture.h"
#include "../../Texture/Concrete/SandTexture.h"
#include "../../Texture/Concrete/JungleTexture.h"
#include <map>
#include <functional>

class ObjReader : public BaseReader
{
public:
    ObjReader() {
        textureFactories[0] = []() {return std::make_shared<NoTexture>();};
        textureFactories[1] = []() {return std::make_shared<SimpleMountainTexture>();};
        textureFactories[2] = []() {return std::make_shared<SandTexture>();};
        textureFactories[3] = []() {return std::make_shared<JungleTexture>();};
    }
    std::shared_ptr<PolygonObject> Read(std::string fileName, int type) override;
    ~ObjReader() = default;

private:
    std::map<int, std::function<std::shared_ptr<BaseTexture>()>> textureFactories;
};

#endif