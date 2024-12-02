#ifndef __OBJREADER__
#define __OBJREADER__

#include <memory>
#include <string>

#include "../BaseReader.h"
#include "../../Objects/Concrete/PolygonObject.h"
#include "../../Components/BaseComponent.h"
#include "../../Components/Concrete/Polygon.h"

class ObjReader : public BaseReader
{
public:
    std::shared_ptr<PolygonObject> Read(std::string fileName) override;
    ~ObjReader() = default;
};

#endif