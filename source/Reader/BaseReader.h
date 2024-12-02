#ifndef __BASE_READER__
#define __BASE_READER__

#include <memory>
#include <string>

#include "../Objects/Concrete/PolygonObject.h"
#include "../Components/BaseComponent.h"
#include "../Components/Concrete/Polygon.h"

class BaseReader
{
public:
    virtual std::shared_ptr<PolygonObject> Read(std::string fileName) = 0;  // Не делайте так никогда.
    virtual ~BaseReader() = 0;
};

#endif