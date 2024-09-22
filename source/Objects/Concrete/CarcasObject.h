#ifndef CG_CARCASOBJECT_H
#define CG_CARCASOBJECT_H

#include <initializer_list>

#include "../BaseObject.h"
#include "../../Components/Concrete/Line.h"

class CarcasObject : public BaseObject
{
public:
    CarcasObject(std::initializer_list<std::initializer_list<double>> l);
    ~CarcasObject();

};

#endif //CG_CARCASOBJECT_H
