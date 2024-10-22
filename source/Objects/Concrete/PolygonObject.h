#ifndef CG_POLYGONOBJECT_H
#define CG_POLYGONOBJECT_H

#include <initializer_list>
#include "../BaseObject.h"
#include "../../Components/Concrete/Polygon.h"

class PolygonObject : public BaseObject
{
public:
    PolygonObject(std::initializer_list<std::initializer_list<double>> l);
    PolygonObject(std::vector<std::initializer_list<double>>& l);
    PolygonObject(std::vector<Polygon>& polygons);
    PolygonObject(std::vector<std::vector<double>>& l);
    ~PolygonObject();
};

#endif //CG_POLYGONOBJECT_H
