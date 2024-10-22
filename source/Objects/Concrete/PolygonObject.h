#ifndef CG_POLYGONOBJECT_H
#define CG_POLYGONOBJECT_H

#include <initializer_list>
#include "../BaseObject.h"


class PolygonObject : public BaseObject
{
public:
    PolygonObject(std::initializer_list<std::initializer_list<double>> l);
    ~PolygonObject();
};

#endif //CG_POLYGONOBJECT_H
